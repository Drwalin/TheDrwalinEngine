
#ifndef OBJECT_CPP
#define OBJECT_CPP

#include <Object.h>
#include <Engine.h>

void Object::SetScale( btVector3 scale )
{
	this->scale = scale;
	if( body )
	{
		body->getCollisionShape()->setLocalScaling( scale );
		engine->GetWorld()->UpdateColliderForObject( body );
	}
}

btVector3 Object::GetScale()
{
	return scale;
}

btTransform Object::GetTransform()
{
	btTransform transform;
	if( body )
		body->getMotionState()->getWorldTransform( transform );
	return transform;
}

btRigidBody * Object::GetBody()
{
	return body;
}

void Object::Draw()
{
	if( model )
	{
		engine->GetCamera()->SetWorldTransform( GetTransform(), scale );
		model->Draw();
	}
	else
	{
		DrawDebug();
	}
}

void Object::DrawDebug()
{
	Model * mdl;
	switch( objectType )
	{
	case BOX:
		engine->DrawBox( al_map_rgb(255,255,255), GetTransform(), btVector3(debugData[0],debugData[1],debugData[2])*scale );
		break;
	case BALL:
		mdl = engine->GetModel( "Sphere" );
		if( mdl )
		{
			engine->GetCamera()->SetWorldTransform( GetTransform(), scale );
			mdl->Draw();
		}
		else
		{
			engine->DrawBall( al_map_rgb(255,255,255), GetTransform(), debugData[0]*scale.length() );
		}
		break;
	case CAPSULE:
	case CYLINDER:
	case CUSTOM:
		//std::cerr << "\n Can not yet draw different debug objects than boxes and balls\n this->name = " << name;
		break;
	}
}

void Object::SetModel( Model * model )
{
	this->model = model;
}

Object::Object( Engine * engine, std::string name, btRigidBody * body, std::vector < float > debugData, int type )
{
	objectType = type;
	this->debugData = debugData;
	this->engine = engine;
	this->name = name;
	this->body = body;
	model = NULL;
	scale = btVector3(1,1,1);
}

Object::Object()
{
	engine = NULL;
	name = "";
	body = NULL;
	model = NULL;
	objectType = CUSTOM;
	scale = btVector3(1,1,1);
}

Object::~Object()
{
	engine = NULL;
	name = "";
	body = NULL;
	model = NULL;
	objectType = 0;
	scale = btVector3(0,0,0);
}

#endif

