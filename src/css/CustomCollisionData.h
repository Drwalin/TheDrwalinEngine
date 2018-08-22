
#ifndef CUSTOM_COLLISION_SHAPE_DATA_H
#define CUSTOM_COLLISION_SHAPE_DATA_H

#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>

#include <vector>

class CustomCollisionShapeData
{
private:
	
	std::vector < btVector3 > vertices;
	std::vector < int > indices;
	
	btCollisionShape * convexShape;
	
	btTriangleIndexVertexArray * triangleData;
	btCollisionShape * triangleShape;
	
	float friction;
	float restitution;
	
	friend class Model;
	
public:
	
	btCollisionShape * GetTriangleShape();
	btCollisionShape * GetConvexShape();
	
	void DestroyTriangleShape();
	void DestroyConvexShape();
	
	CustomCollisionShapeData();
	~CustomCollisionShapeData();
};

#endif

