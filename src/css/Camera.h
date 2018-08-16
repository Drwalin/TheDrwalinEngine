
#ifndef CAMERA_H
#define CAMERA_H

#include <allegro5/allegro.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>

#include <Debug.h>

class Camera
{
private:
	
	btVector3 pos;
	btVector3 rot;
	btTransform parentTransformation;
	btVector3 locationScale;
	
public:
	
	void SetLocationScale( btVector3 scale );
	btVector3 GetLocationScale();
	
	btVector3 GetFlatRightVector();
	btVector3 GetRightVector();
	btVector3 GetFlatForwardVector();
	btVector3 GetForwardVector();
	
	btVector3 GetLocation();
	
	btVector3 GetPos();
	btVector3 GetRot();
	
	void SetPos( btVector3 src );
	void SetRotation( btVector3 src );
	void Move( btVector3 src );
	void Rotate( btVector3 src );
	void SetWorldTransform( btTransform transform, btVector3 scale = btVector3(1,1,1) );
	void SetCameraTransform( btTransform transform );
	
	Camera();
	~Camera();
};

#endif

