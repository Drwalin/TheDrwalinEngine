
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
	btTransform parentTransformation;
	
public:
	
	btVector3 GetFlatRightVector();
	btVector3 GetRightVector();
	btVector3 GetFlatForwardVector();
	btVector3 GetForwardVector();
	btVector3 GetPos();
	btVector3 GetRot();
	void SetPos( btVector3 src );
	void SetRotation( btVector3 src );
	void Move( btVector3 src );
	void Rotate( btVector3 src );
	void SetWorldTransform( btTransform transform );
	void SetCameraTransform( btTransform transform );
	
	Camera();
	~Camera();
};

#endif

