
#ifndef COLLISION_SHAPE_MANAGER_H
#define COLLISION_SHAPE_MANAGER_H

#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>

#include <vector>
#include <string>
#include <map>

#include <CustomCollisionData.h>

#include <SmartPtr.h>

class Model;
class Object;

class CollisionShapeManager
{
private:
	
	std::map < std::vector < btScalar >, SmartPtr<btCollisionShape> > collisionShape;
	std::map < SmartPtr<btCollisionShape>, std::vector < btScalar > > collisionShapeRev;
	std::map < SmartPtr<btCollisionShape>, int > numberOfReferencesToShape;
	
	std::map < SmartPtr<CustomCollisionShapeData>, SmartPtr<Model> > modelPointerCustomCollisionData;
	std::map < SmartPtr<CustomCollisionShapeData>, int > modelCustomCollisionData;
	std::map < SmartPtr<btCollisionShape>, SmartPtr<CustomCollisionShapeData> > customCollisionShapeData;
	std::map < std::string, SmartPtr<btCollisionShape> > customCollisionShape;
	std::map < SmartPtr<btCollisionShape>, std::string > customCollisionShapeName;
	
	
	SmartPtr<btCollisionShape> GetShape( std::vector < btScalar > constructionData, bool independent );
	SmartPtr<btCollisionShape> AddShape( std::vector < btScalar > constructionData, std::string name );
	
	friend class Engine;
	
public:
	
	static const int SHAPE_TYPE_CONVEX = 0;
	static const int SHAPE_TYPE_TRIANGLE = 1;
	
	std::string GetFirstAvailableName( std::string name );
	bool IsNameAvailable( std::string name );
	
	// if name == "" ->collisionShape else ->customCollisionShape
	SmartPtr<btCollisionShape> GetBox( btVector3 size, std::string name = "" );
	SmartPtr<btCollisionShape> GetBall( btScalar radius, std::string name = "" );
	SmartPtr<btCollisionShape> GetCapsule( btScalar radius, btScalar height, std::string name = "" );
	SmartPtr<btCollisionShape> GetCylinder( btScalar radius, btScalar height, std::string name = "" );
	
	SmartPtr<btCollisionShape> CreateCustomShape( std::string name, SmartPtr<Model> model, int shapeType );
	SmartPtr<btCollisionShape> GetCustomShape( std::string name );
	void RemoveCustomShape( std::string name );
	void RemoveShape( SmartPtr<btCollisionShape> shape );
	
	void Destroy();
	
	CollisionShapeManager();
	~CollisionShapeManager();
};


#endif

