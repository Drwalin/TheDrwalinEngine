
#ifndef OBJECT_H
#define OBJECT_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>

#include <string>
#include <map>
#include <iostream>

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
	
	SmartPtr<btCollisionShape> collisionShape;
	SmartPtr<btRigidBody> body;
	SmartPtr<Model> model;
	btVector3 scale;
	float boundingSphereRadius;
	
	int rayTraceChannel;
	
	float mass;
	
public:
	
	void SetMass( float mass );
	
	bool IsDynamic();
	
	Engine * GetEngine();
	
	std::string GetName() const;
	
	void SetRayTraceChannel( int src );
	int GetRayTraceChannel();
	
	float GetRadius();
	void CalculateRadius();
	
	void SetScale( btVector3 scale );
	btVector3 GetScale();
	
	btTransform GetTransform();
	btVector3 GetLocation() const;
	
	SmartPtr<btRigidBody> GetBody();
	
	virtual void Tick( const float deltaTime );
	virtual void ApplyDamage( const float damage, btVector3 point, btVector3 normal );
	virtual void ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal );
	
	virtual void Draw();
	
	void SetModel( SmartPtr<Model> model );
	
	Object( Engine * engine, std::string name, SmartPtr<btRigidBody> body, SmartPtr<btCollisionShape> collisionShape, float mass_ );
	Object();
	~Object();
};

#endif

