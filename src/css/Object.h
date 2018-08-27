
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
private:
	
	Engine * engine;
	
	std::string name;
	
	SmartPtr<btCollisionShape> collisionShape;
	SmartPtr<btRigidBody> body;
	SmartPtr<Model> model;
	btVector3 scale;
	float boundingSphereRadius;
	
	int rayTraceChannel;
	
public:
	
	Engine * GetEngine();
	
	std::string GetName() const;
	
	void SetRayTraceChannel( int src );
	int GetRayTraceChannel();
	
	float GetRadius();
	void CalculateRadius();
	
	void SetScale( btVector3 scale );
	btVector3 GetScale();
	
	btTransform GetTransform();
	btVector3 GetLocation();
	
	SmartPtr<btRigidBody> GetBody();
	
	void Draw();
	
	void SetModel( SmartPtr<Model> model );
	
	Object( Engine * engine, std::string name, SmartPtr<btRigidBody> body, SmartPtr<btCollisionShape> collisionShape );
	Object();
	~Object();
};

#endif

