
#ifndef CHARACTER_CPP
#define CHARACTER_CPP

#include "Character.h"

#include <Debug.h>


btTransform Character::MakeTransformFromEuler( const btVector3 & euler )
{
	btQuaternion quat( btVector3( 0, 1, 0 ), ALLEGRO_PI-euler.y() );
	quat *= btQuaternion( btVector3( 1, 0, 0 ), euler.x() );
	quat *= btQuaternion( btVector3( 0, 0, 1 ), euler.z() );
	return btTransform( quat );
}


float Character::GetMovementVelocity() const
{
	float velocity = defaultVelocity;
	
	if( isInAir )
		velocity *= 0.311f;
	
	switch( walkMode )
	{
	case Character::WalkMode::CROUCH:
		velocity *= 0.5f;
		break;
	case Character::WalkMode::STRAVAGE:
		velocity *= 0.4f;
		break;
	case Character::WalkMode::WALK:
		velocity *= 1.0f;
		break;
	case Character::WalkMode::RUN:
		velocity *= 1.5f;
		break;
	}
	
	return velocity;
}

float Character::GetJumpVelocity() const
{
	if( body )
		return sqrt( ( ((btRigidBody*)body.GetPtr())->getGravity().length() ) * jumpHeight * 0.5f ) * ( walkMode == Character::WalkMode::RUN ? 1.1f : 1.0f ) * 35.0;
	DEBUG( "Shouldn't appear" );
	return 2.5f;
}



void Character::EventJump()
{
	//if( !isInAir )
	{
		if( body )
		{
			body->applyCentralImpulse( btVector3( 0, GetJumpVelocity(), 0 ) );
		}
	}
}

void Character::EventCrouch()
{
	if( walkMode != Character::WalkMode::CROUCH )
	{
		previousWalkMode = walkMode;
		walkMode = Character::WalkMode::CROUCH;
		SetScale( btVector3( 1.0, 0.5, 1.0 ) );
		SetCameraLocation( btVector3( 0.0, height * 0.5 * 0.5 * 0.9, 0.0 ) );
	}
}

void Character::EventStandUp()
{
	if( walkMode == Character::WalkMode::CROUCH )
	{
		walkMode = previousWalkMode;
		walkMode = Character::WalkMode::WALK;
		SetScale( btVector3( 1.0, 1.0, 1.0 ) );
		SetCameraLocation( btVector3( 0.0, height * 0.5 * 0.9, 0.0 ) );
	}
}

void Character::EventBeginRun()
{
	previousWalkMode = walkMode;
	walkMode = Character::WalkMode::RUN;
}

void Character::EventStopRun()
{
	walkMode = previousWalkMode;
}

void Character::EventBeginStravage()
{
	previousWalkMode = walkMode;
	walkMode = Character::WalkMode::STRAVAGE;
}

void Character::EventStopStravage()
{
	walkMode = previousWalkMode;
}

void Character::EventMoveInDirection( const btVector3 & direction, bool flat )
{
	btVector3 dir = direction.normalized();
	
	if( flat )
		dir.m_floats[1] = 0.0f;
	
	if( body )
	{
		float velocity = GetMovementVelocity();
		if( body->getLinearVelocity().dot( dir ) < velocity )
		{
			body->applyCentralForce( dir * 60.0f * mass );
		}
	}
}

void Character::EventRotateCameraBy( const btVector3 & rotation )
{
	cameraRotation += rotation;
	
	if( cameraRotation.m_floats[0] < -ALLEGRO_PI*0.5f )
		cameraRotation.m_floats[0] = -ALLEGRO_PI*0.5f;
	else if( cameraRotation.m_floats[0] > ALLEGRO_PI*0.5f )
		cameraRotation.m_floats[0] = ALLEGRO_PI*0.5f;
	
	if( camera )
	{
		camera->SetRotation( cameraRotation );
	}
}

void Character::EventRotateCameraToLookAtPoint( const btVector3 & worldPoint, bool smooth )
{
	btVector3 dstCameraRotation(0,0,0);
	{
		btVector3 lookingDirection = worldPoint - GetCameraLocation();
		btVector3 flat = dstCameraRotation;
		flat.m_floats[1] = 0.0f;
		
		dstCameraRotation.m_floats[0] = asin( flat.dot( lookingDirection ) ) * ( lookingDirection.y() < 0.0f ? -1.0f : 1.0f );
		dstCameraRotation.m_floats[1] = asin( btVector3(0,0,1).dot( flat ) ) * ( lookingDirection.x() < 0.0f ? -1.0f : 1.0f );
	}
	
	if( smooth )
	{
		cameraRotation = ( cameraRotation + cameraRotation + dstCameraRotation ) / 3.0;
	}
	else
	{
		cameraRotation = dstCameraRotation;
	}
	
	if( cameraRotation.m_floats[0] < -ALLEGRO_PI*0.5f )
		cameraRotation.m_floats[0] = -ALLEGRO_PI*0.5f;
	else if( cameraRotation.m_floats[0] > ALLEGRO_PI*0.5f )
		cameraRotation.m_floats[0] = ALLEGRO_PI*0.5f;
	
	if( camera )
	{
		camera->SetRotation( cameraRotation );
	}
}



void Character::SetCamera( SmartPtr<Camera> camera )
{
	this->camera = camera;
}

void Character::SetCameraLocation( const btVector3 & location )
{
	cameraLocation = location;
	
	if( camera )
	{
		camera->SetPos( cameraLocation );
	}
}

void Character::SetCameraRotation( const btVector3 & rotation )
{
	cameraRotation = rotation;
	
	if( cameraRotation.m_floats[0] < -ALLEGRO_PI*0.5f )
		cameraRotation.m_floats[0] = -ALLEGRO_PI*0.5f;
	else if( cameraRotation.m_floats[0] > ALLEGRO_PI*0.5f )
		cameraRotation.m_floats[0] = ALLEGRO_PI*0.5f;
	
	if( camera )
	{
		camera->SetRotation( cameraRotation );
	}
}

btVector3 Character::GetCameraLocation() const
{
	return GetLocation() + cameraLocation;
}

btVector3 Character::GetForwardVector() const
{
	return Character::MakeTransformFromEuler( btVector3( cameraRotation.x(), cameraRotation.y(), 0.0 ) ) * btVector3(0.0,0.0,1.0);
}

btVector3 Character::GetFlatForwardVector() const
{
	return Character::MakeTransformFromEuler( btVector3( 0.0, cameraRotation.y(), 0.0 ) ) * btVector3(0.0,0.0,1.0);
}

btVector3 Character::GetLeftVector() const
{
	return Character::MakeTransformFromEuler( btVector3( 0.0, cameraRotation.y(), cameraRotation.z() ) ) * btVector3(1.0,0.0,0.0);
}

btVector3 Character::GetFlatLeftVector() const
{
	return Character::MakeTransformFromEuler( btVector3( 0.0, cameraRotation.y(), 0.0 ) ) * btVector3(1.0,0.0,0.0);
}



void Character::Tick( const float deltaTime )
{
	Object::Tick( deltaTime );
}

void Character::ApplyDamage( const float damage, btVector3 point, btVector3 normal )
{
	
	
	
}

void Character::ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal )
{
	Object::ApplyImpactDamage( damage, impetus, direction, point, normal );
	
	
	
}




void Character::Draw()
{
	Object::Draw();
}

Character::Character( Engine * engine, std::string name, SmartPtr<btRigidBody> body, SmartPtr<btCollisionShape> collisionShape, float mass_ ) :
	Object( engine, name, body, collisionShape, mass_ ),
	cameraRotation(0,0,0), cameraLocation(0,0,0),
	defaultVelocity(5.0), jumpHeight(1.1), height(1.75),
	walkMode(Character::WalkMode::WALK), previousWalkMode(Character::WalkMode::WALK)
{
	SetCameraLocation( btVector3( 0.0, height * 0.5 * 0.9, 0.0 ) );
}

Character::Character() :
	Object(), cameraRotation(0,0,0), cameraLocation(0,0,0),
	defaultVelocity(5.0), jumpHeight(1.1), height(1.75),
	walkMode(Character::WalkMode::WALK), previousWalkMode(Character::WalkMode::WALK)
{
	SetCameraLocation( btVector3( 0.0, height * 0.5 * 0.9, 0.0 ) );
}

Character::~Character()
{
}

#endif

