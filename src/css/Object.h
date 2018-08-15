
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
	
	std::vector < float > debugData;
	int objectType;
	
public:
	
	const static int BOX = 1;
	const static int BALL = 2;
	const static int CAPSULE = 3;
	const static int CYLINDER = 4;
	const static int CUSTOM = 5;
	
	btTransform GetTransform();
	
	void Draw();
	void DrawDebug();
	
	void SetModel( Model * model );
	
	Object( Engine * engine, std::string name, btRigidBody * body, std::vector < float > debugData, int type );
	Object();
	~Object();
};

#endif

