#include "Application.h"
#include <assert.h>
#include <gl.h>
#include <glu.h>


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")


void CApplication::Run()
{
	_win32.SetFullScreen(false);
    while (_win32.IsLooping())  //切换全屏用
    {
    	if (!_win32.Init() || !Init())
    		break;

    	_win32.Run();
        _win32.UnInit();

    	UnInit();
    }
}


void CApplication::ResizeDraw(bool enable)
{
    _win32.ResizeDraw(enable);
}

bool CApplication::IsPressed(unsigned int key)
{
    return _win32.IsPressed(key);
}