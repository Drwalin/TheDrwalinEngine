
#ifndef OBJECT_CPP
#define OBJECT_CPP

#include <Object.h>
#include <Engine.h>

#include <Math.hpp>

#include <cassert>

void Object::UpdateTransformSceneNode()
{
	if( sceneNode )
	{
		sceneNode->setPosition( Math::GetIrrVec( currentTransform ) * irr::core::vector3d<float>(-1,1,1) );
		
		irr::core::vector3d<float> eulerRadians; 
		Math::GetIrrQuaternion( currentTransform ).toEuler( eulerRadians ); 
		sceneNode->setRotation( eulerRadians * irr::core::RADTODEG );
	}
}

void Object::SetPosition( const btVector3 & loc )
{
	currentTransform = btTransform( currentTransform.getRotation(), loc );
	previousTransform = currentTransform;
	
	if( body )
	{
		body->activate( true );
		//body->setCenterOfMassTransform( currentTransform );
		//body->getMotionState()->setWorldTransform( currentTransform );
		engine->GetWorld()->UpdateColliderForObject( body );
		body->activate( true );
	}
	
//	UpdateTransformSceneNode();
}

void Object::SetRotation( const btQuaternion & quat )
{
	currentTransform = btTransform( quat, currentTransform.getOrigin() );
	previousTransform = currentTransform;
	
	if( body )
	{
		body->activate( true );
		//body->setCenterOfMassTransform( currentTransform );
		//body->getMotionState()->setWorldTransform( currentTransform );
		engine->GetWorld()->UpdateColliderForObject( body );
		body->activate( true );
	}
	
//	UpdateTransformSceneNode();
}

void Object::Move( const btVector3 & move )
{
	SetPosition( currentTransform.getOrigin() + move );
}

void Object::Rotate( const btQuaternion & quat )
{
	SetRotation( currentTransform.getRotation() * quat );
}

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
	
	UpdateTransformSceneNode();
	
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
		body->activate( true );
	}
	if( sceneNode )
	{
		sceneNode->setScale( Math::GetIrrVec( scale ) );
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

void Object::SetModel( SmartPtr<Model> model )
{
	if( engine )
	{
		if( engine->GetWindow() )
		{
			if( sceneNode )
			{
				engine->GetWindow()->sceneManager->addToDeletionQueue( sceneNode );
				sceneNode = NULL;
			}
			
			this->model = NULL;
			
			if( model )
			{
				if( model->GetMesh() )
				{
					this->model = model;
					sceneNode = engine->GetWindow()->sceneManager->addAnimatedMeshSceneNode( model->GetMesh() );
					model->SetMaterialsToNode( sceneNode );
					
					sceneNode->setScale( Math::GetIrrVec( scale ) );
				}
			}
		}
	}
}

Object::Object( Engine * engine, std::string name, SmartPtr<btRigidBody> body, SmartPtr<btCollisionShape> collisionShape, float mass_ ) :
	mass(mass_)
{
	this->collisionShape = collisionShape;
	this->engine = engine;
	this->name = name;
	this->body = body;
	scale = btVector3(1,1,1);
	rayTraceChannel = Engine::RayTraceChannel::COLLIDING | Engine::RayTraceChannel::NOT_TRANSPARENT;
	sceneNode = NULL;
}

Object::Object() :
	mass(1.0)
{
	engine = NULL;
	name = "";
	scale = btVector3(1,1,1);
	rayTraceChannel = Engine::RayTraceChannel::COLLIDING | Engine::RayTraceChannel::NOT_TRANSPARENT;
	sceneNode = NULL;
}

Object::~Object()
{
	if( sceneNode )
	{
		if( engine )
		{
			if( engine->GetWindow() )
			{
				if( engine->GetWindow()->sceneManager )
				{
					engine->GetWindow()->sceneManager->addToDeletionQueue( sceneNode );
				}
			}
		}
		sceneNode = NULL;
	}
	
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
	mass = 0.0f;
}

#endif

