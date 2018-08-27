
#ifndef CUSTOM_COLLISION_SHAPE_DATA_CPP
#define CUSTOM_COLLISION_SHAPE_DATA_CPP

#include <CustomCollisionData.h>

#include <Debug.h>

SmartPtr<btCollisionShape> CustomCollisionShapeData::GetTriangleShape()
{
	if( !triangleShape )
	{
		if( !triangleData )
		{
			triangleData = new btTriangleIndexVertexArray( indices.size() / 3, &indices.front(), sizeof(int) * 3, vertices.size(), vertices.front().m_floats, sizeof(btVector3) );
			if( !triangleData )
			{
				return triangleShape;
			}
		}
		triangleShape = new btBvhTriangleMeshShape( (btStridingMeshInterface*)(triangleData.GetPtr()), true, true );
	}
	return triangleShape;
}

SmartPtr<btCollisionShape> CustomCollisionShapeData::GetConvexShape()
{
	if( !convexShape )
	{
		convexShape = new btConvexHullShape( vertices.front().m_floats, vertices.size(), sizeof(btVector3) );
	}
	return convexShape;
}

void CustomCollisionShapeData::DestroyTriangleShape()
{
	triangleShape.Delete();
	triangleData.Delete();
}

void CustomCollisionShapeData::DestroyConvexShape()
{
	convexShape.Delete();
}

CustomCollisionShapeData::CustomCollisionShapeData()
{
	friction = 0.5f;
	restitution = 0.5f;
}

CustomCollisionShapeData::~CustomCollisionShapeData()
{
	friction = 0.0f;
	restitution = 0.0f;
}

#endif

