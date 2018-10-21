
#ifndef OBJECT_CPP
#define OBJECT_CPP

#include <Object.h>
#include <Engine.h>

void Object::NextOverlappingFrame()
{
	for( auto it = overlappingInPreviousFrame.begin(); it != overlappingInPreviousFrame.end(); ++it )
	{
		if( overlappingInCurrentFrame.find( *it ) == overlappingInCurrentFrame.end() )
		{
			EventOnObjectEndOverlapp( *it );
		}
	}
	
	overlappingInPreviousFrame = overlappingInCurrentFrame;
	overlappingInCurrentFrame.clear();
}

void Object::OverlapWithObject( Object * other, btPersistentManifold * perisstentManifold )
{
	if( perisstentManifold )
	{
		if( other )
		{
			if( overlappingInPreviousFrame.find( other ) != overlappingInPreviousFrame.end() )
			{
				EventOnObjectTickOverlapp( other, perisstentManifold );
			}
			else
			{
				EventOnObjectBeginOverlapp( other, perisstentManifold );
			}
			overlappingInCurrentFrame.insert( other );
		}
		else
		{
			DEBUG( "other = NULL" );
		}
	}
	else
	{
		DEBUG( "perisstentManifold = NULL" );
	}
}

void Object::EventOnObjectBeginOverlapp( Object * other, btPersistentManifold * perisstentManifold )
{
}

void Object::EventOnObjectTickOverlapp( Object * other, btPersistentManifold * perisstentManifold )
{
}

void Object::EventOnObjectEndOverlapp( Object * other )
{
}

SmartPtr<Object> Object::GetThis()
{
	if( !thisPtr )
	{
		if( engine )
		{
			thisPtr = engine->GetObject( std::string(name) );
		}
		else
		{
			DEBUG("Error creating SmartPtr Object::thisPtr ");
		}
	}
	return thisPtr;
}



void Object::Tick( const float deltaTime )
{
	if( body )
	{
		previousTransform = currentTransform;
		body->getMotionState()->getWorldTransform( currentTransform );
	}
	
	if( currentTransform.getOrigin().y() < -1000.0 )
	{
		engine->QueueObjectToDestroy( name );
	}
}

void Object::ApplyDamage( const float damage, btVector3 point, btVector3 normal )
{
}

void Object::ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal )
{
	if( body )
	{
		if( normal.dot( direction ) > 0 )
			normal *= -1;
		
		body->activate( true );
		body->applyImpulse( direction.normalized() * impetus,  point - currentTransform.getOrigin() );
		body->activate( true );
	}
}

void Object::SetMass( float mass )
{
	if( IsDynamic() )
	{
		this->mass = mass;
		if( body )
		{
			btCollisionShape * shape = body->getCollisionShape();
			if( shape )
			{
				btVector3 inertia = body->getInvInertiaDiagLocal();
				shape->calculateLocalInertia( mass, inertia );
				body->setMassProps( mass, inertia );
			}
		}
	}
}

bool Object::IsDynamic() const
{
	if( body )
	{
		if( mass > 0.0f )
		{
			return true;
		}
	}
	return false;
}

Engine * Object::GetEngine()
{
	return engine;
}

void Object::SetScale( btVector3 scale )
{
	this->scale = scale;
	if( body )
	{
		body->activate( true );
		body->getCollisionShape()->setLocalScaling( scale );
		engine->GetWorld()->UpdateColliderForObject( body );
		CalculateRadius();
		body->activate( true );
	}
}

btVector3 Object::GetScale()
{
	return scale;
}

btTransform Object::GetTransform()
{
	btTransform transform;
	if( body )
		body->getMotionState()->getWorldTransform( transform );
	return transform;
}

SmartPtr<btRigidBody> Object::GetBody()
{
	return body;
}

void Object::Draw()
{
	if( model )
	{
		engine->GetCamera()->SetWorldTransform( GetTransform(), scale );
		model->Draw();
	}
}

void Object::SetModel( SmartPtr<Model> model )
{
	this->model = model;
}

btVector3 Object::GetLocation() const
{
	if( body )
	{
		btTransform transform;
		body->getMotionState()->getWorldTransform( transform );
		return transform.getOrigin();
	}
	return btVector3();
}

float Object::GetRadius()
{
	return boundingSphereRadius;
}

void Object::CalculateRadius()
{
	if( body )
	{
		btVector3 min, max, origin;
		body->getAabb( min, max );
		origin = GetLocation();
		min = (min-origin).absolute();
		max = (max-origin).absolute();
		
		for(int i = 0; i < 3; ++i )
			if( min.m_floats[i] > max.m_floats[i] )
				max.m_floats[i] = min.m_floats[i];
		
		boundingSphereRadius = max.length();
	}
	else
	{
		boundingSphereRadius = 0.01f;
	}
}

void Object::SetRayTraceChannel( int src )
{
	rayTraceChannel = src;
}

int Object::GetRayTraceChannel()
{
	return rayTraceChannel;
}

std::string Object::GetName() const
{
	return name;
}

Object::Object( Engine * engine, std::string name, SmartPtr<btRigidBody> body, SmartPtr<btCollisionShape> collisionShape, float mass_ ) :
	mass(mass_)
{
	this->collisionShape = collisionShape;
	this->engine = engine;
	this->name = name;
	this->body = body;
	model = NULL;
	scale = btVector3(1,1,1);
	CalculateRadius();
	rayTraceChannel = Engine::RayTraceChannel::COLLIDING | Engine::RayTraceChannel::NOT_TRANSPARENT;
}

Object::Object() :
	mass(1.0)
{
	engine = NULL;
	name = "";
	scale = btVector3(1,1,1);
	boundingSphereRadius = 1.0f;
	rayTraceChannel = Engine::RayTraceChannel::COLLIDING | Engine::RayTraceChannel::NOT_TRANSPARENT;
}

Object::~Object()
{
	if( collisionShape )
	{
		engine->GetCollisionShapeManager()->RemoveShape( collisionShape );
	}
	
	if( body )
	{
		if( body->getMotionState() )
		{
			assert( body->getMotionState() != NULL );
			delete body->getMotionState();
			body->setMotionState( NULL );
		}
		
		body->setCollisionShape( NULL );
		
		assert( body );
		body.Delete();
	}
	
	name = "";
	scale = btVector3(0,0,0);
	boundingSphereRadius = 0.0f;
	mass = 0.0f;
}

#endif

