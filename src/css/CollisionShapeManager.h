
#ifndef COLLISION_SHAPE_MANAGER_H
#define COLLISION_SHAPE_MANAGER_H

#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>

#include <vector>
#include <string>
#include <map>

#include <CustomCollisionData.h>

#include <memory>

class Model;
class Object;
class Engine;

class CollisionShapeManager
{
private:
	
	Engine * engine;
	
	std::map < std::vector<btScalar>, std::shared_ptr<btCollisionShape> > shapesVecPtr;
	std::map < std::shared_ptr<btCollisionShape>, std::vector<btScalar> > shapesPtrVec;
	
	std::map < std::string, std::shared_ptr<btCollisionShape> > shapesStrPtr;
	std::map < std::shared_ptr<btCollisionShape>, std::shared_ptr<CustomCollisionShapeData> > shapesPtrPtr;
	
	
	
	
	
	
	std::map < std::vector < btScalar >, std::shared_ptr<btCollisionShape> > collisionShape;
	std::map < std::shared_ptr<btCollisionShape>, std::vector < btScalar > > collisionShapeRev;
	std::map < std::shared_ptr<btCollisionShape>, int > numberOfReferencesToShape;
	
	std::map < std::shared_ptr<CustomCollisionShapeData>, std::shared_ptr<Model> > modelPointerCustomCollisionData;
	std::map < std::shared_ptr<CustomCollisionShapeData>, int > modelCustomCollisionData;
	std::map < std::shared_ptr<btCollisionShape>, std::shared_ptr<CustomCollisionShapeData> > customCollisionShapeData;
	std::map < std::string, std::shared_ptr<btCollisionShape> > customCollisionShape;
	std::map < std::shared_ptr<btCollisionShape>, std::string > customCollisionShapeName;
	
	std::shared_ptr<btCollisionShape> GetShape( std::vector < btScalar > constructionData, bool independent );
	std::shared_ptr<btCollisionShape> AddShape( std::vector < btScalar > constructionData, std::string name );
	
	friend class Engine;
	
public:
	
	static const int SHAPE_TYPE_CONVEX = 0;
	static const int SHAPE_TYPE_TRIANGLE = 1;
	
	std::string GetFirstAvailableName( std::string name );
	bool IsNameAvailable( std::string name );
	
	// if name == "" ->collisionShape else ->customCollisionShape
	std::shared_ptr<btCollisionShape> GetBox( btVector3 size, std::string name = "" );
	std::shared_ptr<btCollisionShape> GetBall( btScalar radius, std::string name = "" );
	std::shared_ptr<btCollisionShape> GetCapsule( btScalar radius, btScalar height, std::string name = "" );
	std::shared_ptr<btCollisionShape> GetCylinder( btScalar radius, btScalar height, std::string name = "" );
	
	std::shared_ptr<btCollisionShape> CreateCustomShape( std::string name, std::shared_ptr<Model> model, int shapeType );
	std::shared_ptr<btCollisionShape> GetCustomShape( std::string name );
	void RemoveCustomShape( std::string name );
	void RemoveShape( std::shared_ptr<btCollisionShape> shape );
	
	void Destroy();
	
	CollisionShapeManager( Engine * engine );
	~CollisionShapeManager();
};


#endif

