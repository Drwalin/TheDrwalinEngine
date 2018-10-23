
#ifndef OBJECT_H
#define OBJECT_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <set>

#include <Debug.h>
#include <SmartPtr.h>

#include <Model.h>

class Engine;

class Object
{
protected:
	
	Engine * engine;
	
	std::string name;
	
	btTransform currentTransform;
	btTransform previousTransform;
	
	SmartPtr<btCollisionShape> collisionShape;
	SmartPtr<btRigidBody> body;
	SmartPtr<Model> model;
	btVector3 scale;
	float boundingSphereRadius;
	
	int rayTraceChannel;
	
	float mass;
	
	std::set< Object* > overlappingInPreviousFrame;
	std::set< Object* > overlappingInCurrentFrame;
	
	SmartPtr<Object> thisPtr;
	
public:
	
	virtual void NextOverlappingFrame();
	void OverlapWithObject( Object * other, btPersistentManifold * perisstentManifold );
	
	
	SmartPtr<Object> GetThis();
	
	
	void SetMass( float mass );
	
	bool IsDynamic() const;
	
	Engine * GetEngine();
	
	std::string GetName() const;
	
	void SetRayTraceChannel( int src );
	int GetRayTraceChannel();
	
	float GetRadius();
	void CalculateRadius();
	
	void SetScale( btVector3 scale );
	btVector3 GetScale();
	
	btTransform GetTransform();
	btVector3 GetLocation() const;
	
	SmartPtr<btRigidBody> GetBody();
	
	virtual void EventOnObjectBeginOverlapp( Object * other, btPersistentManifold * perisstentManifold );
	virtual void EventOnObjectTickOverlapp( Object * other, btPersistentManifold * perisstentManifold );
	virtual void EventOnObjectEndOverlapp( Object * other );
	
	virtual void Tick( const float deltaTime );
	virtual void ApplyDamage( const float damage, btVector3 point, btVector3 normal );
	virtual void ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal );
	
	virtual void Draw( const glm::mat4 & cameraMatrix );
	
	void SetModel( SmartPtr<Model> model );
	
	Object( Engine * engine, std::string name, SmartPtr<btRigidBody> body, SmartPtr<btCollisionShape> collisionShape, float mass_ );
	Object();
	~Object();
};

#endif

