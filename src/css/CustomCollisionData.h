
#ifndef CUSTOM_COLLISION_SHAPE_DATA_H
#define CUSTOM_COLLISION_SHAPE_DATA_H

#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>

#include <vector>

#include <SmartPtr.h>

class CustomCollisionShapeData
{
private:
	
	std::vector < btVector3 > vertices;
	std::vector < int > indices;
	
	SmartPtr<btCollisionShape> convexShape;
	
	SmartPtr<btTriangleIndexVertexArray> triangleData;
	SmartPtr<btCollisionShape> triangleShape;
	
	float friction;
	float restitution;
	
	friend class Model;
	
public:
	
	SmartPtr<btCollisionShape> GetTriangleShape();
	SmartPtr<btCollisionShape> GetConvexShape();
	
	void DestroyTriangleShape();
	void DestroyConvexShape();
	
	CustomCollisionShapeData();
	~CustomCollisionShapeData();
};

#endif

