
#ifndef MAIN_H
#define MAIN_H

#include <Camera.h>
#include <TextPrinter.h>
#include <BasicWindow.h>
#include <Debug.h>
#include <EventResponser.h>

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>

#include <string>

#include <memory>

class Window : public BasicWindow
{
private:
	
	class Engine * engine;
	
public:
	
	std::shared_ptr<Camera> camera;
	TextPrinter * output;
	
	void Init( Engine * engine, const char * windowName, const char * iconFile, int width, int height, EventResponser * eventResponser, bool fullscreen = false );
	
	virtual void ParallelToDrawTick( const float deltaTime ) override;
	
	virtual void Tick( const float deltaTime ) override;
	float GetSmoothFps();
	
	void Destroy();
	
	Window();
	~Window();
};

#endif

