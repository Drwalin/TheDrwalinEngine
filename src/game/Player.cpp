
//	This file is part of The Drwalin Game project
// Copyright (C) 2018 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef PLAYER_CPP
#define PLAYER_CPP

#include "Player.h"

#include <Debug.h>

void Player::Tick( const float deltaTime )
{
	Character::Tick( deltaTime );
}

Player::Player( Engine * engine, std::string name, std::shared_ptr<btRigidBody> body, std::shared_ptr<btCollisionShape> collisionShape, float mass_ ) :
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

