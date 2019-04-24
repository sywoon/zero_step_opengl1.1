#pragma once

#include "Base.h"
#include "Win32.h"
#include "Camera.h"
#include "GLFont.h"
#include "SkyBox.h"
#include "Terrain.h"


class Mouse;
class Application
{
public:
	static Application* create();
	static Application* getInstance();
	static void destroy();

public:
	virtual bool init() = 0;
	virtual void unInit() = 0;
	virtual void update(DWORD milliseconds) = 0;
	virtual void draw() = 0;

	virtual void run();

	Win32& 	getWin32()  { return _win32; }
	Mouse&  getMouse()  { return _win32.getMouse(); }
	Camera& getCamera() { return _camera; }

	void resizeDraw(bool enable);
	bool isPressed(unsigned int key);

protected:
	Application() {}
	virtual ~Application() {}

	void initGLExt();
	
	
protected:
	Win32 	 _win32;
	Camera 	 _camera; 
	GLFont 	 _font;
	SkyBox   _skyBox;
	Terrain  _terrain;
};