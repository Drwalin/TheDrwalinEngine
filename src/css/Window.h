
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

#include <SmartPtr.h>

class Window : public BasicWindow
{
private:
	
	class Engine * engine;
	
public:
	
	SmartPtr<Camera> camera;
	TextPrinter * output;
	
	void Init( Engine * engine, const char * windowName, const char * iconFile, int width, int height, bool fullscreen = false );
	
	virtual void ParallelToDrawTick( const float deltaTime ) override;
	
	virtual void Tick( const float deltaTime ) override;
	virtual void Draw3D() override;
	float GetSmoothFps();
	virtual void Draw2D() override;
	
	void Destroy();
	
	Window();
	~Window();
};

#endif

