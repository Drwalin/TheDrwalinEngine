
#ifndef MODEL_H
#define MODEL_H

#include <irrlicht\irrlicht.h>

#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <cstring>
#include <cstdio>

#include <Debug.h>
#include <AR.hpp>
#include <SmartPtr.h>

#include <CustomCollisionData.h>

class Engine;

class Model
{
private:
	
	struct Face
	{
		unsigned a, b, c;
		btVector3 normal;
	};
	
	irr::scene::IAnimatedMesh * mesh;
	std::map < int, irr::video::SMaterial > materials;
	
	Engine * engine;
	
	btVector3 minAABB, maxAABB;
	
	SmartPtr<CustomCollisionShapeData> collisionShapeData;
	
public:
	
	void SetMaterialsToNode( irr::scene::ISceneNode * node );
	
	btVector3 GetInertia() const;
	
	irr::scene::IAnimatedMesh * GetMesh();
	
	bool LoadCustomCollisionShapeFromObj( std::string objFileName );
	
	bool LoadFromObj( Engine * engine, std::string objFileName );
	
	SmartPtr<CustomCollisionShapeData> GetCustomCollisionShapeData( float acceptableDistanceToJoinVertices = 0.0311 );
	void NullCustomCollisionShape();
	
	void Destroy();
	
	Model( const Model * other );
	Model();
	~Model();
};

#endif

