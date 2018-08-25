
#ifndef COLLISION_SHAPE_MANAGER_CPP
#define COLLISION_SHAPE_MANAGER_CPP

#include <CollisionShapeManager.h>
#include <Model.h>
#include <Object.h>

bool CollisionShapeManager::IsNameAvailable( std::string name )
{
	if( name == "" )
		return false;
	return bool( customCollisionShape.find( name ) == customCollisionShape.end() );
}

std::string CollisionShapeManager::GetFirstAvailableName( std::string name )
{
	if( IsNameAvailable( name ) )
		return name;
	for( int i = 0;; ++i )
	{
		if( IsNameAvailable( name + std::to_string(i) ) )
			return name + std::to_string(i);
	}
	return name;
}

btCollisionShape * CollisionShapeManager::GetShape( std::vector < btScalar > constructionData, bool independent )
{
	auto it = collisionShape.find( constructionData );
	if( !independent && it != collisionShape.end() )
	{
		return it->second;
	}
	else
	{
		btCollisionShape * shape = NULL;
		switch( constructionData.size() )
		{
		case 1:
			shape = new btSphereShape( constructionData[0] );
			break;
		case 2:
			shape = new btCapsuleShape( constructionData[0], constructionData[1]/2.0 );
			break;
		case 3:
			shape = new btBoxShape( btVector3( constructionData[0], constructionData[1], constructionData[2] ) );
			break;
		case 4:
			shape = new btCylinderShape( btVector3( constructionData[0], constructionData[1]/2.0, constructionData[2] ) );
			break;
		}
		
		if( !independent && shape )
		{
			collisionShape[ constructionData ] = shape;
			collisionShapeRev[ shape ] = constructionData;
		}
		return shape;
	}
	return NULL;
}

btCollisionShape * CollisionShapeManager::AddShape( std::vector < btScalar > constructionData, std::string name )
{
	btCollisionShape * shape = NULL;
	if( name == "" )
	{
		shape = GetShape( constructionData, false );
		
		if( shape )
		{
			if( numberOfReferencesToShape.find( shape ) == numberOfReferencesToShape.end() )
			{
				numberOfReferencesToShape[shape] = 1;
			}
			else
			{
				numberOfReferencesToShape[shape] += 1;
			}
DEBUG( std::string("shape references: ")+std::to_string(numberOfReferencesToShape[ shape ]) )
		}
	}
	else if( IsNameAvailable( name ) )
	{
		shape = GetShape( constructionData, true );
		if( shape )
		{
			customCollisionShape[ name ] = shape;
			customCollisionShapeName[ shape ] = name;
		}
	}
	return shape;
}

btCollisionShape * CollisionShapeManager::GetBox( btVector3 size, std::string name )
{
	std::vector < btScalar > constructionData = { size.x(), size.y(), size.z() };
	return AddShape( constructionData, name );
}

btCollisionShape * CollisionShapeManager::GetBall( btScalar radius, std::string name )
{
	std::vector < btScalar > constructionData = { radius };
	return AddShape( constructionData, name );
}

btCollisionShape * CollisionShapeManager::GetCapsule( btScalar radius, btScalar height, std::string name )
{
	std::vector < btScalar > constructionData = { radius, height };
	return AddShape( constructionData, name );
}

btCollisionShape * CollisionShapeManager::GetCylinder( btScalar radius, btScalar height, std::string name )
{
	std::vector < btScalar > constructionData = { radius, height, radius, height };
	return AddShape( constructionData, name );
}

btCollisionShape * CollisionShapeManager::CreateCustomShape( std::string name, Model * model, int shapeType )
{
	if( model == NULL || IsNameAvailable( name ) == false )
		return NULL;
	
	CustomCollisionShapeData * data = model->GetCustomCollisionShapeData();
	
	if( data == NULL )
	{
		return NULL;
	}
	
	modelPointerCustomCollisionData[ data ] = model;
	
	btCollisionShape * shape = NULL;
	
	switch( shapeType )
	{
	case SHAPE_TYPE_CONVEX:
		shape = data->GetConvexShape();
		break;
	case SHAPE_TYPE_TRIANGLE:
		shape = data->GetTriangleShape();
		break;
	default:
		return NULL;
	}
	
	if( shape )
	{
		modelCustomCollisionData[ data ] += 1;
		customCollisionShape[ name ] = shape;
		customCollisionShapeData[ shape ] = data;
		customCollisionShapeName[ shape ] = name;
	}
	
	return shape;
}

btCollisionShape * CollisionShapeManager::GetCustomShape( std::string name )
{
	auto it = customCollisionShape.find( name );
	if( it != customCollisionShape.end() )
	{
		auto it2 = customCollisionShapeData.find( it->second );
		modelCustomCollisionData[it2->second] += 1;
DEBUG( std::string("shape references: ")+std::to_string(modelCustomCollisionData[it2->second]) )
		return it->second;
	}
	return NULL;
}

void CollisionShapeManager::RemoveCustomShape( std::string name )
{
	auto it = customCollisionShape.find( name );
	if( it != customCollisionShape.end() )
	{
		auto it2 = customCollisionShapeData.find( it->second );
		auto it3 = modelCustomCollisionData.find( it2->second );
		
		if( it3 != modelCustomCollisionData.end() )
		{
			it3->second -= 1;
			
DEBUG( std::string("shape references: ")+std::to_string(it3->second) )
			
			if( it3->second <= 0 )
			{
				modelPointerCustomCollisionData[ it2->second ]->NullCustomCollisionShape();
				modelPointerCustomCollisionData.erase( it2->second );
				assert( it2->second != NULL );
				delete it2->second;
				it2->second = NULL;
				modelCustomCollisionData.erase( it3 );
			}
			
			customCollisionShapeName.erase( it->second );
			customCollisionShapeData.erase( it2 );
			it->second = NULL;
			customCollisionShape.erase( it );
		}
		else
		{
			customCollisionShapeName.erase( it->second );
			assert( it->second != NULL );
			delete it->second;
			it->second = NULL;
			customCollisionShape.erase( it );
		}
	}
}

void CollisionShapeManager::RemoveShape( btCollisionShape * shape )
{
	{
		auto it = collisionShapeRev.find( shape );
		if( it != collisionShapeRev.end() )
		{
			numberOfReferencesToShape[ it->first ] -= 1;
			
DEBUG( std::string("shape references: ")+std::to_string(numberOfReferencesToShape[ it->first ]) )
			if( numberOfReferencesToShape[ it->first ] <= 0 )
			{
				collisionShape.erase( it->second );
				numberOfReferencesToShape.erase( it->first );
				assert( it->first != NULL );
				delete it->first;
//				it->first = NULL;/////////////////////////////////////////////////////////////////////////////
				collisionShapeRev.erase( it );
			}
			return;
		}
	}
	
	auto it = customCollisionShapeName.find( shape );
	if( it != customCollisionShapeName.end() )
	{
		RemoveCustomShape( it->second );
	}
}

void DebugBreakPointFunc()
{
	return;
}

void CollisionShapeManager::Destroy()
{
	DebugBreakPointFunc();
	
	for( auto it = numberOfReferencesToShape.begin(); it != numberOfReferencesToShape.end(); ++it )
	{
		assert( it->first != NULL );
		delete it->first;
	}
	
	/*
	for( auto it = customCollisionShape.begin(); it != customCollisionShape.end(); ++it )
	{
		assert( it->second != NULL );
		delete it->second;
	}
	*/
	
	for( auto it = modelPointerCustomCollisionData.begin(); it != modelPointerCustomCollisionData.end(); ++it )
	{
		assert( it->first != NULL );
		delete it->first;
	}
	
	collisionShape.clear();
	collisionShapeRev.clear();
	numberOfReferencesToShape.clear();
	
	modelPointerCustomCollisionData.clear();
	modelCustomCollisionData.clear();
	customCollisionShapeData.clear();
	customCollisionShape.clear();
	customCollisionShapeName.clear();
}

CollisionShapeManager::CollisionShapeManager()
{
}

CollisionShapeManager::~CollisionShapeManager()
{
	Destroy();
}

#endif

