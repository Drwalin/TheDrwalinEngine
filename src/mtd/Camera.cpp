
#ifndef CAMERA_CPP
#define CAMERA_CPP

#include <Camera.h>
#include <Object.h>

#include <Math.hpp>

bool Camera::IsObjectInView( SmartPtr<Object> object )
{
	if( object )
	{
		float radius = object->GetRadius();
		btVector3 location = object->GetLocation();
		
		for( int i = 0; i < 4; ++i )
			if( normal[i].dot( currentLocation - location ) + radius < 0 )
				return false;
		return true;
	}
	return false;
}

void Camera::UpdateViewPlanes()		// need to use basicWindow for y/x bitmap size ration
{
	//btVector3 vectors[4];
	currentLocation = GetLocation();
	
	normal[4] = -GetForwardVector().normalized();
	
	normal[0] = ( GetRightVector().normalized() * 0.75f ) + normal[4];
	normal[1] = -( GetRightVector().normalized() * 0.75f ) + normal[4];
	normal[2] = GetUpVector().normalized() + normal[4];
	normal[3] = -GetUpVector().normalized() + normal[4];
	
	normal[0].normalize();
	normal[1].normalize();
	normal[2].normalize();
	normal[3].normalize();
}

btTransform Camera::GetTransform()
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

void Camera::SetLocationScale( btVector3 scale )
{
	locationScale = scale;
}

btVector3 Camera::GetLocationScale()
{
	return locationScale;
}

btVector3 Camera::GetUpVector()
{
	btVector3 dst(0,1,0);
	btVector3 right = GetFlatRightVector();
	btVector3 forward = GetForwardVector();
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	
	btScalar x, y, z;
	parentTransformation.getRotation().getEulerZYX( z, y, x );
	
	al_rotate_transform_3d( &transform, 0, 1, 0, -(rot.y()-y) + ALLEGRO_PI );
	al_rotate_transform_3d( &transform, right.x(), right.y(), right.z(), -(rot.x()+x) );
	al_rotate_transform_3d( &transform, forward.x(), forward.y(), forward.z(), -(rot.z()+z) );
	al_transform_coordinates_3d( &transform, dst.m_floats, dst.m_floats+1, dst.m_floats+2 );
	return dst;
}

btVector3 Camera::GetFlatRightVector()
{
	btVector3 dst(-1,0,0);
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	
	btScalar x, y, z;
	parentTransformation.getRotation().getEulerZYX( z, y, x );
	
	al_rotate_transform_3d( &transform, 0, 1, 0, -(rot.y()-y) + ALLEGRO_PI );
	al_transform_coordinates_3d( &transform, dst.m_floats, dst.m_floats+1, dst.m_floats+2 );
	return dst;
}

btVector3 Camera::GetRightVector()
{
	btVector3 dst(-1,0,0);
	btVector3 forward = GetForwardVector();
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	
	btScalar x, y, z;
	parentTransformation.getRotation().getEulerZYX( z, y, x );
	
	al_rotate_transform_3d( &transform, 0, 1, 0, -(rot.y()-y) + ALLEGRO_PI );
	al_rotate_transform_3d( &transform, forward.x(), forward.y(), forward.z(), -(rot.z()+z) );
	al_transform_coordinates_3d( &transform, dst.m_floats, dst.m_floats+1, dst.m_floats+2 );
	return dst;
}

btVector3 Camera::GetFlatForwardVector()
{
	btVector3 dst(0,0,1);
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	
	btScalar x, y, z;
	parentTransformation.getRotation().getEulerZYX( z, y, x );
	
	al_rotate_transform_3d( &transform, 0, 1, 0, -(rot.y()-y) + ALLEGRO_PI );
	al_transform_coordinates_3d( &transform, dst.m_floats, dst.m_floats+1, dst.m_floats+2 );
	return dst;
}

btVector3 Camera::GetForwardVector()
{
	btVector3 dst(0,0,1);
	btVector3 right = GetFlatRightVector();
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	
	btScalar x, y, z;
	parentTransformation.getRotation().getEulerZYX( z, y, x );
	
	al_rotate_transform_3d( &transform, 0, 1, 0, -(rot.y()-y) + ALLEGRO_PI );
	al_rotate_transform_3d( &transform, right.x(), right.y(), right.z(), -(rot.x()+x) );
	al_transform_coordinates_3d( &transform, dst.m_floats, dst.m_floats+1, dst.m_floats+2 );
	return dst;
}

btVector3 Camera::GetPos()
{
	return pos;
}

btVector3 Camera::GetRot()
{
	return rot;
}

btVector3 Camera::GetLocation()
{
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	
	btVector3 origin = parentTransformation.getOrigin();
	
	al_translate_transform_3d( &transform, pos.x()*locationScale.x(), pos.y()*locationScale.y(), pos.z()*locationScale.z() );
	al_translate_transform_3d( &transform, origin.x(), origin.y(), origin.z() );
	
	btVector3 dst(0,0,0);
	
	al_transform_coordinates_3d( &transform, dst.m_floats, dst.m_floats+1, dst.m_floats+2 );
	
	return dst;
}

void Camera::SetPos( btVector3 src )
{
	pos = src;
}

void Camera::SetRotation( btVector3 src )
{
	rot = src;
	if( rot.x() > ALLEGRO_PI / 2.0 )
		rot.m_floats[0] = ALLEGRO_PI / 2.0;
	else if( rot.x() < -ALLEGRO_PI / 2.0 )
		rot.m_floats[0] = -ALLEGRO_PI / 2.0;
	
	if( rot.y() > ALLEGRO_PI * 2.0 )
		rot.m_floats[1] -= ALLEGRO_PI * 2.0;
	else if( rot.y() < -ALLEGRO_PI * 2.0 )
		rot.m_floats[1] += ALLEGRO_PI * 2.0;
	
	if( rot.z() > ALLEGRO_PI * 2.0 )
		rot.m_floats[2] -= ALLEGRO_PI * 2.0;
	else if( rot.z() < -ALLEGRO_PI * 2.0 )
		rot.m_floats[2] += ALLEGRO_PI * 2.0;
}

void Camera::Move( btVector3 src )
{
	pos += src;
}

void Camera::Rotate( btVector3 src )
{
	rot += src;
	if( rot.x() > ALLEGRO_PI / 2.0 )
		rot.m_floats[0] = ALLEGRO_PI / 2.0;
	else if( rot.x() < -ALLEGRO_PI / 2.0 )
		rot.m_floats[0] = -ALLEGRO_PI / 2.0;
	
	if( rot.y() > ALLEGRO_PI * 2.0 )
		rot.m_floats[1] -= ALLEGRO_PI * 2.0;
	else if( rot.y() < -ALLEGRO_PI * 2.0 )
		rot.m_floats[1] += ALLEGRO_PI * 2.0;
	
	if( rot.z() > ALLEGRO_PI * 2.0 )
		rot.m_floats[2] -= ALLEGRO_PI * 2.0;
	else if( rot.z() < -ALLEGRO_PI * 2.0 )
		rot.m_floats[2] += ALLEGRO_PI * 2.0;
}

void Camera::SetWorldTransform( btTransform transform, btVector3 scale )
{
	{
		/*
		btVector3 origin = transform.getOrigin();
		btQuaternion rotation = transform.getRotation();
		btVector3 axis = rotation.getAxis().normalized();
		btScalar angle = rotation.getAngle();
		
		modelTransform = glm::mat4(1.0f);
		modelTransform = glm::scale( modelTransform, glm::vec3( scale.x(), scale.y(), scale.z() ) );
		modelTransform = glm::translate( modelTransform, glm::vec3( origin.x(), origin.y(), origin.z() ) );
		modelTransform = glm::rotate( modelTransform, angle, glm::vec3( axis.x(), axis.y(), axis.z() ) );
		*/
		modelTransform = Math::GetMatrix( transform, scale );
	}
	
	{
		btVector3 origin = parentTransformation.getOrigin();
		btScalar x, y, z;
		parentTransformation.getRotation().getEulerZYX( z, y, x );
		
		viewTransform = glm::mat4(1.0f);
		viewTransform = glm::rotate( viewTransform, rot.z(), glm::vec3( 0, 0, 1 ) );
		viewTransform = glm::rotate( viewTransform, rot.x()+x, glm::vec3( 1, 0, 0 ) );
		viewTransform = glm::rotate( viewTransform, rot.y()+y, glm::vec3( 0, 1, 0 ) );
		viewTransform = glm::translate( viewTransform, -glm::vec3( pos.x()*locationScale.x(), pos.y()*locationScale.y(), pos.z()*locationScale.z() ) - glm::vec3( origin.x(), origin.y(), origin.z() ) );		// ??
	}
	
	if( false )
	{
		ALLEGRO_TRANSFORM dst;
		al_identity_transform( &dst );
		
		al_scale_transform_3d( &dst, scale.x(), scale.y(), scale.z() );
		
		{
			btVector3 origin = transform.getOrigin();
			btQuaternion rotation = transform.getRotation();
			btVector3 axis = rotation.getAxis();
			btScalar angle = rotation.getAngle();
			
			al_rotate_transform_3d( &dst, axis.x(), axis.y(), axis.z(), angle );
			al_translate_transform_3d( &dst, origin.x(), origin.y(), origin.z() );
		}
		
		btVector3 origin = -parentTransformation.getOrigin();
		
		btScalar x, y, z;
		parentTransformation.getRotation().getEulerZYX( z, y, x );
			
		al_translate_transform_3d( &dst, -pos.x()*locationScale.x(), -pos.y()*locationScale.y(), -pos.z()*locationScale.z() );
		al_translate_transform_3d( &dst, origin.x(), origin.y(), origin.z() );
		al_rotate_transform_3d( &dst, 0, 1, 0, rot.y()-y );
		al_rotate_transform_3d( &dst, 1, 0, 0, rot.x()+x );
		al_rotate_transform_3d( &dst, 0, 0, 1, rot.z()+z );
		
		al_use_transform( &dst );
	}
}

void Camera::SetCameraTransform( btTransform transform )
{
	parentTransformation = transform;
}

glm::mat4 Camera::GetModelMatrix() const
{
	return modelTransform;
}

glm::mat4 Camera::GetViewMatrix() const
{
	btVector3 origin = parentTransformation.getOrigin();
	btScalar x, y, z;
	parentTransformation.getRotation().getEulerZYX( z, y, x );
	
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::rotate( view, rot.z(), glm::vec3( 0, 0, 1 ) );
	view = glm::rotate( view, rot.x()+x, glm::vec3( 1, 0, 0 ) );
	view = glm::rotate( view, rot.y()+y, glm::vec3( 0, 1, 0 ) );
	view = glm::translate( view, -glm::vec3( pos.x()*locationScale.x(), pos.y()*locationScale.y(), pos.z()*locationScale.z() ) - glm::vec3( origin.x(), origin.y(), origin.z() ) );		// ??
	
	return view;
}

Camera::Camera()
	: viewTransform(1.0f), modelTransform(1.0f)
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

