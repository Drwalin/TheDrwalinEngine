
#ifndef OBJECT_CPP
#define OBJECT_CPP

#include <Object.h>
#include <Engine.h>

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
		engine->GetCamera()->SetWorldTransform( GetTransform() );
		model->Draw();
	}
	else
	{
		DrawDebug();
	}
}

void Object::DrawDebug()
{
	switch( objectType )
	{
	case BOX:
		engine->DrawBox( al_map_rgb(255,255,255), GetTransform(), btVector3(debugData[0],debugData[1],debugData[2]) );
		break;
	case BALL:
		engine->DrawBall( al_map_rgb(255,255,255), GetTransform(), debugData[0] );
		break;
	case CAPSULE:
	case CYLINDER:
	case CUSTOM:
		std::cerr << "\n Can not yet draw different debug objects tha boxes and balls\n this->name = " << name;
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
}

Object::Object()
{
	engine = NULL;
	name = "";
	body = NULL;
	model = NULL;
	objectType = CUSTOM;
}

Object::~Object()
{
	engine = NULL;
	name = "";
	body = NULL;
	model = NULL;
	objectType = 0;
}

#endif

