
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TRIGGER_CPP
#define TRIGGER_CPP

#include <Trigger.h>
#include <Engine.h>

#include <Math.hpp>

#include <cassert>

void Trigger::SetPosition( const btVector3 & loc )
{
	currentTransform = btTransform( currentTransform.getRotation(), loc );
	
	if( body )
	{
		body->setWorldTransform( currentTransform );
		engine->GetWorld()->UpdateColliderForTrigger( body );
	}
}

void Trigger::SetRotation( const btQuaternion & quat )
{
	currentTransform = btTransform( quat, currentTransform.getOrigin() );
	
	if( body )
	{
		body->setWorldTransform( currentTransform );
		engine->GetWorld()->UpdateColliderForTrigger( body );
	}
}

void Trigger::Move( const btVector3 & move )
{
	SetPosition( currentTransform.getOrigin() + move );
}

void Trigger::Rotate( const btQuaternion & quat )
{
	SetRotation( currentTransform.getRotation() * quat );
}

void Trigger::NextOverlappingFrame()
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

void Trigger::OverlapWithObject( Object * other, btPersistentManifold * perisstentManifold )
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
			MESSAGE( "other = NULL" );
		}
	}
	else
	{
		MESSAGE( "perisstentManifold = NULL" );
	}
}

void Trigger::EventOnObjectBeginOverlapp( Object * other, btPersistentManifold * perisstentManifold )
{
}

void Trigger::EventOnObjectTickOverlapp( Object * other, btPersistentManifold * perisstentManifold )
{
}

void Trigger::EventOnObjectEndOverlapp( Object * other )
{
}

std::shared_ptr<Trigger> Trigger::GetThis()
{
	if( !thisPtr )
	{
		if( engine )
		{
			thisPtr = engine->GetTrigger( std::string(name) );
		}
		else
		{
			DEBUG("Error creating std::shared_ptr Object::thisPtr ");
		}
	}
	return thisPtr;
}



void Trigger::Tick( const float deltaTime )
{
	// proceed overlaps
	
}

Engine * Trigger::GetEngine()
{
	return engine;
}

void Trigger::SetScale( btVector3 scale )
{
	this->scale = scale;
	if( body )
	{
		body->activate( true );
		body->getCollisionShape()->setLocalScaling( scale );
		engine->GetWorld()->UpdateColliderForTrigger( body );
		body->activate( true );
	}
}

btVector3 Trigger::GetScale()
{
	return scale;
}

btTransform Trigger::GetTransform()
{
	if( body )
		return body->getWorldTransform();
	return btTransform();
}

std::shared_ptr<btPairCachingGhostObject> Trigger::GetBody()
{
	return body;
}

btVector3 Trigger::GetLocation() const
{
	if( body )
	{
		return body->getWorldTransform().getOrigin();
	}
	return btVector3();
}

std::string Trigger::GetName() const
{
	return name;
}

Trigger::Trigger( Engine * engine, std::string name, std::shared_ptr<btPairCachingGhostObject> body, std::shared_ptr<btCollisionShape> collisionShape )
{
	this->collisionShape = collisionShape;
	this->engine = engine;
	this->name = name;
	this->body = body;
	scale = btVector3(1,1,1);
}

Trigger::Trigger()
{
	engine = NULL;
	name = "";
	scale = btVector3(1,1,1);
}

Trigger::~Trigger()
{
	if( collisionShape )
	{
		engine->GetCollisionShapeManager()->RemoveShape( collisionShape );
	}
	
	if( body )
	{
		body->setCollisionShape( NULL );
		
		assert( body );
		body.reset();
	}
	
	name = "";
	scale = btVector3(0,0,0);
}

#endif

