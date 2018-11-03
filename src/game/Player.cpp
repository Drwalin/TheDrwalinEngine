
#ifndef PLAYER_CPP
#define PLAYER_CPP

#include "Player.h"

#include <Debug.h>

void Player::Tick( const float deltaTime )
{
	Character::Tick( deltaTime );
}

Player::Player( Engine * engine, std::string name, SmartPtr<btRigidBody> body, SmartPtr<btCollisionShape> collisionShape, float mass_ ) :
	Character( engine, name, body, collisionShape, mass_ )
{
}

Player::Player() :
	Character()
{
}

Player::~Player()
{
}

#endif

