
#ifndef CAMERA_CPP
#define CAMERA_CPP

#include <Camera.h>
#include <Object.h>


#include <VBO.h>
void Camera::DrawPlanes()
{
	return;
	ALLEGRO_TRANSFORM t;
	al_identity_transform( &t );
	al_use_transform( &t );
	//SetWorldTransform( GetTransform() );
	SetWorldTransform( btTransform( btQuaternion( btVector3(1,1,1), 0 ), btVector3(0,0,0) ) );
	
	btVector3 loc = GetLocation() - (GetForwardVector()*0.5);
	btVector3 vectors[4];
	vectors[0] = ( GetForwardVector() * 1.02 ) + GetRightVector() + GetUpVector();
	vectors[1] = ( GetForwardVector() * 1.02 ) + GetRightVector() - GetUpVector();
	vectors[2] = ( GetForwardVector() * 1.02 ) - GetRightVector() - GetUpVector();
	vectors[3] = ( GetForwardVector() * 1.02 ) - GetRightVector() + GetUpVector();
	
	for( int i = 0; i < 4; ++i )
	{
		vectors[i] *= 100.0f;
		vectors[i] += loc;
	}
	
	VBO vbo;
	
	for( int i = 0; i < 4; ++i )
	{
		vbo.AddTriangle
		(
			(ALLEGRO_VERTEX){ loc.x(), loc.y(), loc.z(), 0, 0, al_map_rgba(128,128,128,128) },
			(ALLEGRO_VERTEX){ vectors[i].x(), vectors[i].y(), vectors[i].z(), 0, 0, al_map_rgba(128,128,128,128) },
			(ALLEGRO_VERTEX){ vectors[(i+1)%4].x(), vectors[(i+1)%4].y(), vectors[(i+1)%4].z(), 0, 0, al_map_rgba(128,128,128,128) }
		);
	}
	
	vbo.Generate();
	vbo.Draw();
	vbo.Destroy();
}

bool Camera::IsObjectInView( Object * object )
{
	if( object )
	{
		return true;
		float radius = object->GetRadius();
		btVector3 location = object->GetLocation();
		
//		if( normal[4].dot( currentLocation - location ) + radius < 0 )
//			return false;
		
		for( int i = 4; i >= 0; --i )
		{
			if( normal[i].dot( location - currentLocation ) + radius < 0 )
				return false;
		}
		return true;
	}
	return false;
}

void Camera::UpdateViewPlanes()		// need to use basicWindow for y/x bitmap size ration
{
	btVector3 vectors[4];
	currentLocation = GetLocation();
	
	vectors[0] = GetForwardVector() * 0.5 + GetRightVector() + GetUpVector();
	vectors[1] = GetForwardVector() * 0.5 + GetRightVector() - GetUpVector();
	vectors[2] = GetForwardVector() * 0.5 - GetRightVector() - GetUpVector();
	vectors[3] = GetForwardVector() * 0.5 - GetRightVector() + GetUpVector();
	
	for( int i = 0; i < 4; ++i )
	{
		normal[i] = vectors[i].cross( vectors[(i+1)%4] );
		normal[i].normalize();
		if( normal[i].dot( btVector3(0,0,1)/*vectors[(i+2)%4]*/ ) < 0 )
			normal[i] *= -1;
	}
	normal[4] = GetForwardVector();
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
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	
	btQuaternion rotation = parentTransformation.getRotation();
	btVector3 axis = rotation.getAxis();
	btScalar angle = rotation.getAngle();
	al_rotate_transform_3d( &transform, axis.x(), axis.y(), axis.z(), angle );
	
	al_rotate_transform_3d( &transform, 0, 1, 0, -rot.y() + ALLEGRO_PI );
	al_rotate_transform_3d( &transform, 1, 0, 0, -rot.x() );
	al_rotate_transform_3d( &transform, 0, 0, 1, rot.z() );
	al_transform_coordinates_3d( &transform, dst.m_floats, dst.m_floats+1, dst.m_floats+2 );
	return dst;
}

btVector3 Camera::GetFlatRightVector()
{
	btVector3 dst(-1,0,0);
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	
	btQuaternion rotation = parentTransformation.getRotation();
	btVector3 axis = rotation.getAxis();
	btScalar angle = rotation.getAngle();
	al_rotate_transform_3d( &transform, axis.x(), axis.y(), axis.z(), angle );
	
	al_rotate_transform_3d( &transform, 0, 1, 0, -rot.y() + ALLEGRO_PI );
	al_transform_coordinates_3d( &transform, dst.m_floats, dst.m_floats+1, dst.m_floats+2 );
	return dst;
}

btVector3 Camera::GetRightVector()
{
	btVector3 dst(-1,0,0);
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	
	btQuaternion rotation = parentTransformation.getRotation();
	btVector3 axis = rotation.getAxis();
	btScalar angle = rotation.getAngle();
	al_rotate_transform_3d( &transform, axis.x(), axis.y(), axis.z(), angle );
	
	al_rotate_transform_3d( &transform, 0, 1, 0, -rot.y() + ALLEGRO_PI );
	al_rotate_transform_3d( &transform, 1, 0, 0, -rot.x() );
	al_rotate_transform_3d( &transform, 0, 0, 1, rot.z() );
	al_transform_coordinates_3d( &transform, dst.m_floats, dst.m_floats+1, dst.m_floats+2 );
	return dst;
}

btVector3 Camera::GetFlatForwardVector()
{
	btVector3 dst(0,0,1);
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	
	btQuaternion rotation = parentTransformation.getRotation();
	btVector3 axis = rotation.getAxis();
	btScalar angle = rotation.getAngle();
	al_rotate_transform_3d( &transform, axis.x(), axis.y(), axis.z(), angle );
	
	al_rotate_transform_3d( &transform, 0, 1, 0, -rot.y() + ALLEGRO_PI );
	al_transform_coordinates_3d( &transform, dst.m_floats, dst.m_floats+1, dst.m_floats+2 );
	return dst;
}

btVector3 Camera::GetForwardVector()
{
	btVector3 dst(0,0,1);
	btVector3 a;
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	a = GetRightVector();
	
	btQuaternion rotation = parentTransformation.getRotation();
	btVector3 axis = rotation.getAxis();
	btScalar angle = -rotation.getAngle();
	
	//al_rotate_transform_3d( &transform, axis.x(), axis.y(), axis.z(), angle );
	//al_rotate_transform_3d( &transform, 0, 1, 0, angle );
	al_rotate_transform_3d( &transform, 0, 1, 0, -(rot.y()+angle) + ALLEGRO_PI );
	al_rotate_transform_3d( &transform, a.x(), a.y(), a.z(), -rot.x() );
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
	btQuaternion rotation = parentTransformation.getRotation();
	btVector3 axis = rotation.getAxis();
	btScalar angle = -rotation.getAngle();
		
	al_translate_transform_3d( &dst, -pos.x()*locationScale.x(), -pos.y()*locationScale.y(), -pos.z()*locationScale.z() );
	al_translate_transform_3d( &dst, origin.x(), origin.y(), origin.z() );
	al_rotate_transform_3d( &dst, axis.x(), axis.y(), axis.z(), angle );
	al_rotate_transform_3d( &dst, 0, 1, 0, rot.y() );
	al_rotate_transform_3d( &dst, 1, 0, 0, rot.x() );
	al_rotate_transform_3d( &dst, 0, 0, 1, rot.z() );
	
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

