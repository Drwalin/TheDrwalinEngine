
#ifndef OBJECT_H
#define OBJECT_H

#include <irrlicht\irrlicht.h>

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>

#include <string>
#include <set>

#include <Debug.h>
#include <SmartPtr.h>

#include <Model.h>

class Engine;

class Object
{
protected:
	
	Engine * engine;
	
	std::string name;
	
	btTransform currentTransform;
	btTransform previousTransform;
	
	SmartPtr<Model> model;
	irr::scene::IAnimatedMeshSceneNode *sceneNode;
	SmartPtr<btCollisionShape> collisionShape;
	SmartPtr<btRigidBody> body;
	
	btVector3 scale;
	
	int rayTraceChannel;
	
	float mass;
	
	std::set< Object* > overlappingInPreviousFrame;
	std::set< Object* > overlappingInCurrentFrame;
	
	SmartPtr<Object> thisPtr;
	
	void UpdateTransformSceneNode();
	
public:
	
	virtual void NextOverlappingFrame();
	void OverlapWithObject( Object * other, btPersistentManifold * perisstentManifold );
	
	void SetPosition( const btVector3 & loc );
	void SetRotation( const btQuaternion & quat );
	void Move( const btVector3 & move );
	void Rotate( const btQuaternion & quat );
	
	SmartPtr<Object> GetThis();
	
	
	void SetMass( float mass );
	
	bool IsDynamic() const;
	
	Engine * GetEngine();
	
	std::string GetName() const;
	
	void SetRayTraceChannel( int src );
	int GetRayTraceChannel();
	
	void SetScale( btVector3 scale );
	btVector3 GetScale();
	
	btTransform GetTransform();
	btVector3 GetLocation() const;
	
	SmartPtr<btRigidBody> GetBody();
	
	virtual void EventOnObjectBeginOverlapp( Object * other, btPersistentManifold * perisstentManifold );
	virtual void EventOnObjectTickOverlapp( Object * other, btPersistentManifold * perisstentManifold );
	virtual void EventOnObjectEndOverlapp( Object * other );
	
	virtual void Tick( const float deltaTime );
	virtual void ApplyDamage( const float damage, btVector3 point, btVector3 normal );
	virtual void ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal );
	
	void SetModel( SmartPtr<Model> model );
	
	Object( Engine * engine, std::string name, SmartPtr<btRigidBody> body, SmartPtr<btCollisionShape> collisionShape, float mass_ );
	Object();
	~Object();
};

#endif

