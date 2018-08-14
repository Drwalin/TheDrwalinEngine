
#ifndef WORLD_CPP
#define WORLD_CPP

#include <World.h>

void World::Tick( btScalar deltaTime, int count )
{
	if( count > 0 )
		dynamicsWorld->stepSimulation( deltaTime, count );
	else
		dynamicsWorld->stepSimulation( deltaTime );
}

void World::Init()
{
	Destroy();
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher( collisionConfiguration );
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld( dispatcher, broadphase, solver, collisionConfiguration );
	dynamicsWorld->setGravity( btVector3(0, -9.81, 0) );
}

bool World::AddObject( std::string name, btCollisionShape * shape, btTransform transofrm, bool isDynamic, btScalar mass, btVector3 inertia )
{
	bool ret = true;
	if( object.find(name) != object.end() )
	{
		DeleteObject( name );
		ret = false;
	}
	
	if( isDynamic && mass > 0 )
	{
		shape->calculateLocalInertia( mass, inertia );
	}
	else
	{
		mass = 0;
	}
	
	btDefaultMotionState* motionState = new btDefaultMotionState( transofrm );
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI( mass, motionState, shape, inertia );
	btRigidBody* rigidBody = new btRigidBody( rigidBodyCI );
	dynamicsWorld->addRigidBody( rigidBody );
	object[name] = rigidBody;
	
	return ret;
}

void World::DeleteObject( std::string name )
{
	auto it = object.find(name);
	if( it != object.end() )
	{
		if( it->second )
		{
			dynamicsWorld->removeRigidBody( it->second );
			if( it->second->getMotionState() )
				delete it->second->getMotionState();
			if( it->second->getCollisionShape() )
				delete it->second->getCollisionShape();
			delete it->second;
		}
		object.erase( it );
	}
}

void World::DeleteObjects()
{
	for( auto it = object.begin(); it != object.end(); ++it )
	{
		if( it->second )
		{
			dynamicsWorld->removeRigidBody( it->second );
			if( it->second->getMotionState() )
				delete it->second->getMotionState();
			if( it->second->getCollisionShape() )
				delete it->second->getCollisionShape();
			delete it->second;
		}
	}
	object.clear();
}

void World::Destroy()
{
	DeleteObjects();
	
	if( dynamicsWorld )
		delete dynamicsWorld;
	if( solver )
		delete solver;
	if( collisionConfiguration )
		delete collisionConfiguration;
	if( dispatcher )
		delete dispatcher;
	if( broadphase )
		delete broadphase;
	broadphase = NULL;
	collisionConfiguration = NULL;
	dispatcher = NULL;
	solver = NULL;
	dynamicsWorld = NULL;
}

World::World()
{
	broadphase = NULL;
	collisionConfiguration = NULL;
	dispatcher = NULL;
	solver = NULL;
	dynamicsWorld = NULL;
}

World::~World()
{
	Destroy();
}




#endif

