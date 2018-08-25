
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

#include <Model.h>
#include <Camera.h>

class Engine;

class Object
{
private:
	
	Engine * engine;
	
	std::string name;
	
	btRigidBody * body;
	Model * model;
	btVector3 scale;
	float boundingSphereRadius;
	
	int rayTraceChannel;
	
public:
	
	std::string GetName() const;
	
	void SetRayTraceChannel( int src );
	int GetRayTraceChannel();
	
	float GetRadius();
	void CalculateRadius();
	
	void SetScale( btVector3 scale );
	btVector3 GetScale();
	
	btTransform GetTransform();
	btVector3 GetLocation();
	
	btRigidBody * GetBody();
	void SetBody( btRigidBody * body );
	
	void Draw();
	
	void SetModel( Model * model );
	
	Object( Engine * engine, std::string name, btRigidBody * body );
	Object();
	~Object();
};

#endif

