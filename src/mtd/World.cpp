
#ifndef WORLD_CPP
#define WORLD_CPP

#include <World.h>
#include <Debug.h>

void World::ActivateAll()
{
	if( activateAll <= 0 )
		activateAll = 1;
	else if( activateAll < 2 )
		++activateAll;
	else if( activateAll > 2 )
		activateAll = 2;
}

btDiscreteDynamicsWorld * World::DynamicsWorld()
{
	return dynamicsWorld;
}

void World::Tick( btScalar deltaTime, int count )
{
	if( activateAll > 0 )
	{
		auto it = object.find( currentActivator );
		if( it == object.end() )
			it = object.begin();
		
		for( int i = 0; i < 113; ++i, ++it )
		{
			if( it == object.end() )
			{
				--activateAll;
				break;
			}
			it->second->activate( true );
		}
		
		if( it != object.end() )
		{
			currentActivator = it->first;
		}
		else
		{
			currentActivator = "";
		}
	}
	
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
			return false;
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
			ActivateAll();
			it->second->activate( true );
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

World::World() :
	activateAll(0)
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

