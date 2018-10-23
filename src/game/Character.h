
#ifndef CHARACTER_H
#define CHARACTER_H

#include <Object.h>
#include <Camera.h>
#include <cmath>

class Character : public Object
{
protected:
	
	enum WalkMode
	{
		CROUCH,
		STRAVAGE,
		WALK,
		RUN
	};
	
	btVector3 cameraRotation;
	btVector3 cameraLocation;
	
	float height;
	
	float defaultVelocity;
	float jumpHeight;
	
	bool isInAir;
	WalkMode walkMode;
	WalkMode previousWalkMode;
	
	SmartPtr<Camera> camera;
	
	float lastTimeInAir;
	
	float GetMovementVelocity() const;
	btVector3 GetJumpVelocity() const;
	
	void CorrectCameraRotation();
	
public:
	
	virtual void NextOverlappingFrame() override;
	
	float GetBottomY() const;
	
	static btTransform MakeTransformFromEuler( const btVector3 & euler );
	
	void SetCamera( SmartPtr<Camera> camera );
	
	
	virtual void EventOnObjectBeginOverlapp( Object * other, btPersistentManifold * perisstentManifold ) override;
	virtual void EventOnObjectTickOverlapp( Object * other, btPersistentManifold * perisstentManifold ) override;
	virtual void EventOnObjectEndOverlapp( Object * other ) override;
	
	void EventJump();
	void EventCrouch();
	void EventStandUp();
	
	void EventBeginRun();
	void EventStopRun();
	void EventBeginStravage();
	void EventStopStravage();
	void EventMoveInDirection( const btVector3 & direction, bool flat );
	
	void EventRotateCameraBy( const btVector3 & rotation );
	void EventRotateCameraToLookAtPoint( const btVector3 & worldPoint, bool smooth );/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	void SetCameraLocation( const btVector3 & location );
	void SetCameraRotation( const btVector3 & rotation );
	
	btVector3 GetCameraLocation() const;
	btVector3 GetForwardVector() const;
	btVector3 GetFlatForwardVector() const;
	btVector3 GetLeftVector() const;
	btVector3 GetFlatLeftVector() const;
	
	
	virtual void Tick( const float deltaTime ) override;
	virtual void ApplyDamage( const float damage, btVector3 point, btVector3 normal ) override;
	virtual void ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal ) override;
	
	virtual void Draw( const glm::mat4 & cameraMatrix ) override;
	
	Character( Engine * engine, std::string name, SmartPtr<btRigidBody> body, SmartPtr<btCollisionShape> collisionShape, float mass_ );
	Character();
	~Character();
};

#endif

