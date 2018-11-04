
#ifndef CAMERA_H
#define CAMERA_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>

#include <Debug.h>
#include <SmartPtr.h>

#include <Object.h>

class Camera
{
private:
	
	btVector3 pos;
	btVector3 rot;
	btTransform parentTransformation;
	btVector3 locationScale;
	
	btVector3 currentLocation;
	
	void UpdateCameraView();
	
public:
	
	irr::scene::ICameraSceneNode * sceneNode;
	
	btTransform GetTransform() const;
	btQuaternion GetRotation() const;
	btQuaternion GetFlatRotation() const;
	
	btVector3 GetFlatRightVector() const;
	btVector3 GetRightVector() const;
	btVector3 GetFlatForwardVector() const;
	btVector3 GetForwardVector() const;
	btVector3 GetUpVector() const;
	
	btVector3 GetLocation() const;
	
	btVector3 GetPos() const;
	btVector3 GetRot() const;
	
	void SetPos( btVector3 src );
	void SetRotation( btVector3 src );
	void Move( btVector3 src );
	void Rotate( btVector3 src );
	void SetCameraTransform( btTransform transform );
	
	Camera();
	~Camera();
};

#endif

