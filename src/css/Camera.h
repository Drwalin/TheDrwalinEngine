
#ifndef CAMERA_H
#define CAMERA_H

#include <allegro5/allegro.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Debug.h>
#include <SmartPtr.h>

#include <Shader.h>
#include <Object.h>

class Camera
{
private:
	
	glm::mat4 modelTransform;
	glm::mat4 viewTransform;
	
	btVector3 pos;
	btVector3 rot;
	btTransform parentTransformation;
	btVector3 locationScale;
	
	btVector3 normal[5];		// view side planes normals
	btVector3 currentLocation;
	
public:
	
	glm::mat4 GetModelMatrix() const;
	glm::mat4 GetViewMatrix() const;
	
	bool IsObjectInView( SmartPtr<Object> object );
	void UpdateViewPlanes();
	
	void SetLocationScale( btVector3 scale );
	btVector3 GetLocationScale();
	
	btTransform GetTransform();
	
	btVector3 GetFlatRightVector();
	btVector3 GetRightVector();
	btVector3 GetFlatForwardVector();
	btVector3 GetForwardVector();
	btVector3 GetUpVector();
	
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

