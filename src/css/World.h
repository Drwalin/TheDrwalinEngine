
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
	
	std::map < std::string, btRigidBody* > object;
	
	std::string currentActivator;
	int activateAll;
	
public:
	
	void ActivateAll();
	
	btDiscreteDynamicsWorld * DynamicsWorld();
	
	void UpdateColliderForObject( btRigidBody * body );
	
	btVector3 GetGravity();
	
	void Tick( btScalar deltaTime, int count = 0 );
	
	bool AddBody( std::string name, btRigidBody * body );
	
	void RemoveBody( std::string name );
	void RemoveBodys();
	
	void Init();
	void Destroy();
	
	World();
	~World();
};

#endif

