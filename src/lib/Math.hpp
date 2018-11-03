
#ifndef MATH_ENGINE_HPP
#define MATH_ENGINE_HPP

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Math
{
	const float PI = 3.14159265359f;
	
	inline glm::vec3 GetVec( const btVector3 & src )
	{
		return glm::vec3( src.x(), src.y(), src.z() );
	}
	
	inline glm::mat4 GetMatrix( const btTransform & transform, const btVector3 & scale )
	{
		btVector3 origin = transform.getOrigin();
		btQuaternion rotation = transform.getRotation();
		btVector3 axis = rotation.getAxis().normalized();
		btScalar angle = rotation.getAngle();
		
		glm::mat4 matrix(1.0f);
		
		matrix = glm::scale( matrix, glm::vec3( scale.x(), scale.y(), scale.z() ) );
		matrix = glm::translate( matrix, glm::vec3( origin.x(), origin.y(), origin.z() ) );
		matrix = glm::rotate( matrix, angle, glm::vec3( axis.x(), axis.y(), axis.z() ) );
		
		return matrix;
	}
	
};

#endif

