
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Engine.h"

template < class T >
std::shared_ptr<Object> Engine::AddObject( std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform, bool dynamic, btScalar mass, btVector3 inertia )
{
	DEBUG(0)
	if( shape && object.find(name) == object.end() )
	{
	DEBUG(1)
		if( dynamic && mass > 0 )
			shape->calculateLocalInertia( mass, inertia );
		else
			mass = 0;
		
	DEBUG(2)
		btDefaultMotionState* motionState = new btDefaultMotionState( transform );
		std::shared_ptr<btRigidBody> rigidBody( new btRigidBody( mass, motionState, (btCollisionShape*)shape.get(), inertia ) );
		world->AddBody( name, rigidBody );
		rigidBody->setDamping( 0.2, 0.1 );
		rigidBody->setFriction( 0.2 );
		
	DEBUG(3)
		std::shared_ptr<Object> obj( new T( this, name, rigidBody, shape, mass ) );
		object[name] = obj;
		
	DEBUG(4)
		rigidBody->setUserPointer( (void*)obj.get() );
		
	DEBUG(5)
		return obj;
	}
	DEBUG(6)
	
	if( !shape )
	{
		DEBUG( std::string("Shape = NULL : ") + name );
	}
	
	DEBUG(7)
	
	if( object.find(name) != object.end() )
	{
		DEBUG( ( std::string( "Trying to spawn object whit name that exist: " ) + name ) );
	}
	
	DEBUG(8)
	return std::shared_ptr<Object>();
}

template < class T >
std::shared_ptr<Object> Engine::AddCharacter( std::string name, btScalar width, btScalar height, btTransform transform, btScalar mass )
{
	if( object.find(name) == object.end() )
	{
		std::string shapeName = collisionShapeManager->GetFirstAvailableName( name );
		std::shared_ptr<btCollisionShape> shape = collisionShapeManager->GetCapsule( width/2.0, height, shapeName );
		std::shared_ptr<Object> obj = AddObject<T>( name, shape, transform, true, mass, btVector3(0,0,0) );
		if( obj )
		{
			obj->GetBody()->setAngularFactor( btVector3( 0, 0, 0 ) );
			obj->GetBody()->setActivationState( DISABLE_DEACTIVATION );
			obj->GetBody()->setDamping( 0.8, 0.0 );
			//obj->GetBody()->setGravity( world->GetGravity() * 1.7 );
			obj->GetBody()->setFriction( 0.8 );
		}
		else
		{
			collisionShapeManager->RemoveShape( shape );
		}
		return obj;
	}
	return std::shared_ptr<Object>();
}

#endif

