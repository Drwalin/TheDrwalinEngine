
#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Engine.h"

template < class T >
SmartPtr<Object> Engine::AddObject( std::string name, SmartPtr<btCollisionShape> shape, btTransform transform, bool dynamic, btScalar mass, btVector3 inertia )
{
	if( shape && object.find(name) == object.end() )
	{
		if( dynamic && mass > 0 )
			shape->calculateLocalInertia( mass, inertia );
		else
			mass = 0;
		
		btDefaultMotionState* motionState = new btDefaultMotionState( transform );
		SmartPtr<btRigidBody> rigidBody;
		rigidBody = new btRigidBody( mass, motionState, (btCollisionShape*)shape.GetPtr(), inertia );
		world->AddBody( name, rigidBody );
		rigidBody->setDamping( 0.2, 0.1 );
		rigidBody->setFriction( 0.2 );
		
		SmartPtr<Object> obj;
		obj = new T( this, name, rigidBody, shape, mass );
		object[name] = obj;
		
		rigidBody->setUserPointer( (void*)obj.GetPtr() );
		
		return obj;
	}
	return SmartPtr<Object>();
}

template < class T >
SmartPtr<Object> Engine::AddCharacter( std::string name, btScalar width, btScalar height, btTransform transform, btScalar mass )
{
	if( object.find(name) == object.end() )
	{
		std::string shapeName = collisionShapeManager->GetFirstAvailableName( name );
		SmartPtr<btCollisionShape> shape = collisionShapeManager->GetCapsule( width/2.0, height, shapeName );
		SmartPtr<Object> obj = AddObject<T>( name, shape, transform, true, mass, btVector3(0,0,0) );
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
			collisionShapeManager->RemoveCustomShape( shapeName );
		}
		return obj;
	}
	return SmartPtr<Object>();
}

#endif

