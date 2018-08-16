
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

class Engine;

class Model
{
private:
	
	std::vector < VBO > vbo;
	
public:
	
	const static int CENTER_NONE = 0;
	const static int CENTER_BARYCENTER = 1;
	const static int CENTER_AABB_MID = 2;
	const static int RESCALE_TO_SIZE = 4;
	const static int RESCALE_WITH_SCALE = 8;
	
	void Draw();
	
	bool LoadFromObj( Engine * engine, std::string fileName, int flags = Model::CENTER_NONE, btVector3 arg1 = btVector3(0,0,0), btVector3 origin = btVector3(0,0,0) );
	//bool loadFromCompressedFile( Engine * engine, std::string fileName );
	
	btCollisionShape * MakeConvexCollisionShape();
	btCollisionShape * MakeStaticTriangleCollisionShape();
	
	void Destroy();
	
	Model();
	~Model();
};

#endif

