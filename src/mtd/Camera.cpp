
#ifndef CAMERA_CPP
#define CAMERA_CPP

#include <Camera.h>

#include <Math.hpp>

btTransform Camera::GetTransform() const
{
	return btTransform
		(
			parentTransformation.getRotation() *
			btQuaternion( btVector3(0,1,0), rot.y() ) *
			btQuaternion( btVector3(1,0,0), rot.x() ) *
			btQuaternion( btVector3(0,0,1), rot.z() ),
			GetLocation()
		);
}

btQuaternion Camera::GetRotation() const
{
	return parentTransformation.getRotation() *
			btQuaternion( btVector3(0,1,0), rot.y() ) *
			btQuaternion( btVector3(1,0,0), rot.x() ) *
			btQuaternion( btVector3(0,0,1), rot.z() );
}

btQuaternion Camera::GetFlatRotation() const
{
	return parentTransformation.getRotation() *
			btQuaternion( btVector3(0,1,0), rot.y() );
}

btVector3 Camera::GetUpVector() const
{
	return btTransform( GetRotation() ) * btVector3( 0, 1, 0 );
}

btVector3 Camera::GetFlatRightVector() const
{
	return btTransform( GetFlatRotation() ) * btVector3( -1, 0, 0 );
}

btVector3 Camera::GetRightVector() const
{
	return btTransform( GetRotation() ) * btVector3( -1, 0, 0 );
}

btVector3 Camera::GetFlatForwardVector() const
{
	return btTransform( GetFlatRotation() ) * btVector3( 0, 0, 1 );
}

btVector3 Camera::GetForwardVector() const
{
	return btTransform( GetRotation() ) * btVector3( 0, 0, 1 );
}

btVector3 Camera::GetPos() const
{
	return pos;
}

btVector3 Camera::GetRot() const
{
	return rot;
}

btVector3 Camera::GetLocation() const
{
	return parentTransformation.getOrigin() + pos;
}

void Camera::SetPos( btVector3 src )
{
	pos = src;
	sceneNode->setPosition( irr::core::vector3d<float>( GetLocation().x(), GetLocation().y(), GetLocation().z() ) );
	btVector3 p = GetLocation() + ( GetForwardVector() );
	irr::core::vector3d<float> vec( p.x(), p.y(), p.z() );
	sceneNode->setTarget( vec );
}

void Camera::SetRotation( btVector3 src )
{
	rot = src;
	
	if( rot.x() > Math::PI / 2.0 )
		rot.m_floats[0] = Math::PI / 2.0;
	else if( rot.x() < -Math::PI / 2.0 )
		rot.m_floats[0] = -Math::PI / 2.0;
	
	if( rot.y() > Math::PI * 2.0 )
		rot.m_floats[1] -= Math::PI * 2.0;
	else if( rot.y() < -Math::PI * 2.0 )
		rot.m_floats[1] += Math::PI * 2.0;
	
	if( rot.z() > Math::PI * 2.0 )
		rot.m_floats[2] -= Math::PI * 2.0;
	else if( rot.z() < -Math::PI * 2.0 )
		rot.m_floats[2] += Math::PI * 2.0;
	
	btVector3 p = GetLocation() + ( GetForwardVector() );
	irr::core::vector3d<float> vec( p.x(), p.y(), p.z() );
	sceneNode->setTarget( vec );
}

void Camera::Move( btVector3 src )
{
	pos += src;
	sceneNode->setPosition( irr::core::vector3d<float>( GetLocation().x(), GetLocation().y(), GetLocation().z() ) );
	btVector3 p = GetLocation() + ( GetForwardVector() );
	irr::core::vector3d<float> vec( p.x(), p.y(), p.z() );
	sceneNode->setTarget( vec );
}

void Camera::Rotate( btVector3 src )
{
	rot += src;
	
	if( rot.x() > Math::PI / 2.0 )
		rot.m_floats[0] = Math::PI / 2.0;
	else if( rot.x() < -Math::PI / 2.0 )
		rot.m_floats[0] = -Math::PI / 2.0;
	
	if( rot.y() > Math::PI * 2.0 )
		rot.m_floats[1] -= Math::PI * 2.0;
	else if( rot.y() < -Math::PI * 2.0 )
		rot.m_floats[1] += Math::PI * 2.0;
	
	if( rot.z() > Math::PI * 2.0 )
		rot.m_floats[2] -= Math::PI * 2.0;
	else if( rot.z() < -Math::PI * 2.0 )
		rot.m_floats[2] += Math::PI * 2.0;
	
	btVector3 p = GetLocation() + ( GetForwardVector() );
	irr::core::vector3d<float> vec( p.x(), p.y(), p.z() );
	sceneNode->setTarget( vec );
}

void Camera::SetCameraTransform( btTransform transform )
{
	parentTransformation = transform;
	sceneNode->setPosition( irr::core::vector3d<float>( GetLocation().x(), GetLocation().y(), GetLocation().z() ) );
	btVector3 p = GetLocation() + ( GetForwardVector() );
	irr::core::vector3d<float> vec( p.x(), p.y(), p.z() );
	sceneNode->setTarget( vec );
}

Camera::Camera()
{
	pos = btVector3(0,0,0);
	rot = btVector3(0,0,0);
	locationScale = btVector3(1,1,1);
	parentTransformation = btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,0,0) );
}

Camera::~Camera()
{
	pos = btVector3(0,0,0);
	rot = btVector3(0,0,0);
	locationScale = btVector3(0,0,0);
	parentTransformation = btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,0,0) );
}

#endif

