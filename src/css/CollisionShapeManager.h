
#ifndef COLLISION_SHAPE_MANAGER_H
#define COLLISION_SHAPE_MANAGER_H

#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>

#include <vector>
#include <string>
#include <map>

#include <CustomCollisionData.h>

class Model;
class Object;

class CollisionShapeManager
{
private:
	
	std::map < std::vector < btScalar >, btCollisionShape* > collisionShape;
	std::map < btCollisionShape*, std::vector < btScalar > > collisionShapeRev;
	std::map < btCollisionShape*, int > numberOfReferencesToShape;
	
	std::map < CustomCollisionShapeData*, Model* > modelPointerCustomCollisionData;
	std::map < CustomCollisionShapeData*, int > modelCustomCollisionData;
	std::map < btCollisionShape*, CustomCollisionShapeData* > customCollisionShapeData;
	std::map < std::string, btCollisionShape* > customCollisionShape;
	std::map < btCollisionShape*, std::string > customCollisionShapeName;
	
	
	btCollisionShape * GetShape( std::vector < btScalar > constructionData, bool independent );
	btCollisionShape * AddShape( std::vector < btScalar > constructionData, std::string name );
	
	friend class Engine;
	
public:
	
	static const int SHAPE_TYPE_CONVEX = 0;
	static const int SHAPE_TYPE_TRIANGLE = 1;
	
	std::string GetFirstAvailableName( std::string name );
	bool IsNameAvailable( std::string name );
	
	// if name == "" ->collisionShape else ->customCollisionShape
	btCollisionShape * GetBox( btVector3 size, std::string name = "" );
	btCollisionShape * GetBall( btScalar radius, std::string name = "" );
	btCollisionShape * GetCapsule( btScalar radius, btScalar height, std::string name = "" );
	btCollisionShape * GetCylinder( btScalar radius, btScalar height, std::string name = "" );
	
	btCollisionShape * CreateCustomShape( std::string name, Model * model, int shapeType );
	btCollisionShape * GetCustomShape( std::string name );
	void RemoveCustomShape( std::string name );
	void RemoveShape( btCollisionShape * shape );
	
	void Destroy();
	
	CollisionShapeManager();
	~CollisionShapeManager();
};


#endif

