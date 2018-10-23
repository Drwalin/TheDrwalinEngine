
#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"

class Player : public Character
{
protected:
	
	
	
public:
	
	virtual void Tick( const float deltaTime ) override;
	//virtual void ApplyDamage( const float damage, btVector3 point, btVector3 normal ) override;
	//virtual void ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal ) override;
	
	virtual void Draw( const glm::mat4 & cameraMatrix ) override;
	
	Player( Engine * engine, std::string name, SmartPtr<btRigidBody> body, SmartPtr<btCollisionShape> collisionShape, float mass_ );
	Player();
	~Player();
};

#endif

