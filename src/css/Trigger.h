
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TRIGGER_H
#define TRIGGER_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <string>
#include <set>
#include <memory>

#include <Object.h>

#include <Debug.h>

class Engine;

class Trigger
{
protected:
	
	Engine * engine;
	
	std::string name;
	
	btTransform currentTransform;
	
	std::shared_ptr<btCollisionShape> collisionShape;
	std::shared_ptr<btPairCachingGhostObject> body;
	
	btVector3 scale;
	
	float mass;
	
	std::set< Object* > overlappingInPreviousFrame;
	std::set< Object* > overlappingInCurrentFrame;
	
	std::shared_ptr<Trigger> thisPtr;
	
public:
	
	virtual void NextOverlappingFrame();
	void OverlapWithObject( Object * other, btPersistentManifold * perisstentManifold );
	
	void SetPosition( const btVector3 & loc );
	void SetRotation( const btQuaternion & quat );
	void Move( const btVector3 & move );
	void Rotate( const btQuaternion & quat );
	
	std::shared_ptr<Trigger> GetThis();
	
	
	void SetMass( float mass );
	
	Engine * GetEngine();
	
	std::string GetName() const;
	
	void SetScale( btVector3 scale );
	btVector3 GetScale();
	
	btTransform GetTransform();
	btVector3 GetLocation() const;
	
	std::shared_ptr<btPairCachingGhostObject> GetBody();
	
	virtual void EventOnObjectBeginOverlapp( Object * other );
	virtual void EventOnObjectTickOverlapp( Object * other );
	virtual void EventOnObjectEndOverlapp( Object * other );
	
	virtual void Tick( const float deltaTime );
	
	Trigger( Engine * engine, std::string name, std::shared_ptr<btPairCachingGhostObject> body, std::shared_ptr<btCollisionShape> collisionShape );
	Trigger();
	~Trigger();
};

#endif

