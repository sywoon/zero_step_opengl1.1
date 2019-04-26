#include "Application.h"

PFNGLMULTITEXCOORD2FARBPROC	glMultiTexCoord2fARB = NULL;
PFNGLACTIVETEXTUREARBPROC	glActiveTextureARB = NULL;


void Application::initGLExt()
{
    //opengl1.1中 多重纹理通过扩展库方式实现
    glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
    glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
}


void Application::run()
{
	_win32.setFullScreen(false);
    while (_win32.isLooping())  //切换全屏用
    {
    	if (!_win32.init() || !init())
    		break;

    	_win32.run();
        _win32.unInit();

    	unInit();
    }
}


void Application::resizeDraw(bool enable)
{
    _win32.resizeDraw(enable);
}

bool Application::isPressed(unsigned int key)
{
    return _win32.isPressed(key);
}