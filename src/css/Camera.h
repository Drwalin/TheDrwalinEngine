
#ifndef CAMERA_H
#define CAMERA_H

#include <allegro5/allegro.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>

class Camera
{
private:
	
	btVector3 pos;
	btVector3 rot;
	
public:
	
	btVector3 GetFlatRightVector();
	btVector3 GetRightVector();
	btVector3 GetFlatForwardVector();
	btVector3 GetForwardVector();
	void SetPos( btVector3 src );
	void SetRotation( btVector3 src );
	void Move( btVector3 src );
	void Rotate( btVector3 src );
	void SetWorldTransform( btTransform transform );
	
	Camera();
	~Camera();
};

#endif

