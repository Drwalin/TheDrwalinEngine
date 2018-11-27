
//	This file is part of The Drwalin Game project
// Copyright (C) 2018 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CHARACTER_WALK_TRIGGER_CPP
#define CHARACTER_WALK_TRIGGER_CPP

#include "CharacterWalkTrigger.h"

bool CharacterWalkTrigger::IsAnyInside() const
{
	return isAnyInside;
}

void CharacterWalkTrigger::SetParent( std::shared_ptr<Object> parent )
{
	this->parent = parent;
}

void CharacterWalkTrigger::EventOverlapp( Object * other )
{
	if( other != parent.get() )
	{
		isAnyInside = true;
	}
}

void CharacterWalkTrigger::EventOnObjectBeginOverlapp( Object * other )
{
	Trigger::EventOnObjectBeginOverlapp( other );
	this->EventOverlapp( other );
}

void CharacterWalkTrigger::EventOnObjectTickOverlapp( Object * other )
{
	Trigger::EventOnObjectTickOverlapp( other );
	this->EventOverlapp( other );
}

void CharacterWalkTrigger::EventOnObjectEndOverlapp( Object * other )
{
	Trigger::EventOnObjectEndOverlapp( other );
	this->EventOverlapp( other );
}

void CharacterWalkTrigger::Tick( const float deltaTime )
{
	isAnyInside = false;
	
	Trigger::Tick( deltaTime );
}

CharacterWalkTrigger::CharacterWalkTrigger( Engine * engine, std::string name, std::shared_ptr<btPairCachingGhostObject> body, std::shared_ptr<btCollisionShape> collisionShape ) :
	Trigger( engine, name, body, collisionShape )
{
	isAnyInside = false;
}

CharacterWalkTrigger::CharacterWalkTrigger()
{
	isAnyInside = false;
}

CharacterWalkTrigger::~CharacterWalkTrigger()
{
	isAnyInside = false;
}

#endif

