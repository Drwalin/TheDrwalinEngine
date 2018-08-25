
#ifndef WORLD_CPP
#define WORLD_CPP

#include <World.h>
#include <Debug.h>

btDiscreteDynamicsWorld * World::DynamicsWorld()
{
	return dynamicsWorld;
}

void World::Tick( btScalar deltaTime, int count )
{
	if( count > 0 )
		dynamicsWorld->stepSimulation( deltaTime, count );
	else
		dynamicsWorld->stepSimulation( deltaTime );
}

void World::UpdateColliderForObject( btRigidBody * body )
{
	dynamicsWorld->getCollisionWorld()->updateSingleAabb( body );
}

btVector3 World::GetGravity()
{
	return dynamicsWorld->getGravity();
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

bool World::AddBody( std::string name, btRigidBody * body )
{
	if( body )
	{
		if( object.find(name) != object.end() )
		{
			//DeleteObject( name );			////////////////////////////////////////////////////////////////////////
		}
		else
		{
			dynamicsWorld->addRigidBody( body );
			object[name] = body;
			return true;
		}
	}
	return false;
}

void World::RemoveBody( std::string name )
{
	auto it = object.find(name);
	if( it != object.end() )
	{
		if( it->second )
		{
			dynamicsWorld->removeRigidBody( it->second );
		}
		object.erase( it );
	}
}

void World::RemoveBodys()
{
	for( auto it = object.begin(); it != object.end(); ++it )
	{
		if( it->second )
		{
			dynamicsWorld->removeRigidBody( it->second );
		}
	}
	object.clear();
}

void World::Destroy()
{
	RemoveBodys();
	
	if( dynamicsWorld )
		delete dynamicsWorld;
	dynamicsWorld = NULL;
	if( solver )
		delete solver;
	solver = NULL;
	if( collisionConfiguration )
		delete collisionConfiguration;
	collisionConfiguration = NULL;
	if( dispatcher )
		delete dispatcher;
	dispatcher = NULL;
	if( broadphase )
		delete broadphase;
	broadphase = NULL;
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

