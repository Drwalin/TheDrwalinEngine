
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

#include <Debug.h>

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
	
	std::map < std::string, Texture* > texture;
	std::map < std::string, Model* > model;
	std::map < std::string, Object* > object;
	
	float guiDrawTime, sceneDrawTime, physicsSimulationTime;
	
	Object * cameraParent;
	
	bool pausePhysics;
	
public:
	
	class RayTraceData
	{
	public:
		float distance;
		btVector3 begin, end;
		btVector3 point, normal;
		Object * object;
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
	
	Object * RayTrace( btVector3 begin, btVector3 end, int channel, btVector3 & point, btVector3 & normal, const std::vector < Object* > & ignoreObjects );
	
	World * GetWorld();
	Window * GetWindow();
	
	void PauseSimulation();
	void ResumeSimulation();
	
	void DrawBox( ALLEGRO_COLOR color, btTransform transform, btVector3 size );
	
	Camera * GetCamera();
	
	CollisionShapeManager * GetCollisionShapeManager();
	
	Object * AddObject( std::string name, btCollisionShape * shape, btTransform transform, bool dynamic = false, btScalar mass = 1.0f, btVector3 inertia = btVector3(0,0,0) );
	Object * AddCharacter( std::string name, btScalar width, btScalar height, btTransform transform, btScalar mass );
	
	void AttachCameraToObject( std::string name, btVector3 location );
	bool SetCustomModelName( std::string name, Model * mdl );
	
	Model * LoadModel( std::string name );
	Texture * GetTexture( std::string name );
	Model * GetModel( std::string name );
	Object * GetObject( std::string name );
	
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

#endif

