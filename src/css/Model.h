
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
	
	void Draw();
	
	bool LoadFromObj( Engine * engine, std::string fileName );
	//bool loadFromCompressedFile( Engine * engine, std::string fileName );
	
	btCollisionShape * MakeConvexCollisionShape();
	btCollisionShape * MakeStaticTriangleCollisionShape();
	
	void Destroy();
	
	Model();
	~Model();
};

#endif

