#pragma once

#include <windows.h>
#include "Win32.h"
#include "Log.h"
#include "GLFont.h"


class CMouse;
class CApplication
{
public:
	static CApplication* Create();
	static CApplication* GetInstance();
	static void Destroy();


public:
	virtual void Run();

	void ResizeDraw(bool enable);
	bool IsPressed(unsigned int key);

	virtual bool Init() = 0;
	virtual void UnInit() = 0;
	virtual void Update(DWORD milliseconds) = 0;
	virtual void Draw() = 0;

	CWin32& GetWin32() { return _win32; }
	CMouse& GetMouse() { return _win32.GetMouse(); }


protected:
	CApplication() {}
	virtual ~CApplication() {}

	
protected:
	CWin32 _win32;
	GLFont _font;
};