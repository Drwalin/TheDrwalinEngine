
#ifndef CHARACTER_CPP
#define CHARACTER_CPP

#include "Character.h"

#include <Debug.h>

btTransform Character::MakeTransformFromEuler( const btVector3 & euler )
{
	btQuaternion quat( btVector3( 0, 1, 0 ), -euler.y() );
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
		velocity *= 0.7f;
		break;
	case Character::WalkMode::STRAVAGE:
		velocity *= 0.6f;
		break;
	case Character::WalkMode::WALK:
		velocity *= 1.0f;
		break;
	case Character::WalkMode::RUN:
		velocity *= 1.3f;
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



void Character::CorrectCameraRotation()
{
	if( cameraRotation.m_floats[0] < -ALLEGRO_PI*0.5f )
		cameraRotation.m_floats[0] = -ALLEGRO_PI*0.5f;
	else if( cameraRotation.m_floats[0] > ALLEGRO_PI*0.5f )
		cameraRotation.m_floats[0] = ALLEGRO_PI*0.5f;
	
	while( cameraRotation.m_floats[1] >= ALLEGRO_PI * 2.0f )
		cameraRotation.m_floats[1] -= ALLEGRO_PI * 2.0f;
	while( cameraRotation.m_floats[1] < 0.0f )
		cameraRotation.m_floats[1] += ALLEGRO_PI * 2.0f;
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
	
	CorrectCameraRotation();
	
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
	return ( Character::MakeTransformFromEuler( btVector3( cameraRotation.x(), cameraRotation.y(), 0.0 ) ) * btVector3(0.0,0.0,-1.0) ) * btVector3( 1, -1, 1 );
}

btVector3 Character::GetFlatForwardVector() const
{
	return Character::MakeTransformFromEuler( btVector3( 0.0, cameraRotation.y(), 0.0 ) ) * btVector3(0.0,0.0,-1.0);
}

btVector3 Character::GetLeftVector() const/////////////////////////////
{
	return Character::MakeTransformFromEuler( btVector3( 0.0, cameraRotation.y(), cameraRotation.z() ) ) * btVector3(-1.0,0.0,0.0);
}

btVector3 Character::GetFlatLeftVector() const
{
	return Character::MakeTransformFromEuler( btVector3( 0.0, cameraRotation.y(), 0.0 ) ) * btVector3(-1.0,0.0,0.0);
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
	Character::ApplyDamage( damage, point, normal );
}



void Character::Draw()
{
	Object::Draw();
}

Character::Character( Engine * engine, std::string name, SmartPtr<btRigidBody> body, SmartPtr<btCollisionShape> collisionShape, float mass_ ) :
	Object( engine, name, body, collisionShape, mass_ ),
	cameraRotation(0,0,0), cameraLocation(0,0,0),
	defaultVelocity(4.5), jumpHeight(1.1), height(1.75),
	walkMode(Character::WalkMode::WALK), previousWalkMode(Character::WalkMode::WALK)
{
	SetCameraLocation( btVector3( 0.0, height * 0.5 * 0.9, 0.0 ) );
}

Character::Character() :
	Object(), cameraRotation(0,0,0), cameraLocation(0,0,0),
	defaultVelocity(4.5), jumpHeight(1.1), height(1.75),
	walkMode(Character::WalkMode::WALK), previousWalkMode(Character::WalkMode::WALK)
{
	SetCameraLocation( btVector3( 0.0, height * 0.5 * 0.9, 0.0 ) );
}

Character::~Character()
{
}

#endif

