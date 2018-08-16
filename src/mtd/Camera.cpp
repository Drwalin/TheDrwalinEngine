
#ifndef CAMERA_CPP
#define CAMERA_CPP

#include <Camera.h>

btVector3 Camera::GetFlatRightVector()
{
	btVector3 dst(-1,0,0);
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	al_rotate_transform_3d( &transform, 0, 1, 0, -rot.getY() + ALLEGRO_PI );
	al_transform_coordinates_3d( &transform, dst.m_floats, dst.m_floats+1, dst.m_floats+2 );
	return dst;
}

btVector3 Camera::GetRightVector()
{
	btVector3 dst(-1,0,0);
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	al_rotate_transform_3d( &transform, 0, 1, 0, -rot.getY() + ALLEGRO_PI );
	al_rotate_transform_3d( &transform, 1, 0, 0, -rot.getX() );
	al_rotate_transform_3d( &transform, 0, 0, 1, rot.getZ() );
	al_transform_coordinates_3d( &transform, dst.m_floats, dst.m_floats+1, dst.m_floats+2 );
	return dst;
}

btVector3 Camera::GetFlatForwardVector()
{
	btVector3 dst(0,0,1);
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	al_rotate_transform_3d( &transform, 0, 1, 0, -rot.getY() + ALLEGRO_PI );
	al_transform_coordinates_3d( &transform, dst.m_floats, dst.m_floats+1, dst.m_floats+2 );
	return dst;
}

btVector3 Camera::GetForwardVector()
{
	btVector3 dst(0,0,1);
	btVector3 a;
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	a = GetFlatRightVector();
	al_rotate_transform_3d( &transform, 0, 1, 0, -rot.getY() + ALLEGRO_PI );
	al_rotate_transform_3d( &transform, a.getX(), a.getY(), a.getZ(), -rot.getX() );
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

void Camera::SetPos( btVector3 src )
{
	pos = src;
}

void Camera::SetRotation( btVector3 src )
{
	rot = src;
	if( rot.getX() > ALLEGRO_PI / 2.0 )
		rot.m_floats[0] = ALLEGRO_PI / 2.0;
	else if( rot.getX() < -ALLEGRO_PI / 2.0 )
		rot.m_floats[0] = -ALLEGRO_PI / 2.0;
	
	if( rot.getY() > ALLEGRO_PI * 2.0 )
		rot.m_floats[1] -= ALLEGRO_PI * 2.0;
	else if( rot.getY() < -ALLEGRO_PI * 2.0 )
		rot.m_floats[1] += ALLEGRO_PI * 2.0;
	
	if( rot.getZ() > ALLEGRO_PI * 2.0 )
		rot.m_floats[2] -= ALLEGRO_PI * 2.0;
	else if( rot.getZ() < -ALLEGRO_PI * 2.0 )
		rot.m_floats[2] += ALLEGRO_PI * 2.0;
}

void Camera::Move( btVector3 src )
{
	pos += src;
}

void Camera::Rotate( btVector3 src )
{
	rot += src;
	if( rot.getX() > ALLEGRO_PI / 2.0 )
		rot.m_floats[0] = ALLEGRO_PI / 2.0;
	else if( rot.getX() < -ALLEGRO_PI / 2.0 )
		rot.m_floats[0] = -ALLEGRO_PI / 2.0;
	
	if( rot.getY() > ALLEGRO_PI * 2.0 )
		rot.m_floats[1] -= ALLEGRO_PI * 2.0;
	else if( rot.getY() < -ALLEGRO_PI * 2.0 )
		rot.m_floats[1] += ALLEGRO_PI * 2.0;
	
	if( rot.getZ() > ALLEGRO_PI * 2.0 )
		rot.m_floats[2] -= ALLEGRO_PI * 2.0;
	else if( rot.getZ() < -ALLEGRO_PI * 2.0 )
		rot.m_floats[2] += ALLEGRO_PI * 2.0;
}

void Camera::SetWorldTransform( btTransform transform )
{
	ALLEGRO_TRANSFORM dst;
	al_identity_transform( &dst );
	
	{
		btVector3 origin = transform.getOrigin();
		btQuaternion rotation = transform.getRotation();
		btVector3 axis = rotation.getAxis();
		btScalar angle = rotation.getAngle();
		
		al_rotate_transform_3d( &dst, axis.getX(), axis.getY(), axis.getZ(), angle );
		al_translate_transform_3d( &dst, origin.getX(), origin.getY(), origin.getZ() );
	}
	
	
	
	{
		btVector3 origin = parentTransformation.getOrigin();
		btQuaternion rotation = parentTransformation.getRotation();
		btVector3 axis = rotation.getAxis();
		btScalar angle = rotation.getAngle();
		
		al_rotate_transform_3d( &dst, axis.getX(), axis.getY(), axis.getZ(), angle );
		al_translate_transform_3d( &dst, origin.getX(), origin.getY(), origin.getZ() );
	}
		
	al_translate_transform_3d( &dst, -pos.getX(), -pos.getY(), -pos.getZ() );
	al_rotate_transform_3d( &dst, 0, 1, 0, rot.getY() );
	al_rotate_transform_3d( &dst, 1, 0, 0, rot.getX() );
	al_rotate_transform_3d( &dst, 0, 0, 1, rot.getZ() );
	
	
	
	al_use_transform( &dst );
}

void Camera::SetCameraTransform( btTransform transform )
{
	parentTransformation = transform;
}

Camera::Camera()
{
	pos = btVector3(0,0,0);
	rot = btVector3(0,0,0);
	parentTransformation = btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,0,0) );
}

Camera::~Camera()
{
	pos = btVector3(0,0,0);
	rot = btVector3(0,0,0);
	parentTransformation = btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,0,0) );
}

#endif

