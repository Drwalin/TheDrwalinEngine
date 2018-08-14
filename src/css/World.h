
#ifndef WORLD_H
#define WORLD_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>

#include <cstdio>

#include <map>
#include <string>

class World
{
private:
	
	btBroadphaseInterface * broadphase;
	btDefaultCollisionConfiguration * collisionConfiguration;
	btCollisionDispatcher * dispatcher;
	btSequentialImpulseConstraintSolver * solver;
	btDiscreteDynamicsWorld * dynamicsWorld;
	
public:
	
	std::map < std::string, btRigidBody* > object;
	
	
	void Tick( btScalar deltaTime, int count = 0 );
	void Draw();
	
	
	void DeleteObject( std::string name );
	void DeleteObjects();
	
	bool AddObject( std::string name, btCollisionShape * shape, btTransform transofrm, bool isDynamic = false, btScalar mass = 0, btVector3 inertia = btVector3(0,0,0) );		// return true if wasn't overrided
	
	void Init();
	void Destroy();
	
	World();
	~World();
};

#endif

