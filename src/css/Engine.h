
#ifndef ENGINE_H
#define ENGINE_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>

#include <set>
#include <map>
#include <vector>
#include <string>
#include <queue>

#include <Debug.h>
#include <SmartPtr.h>

#include <Event.h>
#include <World.h>
#include <Window.h>
#include <Texture.h>
#include <Model.h>
#include <Object.h>

#include <CollisionShapeManager.h>

class Engine
{
private:
	
	World * world;
	Window * window;
	Event * event;
	CollisionShapeManager * collisionShapeManager;
	
	std::map < std::string, SmartPtr<Texture> > texture;
	std::map < std::string, SmartPtr<Model> > model;
	std::map < std::string, SmartPtr<Object> > object;
	
	std::queue < std::string > objectsQueuedToDestroy;
	
	float guiDrawTime, sceneDrawTime, physicsSimulationTime;
	
	SmartPtr<Object> cameraParent;
	
	bool pausePhysics;
	
	void UpdateObjects( const float deltaTime );
	
public:
	
	class RayTraceData
	{
	public:
		float distance;
		btVector3 begin, end;
		btVector3 point, normal;
		SmartPtr<Object> object;
		bool operator < ( const RayTraceData & other ) const;
		RayTraceData( btCollisionWorld::AllHitsRayResultCallback & hitData, unsigned id );
		RayTraceData();
	};
	
	enum RayTraceChannel
	{
		NONE = 0,
		COLLIDING = 1,
		NOT_TRANSPARENT = 2
	};
	
	void DrawCrosshair();
	
public:
	
	void QueueObjectToDestroy( SmartPtr<Object> ptr );
	void QueueObjectToDestroy( const std::string & name );
	
	SmartPtr<Object> RayTrace( btVector3 begin, btVector3 end, int channel, btVector3 & point, btVector3 & normal, const std::vector < SmartPtr<Object> > & ignoreObjects );
	
	float GetDeltaTime();
	
	World * GetWorld();
	Window * GetWindow();
	
	void PauseSimulation();
	void ResumeSimulation();
	
	void DrawBox( ALLEGRO_COLOR color, btTransform transform, btVector3 size );
	
	SmartPtr<Camera> GetCamera() const;
	SmartPtr<Object> GetCameraParent() const;
	
	CollisionShapeManager * GetCollisionShapeManager();
	
	template < class T >
	SmartPtr<Object> AddObject( std::string name, SmartPtr<btCollisionShape> shape, btTransform transform, bool dynamic = false, btScalar mass = 1.0f, btVector3 inertia = btVector3(0,0,0) );
	template < class T >
	SmartPtr<Object> AddCharacter( std::string name, btScalar width, btScalar height, btTransform transform, btScalar mass );
	
	void AttachCameraToObject( std::string name, btVector3 location );
	bool SetCustomModelName( std::string name, SmartPtr<Model> mdl );
	
	SmartPtr<Model> LoadModel( std::string name );
	SmartPtr<Texture> GetTexture( std::string name );
	SmartPtr<Model> GetModel( std::string name );
	SmartPtr<Object> GetObject( std::string name );
	
	std::string GetAvailableObjectName( std::string name );
	
	void DeleteObject( std::string name );
	
	int CalculateNumberOfSimulationsPerFrame( const float deltaTime );
	void Tick( const float deltaTime );
	void ParallelToDrawTick( const float deltaTime );
	
	void Draw2D();
	void Draw3D();
	
	void Init( const char * windowName, const char * iconFile, int width, int height, bool fullscreen = false );
	
	void BeginLoop();
	
	void Destroy();
	
	Engine();
	~Engine();
};

#include "Engine.hpp"

#endif

