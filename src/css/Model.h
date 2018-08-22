
#ifndef MODEL_H
#define MODEL_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

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

#include <VBO.h>
#include <CustomCollisionData.h>

class Engine;

class Model
{
private:
	
	std::vector < VBO > vbo;
	Engine * engine;
	
	CustomCollisionShapeData * collisionShapeData;
	
public:
	
	void Draw();
	
	static bool SaveMeshFile( std::string meshFileName, float friction, float restitution, std::map < std::string, std::vector < ALLEGRO_VERTEX > > & trianglesMaterial, std::map < std::string, std::string > & materialTexture );
	static void RescaleAndMove( std::map < std::string, std::vector < ALLEGRO_VERTEX > > & trianglesMaterial, btVector3 min, btVector3 max, bool scaleToSize = false, btVector3 size = btVector3(1,1,1), Engine * engine = NULL, const std::map < std::string, std::string > & materialTexture = std::map<std::string,std::string>() );
	static bool LoadMtl( std::string mtlFileName, std::map < std::string, std::string > & materialTexture );
	static bool LoadObj( std::string objFileName, std::map < std::string, std::vector < ALLEGRO_VERTEX > > & trianglesMaterial, btVector3 & min, btVector3 & max );
	static bool ConvertObjToMesh( std::string objFileName, float friction = 0.5f, float restitution = 0.0f, bool scaleToSize = false, btVector3 size = btVector3(1,1,1) );
	
	bool LoadFromObj( Engine * engine, std::string objFileName );
	bool loadFromMeshFile( Engine * engine, std::string meshFileName );
	bool LoadFromFile( Engine * engine, std::string fileName );
	
	CustomCollisionShapeData * GetCustomCollisionShapeData( float acceptableDistanceToJoinVertices = 0.0311 );
	void NullCustomCollisionShape();
	
	void Destroy();
	
	Model();
	~Model();
};

#endif

