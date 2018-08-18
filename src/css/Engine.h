
#ifndef ENGINE_H
#define ENGINE_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>

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

class Engine
{
private:
	
	World * world;
	Window * window;
	Event * event;
	
	std::map < std::string, Texture* > texture;
	std::map < std::string, Model* > model;
	std::map < std::string, Object* > object;
	std::map < std::vector < btScalar >, btCollisionShape* > collisionShape;
	std::vector < btCollisionShape* > customCollisionShape;
	
	float guiDrawTime, sceneDrawTime, physicsSimulationTime;
	
	Object * cameraParent;
	
	bool pausePhysics;
	
public:
	
	World * GetWorld();
	
	void PauseSimulation();
	void ResumeSimulation();
	
	void DrawBox( ALLEGRO_COLOR color, btTransform transform, btVector3 size );
	void DrawBall( ALLEGRO_COLOR color, btTransform transform, float radian );
	
	Camera * GetCamera();
	
	Object * AddObject( std::string name, btCollisionShape * shape, btTransform transform, std::vector < btScalar > collisionBinaryInfo, int type, bool dynamic = false, btScalar mass = 1.0f, btVector3 inertia = btVector3(0,0,0) );
	Object * AddBox( std::string name, btVector3 size, btTransform transform, bool dynamic = false, btScalar mass = 1.0f );
	Object * AddBall( std::string name, btScalar radius, btTransform transform, bool dynamic = false, btScalar mass = 1.0f );
	Object * AddCapsule( std::string name, btScalar radius, btScalar height, btTransform transform, bool dynamic = false, btScalar mass = 1.0f );
	Object * AddCylinder( std::string name, btScalar radius, btScalar height, btTransform transform, bool dynamic = false, btScalar mass = 1.0f );
	Object * AddCustom( std::string name, btCollisionShape * collisionShape, btTransform transform, bool dynamic = false, btScalar mass = 1.0f, btVector3 inertia = btVector3(0,0,0) );		// engine take full control of instance of btCollisionShape * collisionShape
	Object * AddCharacter( std::string name, btScalar width, btScalar height, btTransform transform, btScalar mass );
	void AttachCameraToObject( std::string name, btVector3 location );
	
	bool SetCustomModel( std::string name, Model * mdl );
	
	Model * LoadModel( std::string name, int flags = Model::CENTER_NONE, btVector3 arg1 = btVector3(0,0,0), btVector3 origin = btVector3(0,0,0) );
	Texture * GetTexture( std::string name );
	Model * GetModel( std::string name );
	Object * GetObject( std::string name );
	
	std::string GetAvailableObjectName( std::string name );
	
	void DeleteObject( std::string name );
	
	int CalculateNumberOfSimulationsPerFrame( const float deltaTime );
	void Tick( const float deltaTime );
	
	void Draw2D();
	void Draw3D();
	
	void Init( const char * windowName, const char * iconFile, int width, int height, bool fullscreen = false );
	
	void BeginLoop();
	
	void Destroy();
	
	Engine();
	~Engine();
};

#endif

