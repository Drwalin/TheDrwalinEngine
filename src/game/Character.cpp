
#ifndef CHARACTER_CPP
#define CHARACTER_CPP

#include "Character.h"

#include <Debug.h>
#include <Math.hpp>

#include <ctime>

void Character::NextOverlappingFrame()
{
	Object::NextOverlappingFrame();
	if( isInAir )
		lastTimeInAir = float(clock())/1000.0f;
	isInAir = true;
	body->setDamping( 0.1, 0.0 );
	body->setFriction( 0.0 );
}

float Character::GetBottomY() const
{
	if( body )
	{
		btVector3 min, max, origin;
		body->getAabb( min, max );
		return (min.y()*0.85) + (GetLocation().y()*0.15);
	}
	return currentTransform.getOrigin().y();
}

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
		velocity *= 0.08f;
	
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

btVector3 Character::GetJumpVelocity() const
{
	if( body )
	{
		btVector3 vel = body->getLinearVelocity();
		vel.m_floats[1] = 0;
		
		
		return ( btVector3( 0, sqrt( ( ( /*((btRigidBody*)*/body/*.GetPtr())*/->getGravity().length() ) * jumpHeight * 0.5f ) + ( vel.length() * 0.311 ) ), 0 ) * 35.0 ) + ( vel * 0.1 );
	}
	DEBUG( "Shouldn't appear" );
	return btVector3(0,2.5f,0);
}



void Character::CorrectCameraRotation()
{
	if( cameraRotation.m_floats[0] < -Math::PI*0.5f )
		cameraRotation.m_floats[0] = -Math::PI*0.5f;
	else if( cameraRotation.m_floats[0] > Math::PI*0.5f )
		cameraRotation.m_floats[0] = Math::PI*0.5f;
	
	while( cameraRotation.m_floats[1] >= Math::PI * 2.0f )
		cameraRotation.m_floats[1] -= Math::PI * 2.0f;
	while( cameraRotation.m_floats[1] < 0.0f )
		cameraRotation.m_floats[1] += Math::PI * 2.0f;
}

void Character::SetCamera( std::shared_ptr<Camera> camera )
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




Character::Character( Engine * engine, std::string name, std::shared_ptr<btRigidBody> body, std::shared_ptr<btCollisionShape> collisionShape, float mass_ ) :
	Object( engine, name, body, collisionShape, mass_ ),
	cameraRotation(0,0,0), cameraLocation(0,0,0),
	defaultVelocity(4.5), jumpHeight(1.1), height(1.75),
	walkMode(Character::WalkMode::WALK), previousWalkMode(Character::WalkMode::WALK),
	isInAir(false),
	lastTimeInAir(0.0)
{
	SetCameraLocation( btVector3( 0.0, height * 0.5 * 0.9, 0.0 ) );
}

Character::Character() :
	Object(), cameraRotation(0,0,0), cameraLocation(0,0,0),
	defaultVelocity(4.5), jumpHeight(1.1), height(1.75),
	walkMode(Character::WalkMode::WALK), previousWalkMode(Character::WalkMode::WALK),
	isInAir(false),
	lastTimeInAir(0.0)
{
	SetCameraLocation( btVector3( 0.0, height * 0.5 * 0.9, 0.0 ) );
}

Character::~Character()
{
}

#endif

