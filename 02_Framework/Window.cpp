#include "Window.h"

#include <gl.h>
#include <glu.h>												


Window::Window()													
{
	m_windowPosX	= 0;
	m_windowPosY	= 0;
	m_windowWidth	= 640;
	m_windowHeight	= 480;
	m_screenWidth	= 1024;
	m_screenHeight	= 768;
	m_bitsPerPixel	= 16;
	m_isFullScreen	= false;

	m_hWnd = 0;
	m_hDC = 0;
	m_hRC = 0;
}

int Window::GetWidth()
{
	if (m_isFullScreen)
	{
		return m_screenWidth;
	}
	else
	{
		return m_windowWidth;
	}
}
int Window::GetHeight()
{
	if (m_isFullScreen)
	{
		return m_screenHeight;
	}
	else
	{
		return m_windowHeight;
	}
}

void Window::SetWidth(int width)
{
	if (m_isFullScreen)
	{
		m_screenWidth = width;
	}
	else
	{
		m_windowWidth = width;
	}
}
void Window::SetHeight(int height)
{
	if (m_isFullScreen)
	{
		m_screenHeight = height;
	}
	else
	{
		m_windowHeight = height;
	}
}

int Window::GetPosX()
{
	if (m_isFullScreen)
	{
		return m_windowPosX;
	}
	return 0;
}
int Window::GetPosY()
{
	if (m_isFullScreen)
	{
		return m_windowPosY;
	}
	return 0;
}

void Window::SetPosX(int x)
{
	if (m_isFullScreen)
	{
		m_windowPosX = x;
	}
}
void Window::SetPosY(int y)
{
	if (m_isFullScreen)
	{
		m_windowPosY = y;
	}
}

/** 当窗口大小改变时，通知OpenGL调整大小 */
void Window::ReshapeGL()
{
	GLsizei width = GetWidth();
	GLsizei height = GetHeight();
	glViewport(0, 0, width, height);	/**< 重新设置视口 */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/** 改变窗口的显示设置 */
bool Window::ChangeScreenSetting()
{
	DEVMODE dmScreenSettings;											/**< 设备模式 */
	ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));						/**< 清零结构 */
	dmScreenSettings.dmSize			= sizeof(DEVMODE);					/**< 结构大小 */
	dmScreenSettings.dmPelsWidth	= GetWidth();						/**< 设置宽度 */
	dmScreenSettings.dmPelsHeight	= GetHeight();						/**< 设置高度 */
	dmScreenSettings.dmBitsPerPel	= m_bitsPerPixel;					/**< 设置位深度 */
	//dmScreenSettings.dmDisplayFrequency = 75;                           /**< 设置屏幕刷新率 */
	dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;//| DM_DISPLAYFREQUENCY;

	/// 改变显示模式
	if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		return false;

	return true;
}

bool Window::Create(const char * window_title,const char * class_name, HINSTANCE h_instance, LPVOID lpParam)
{
	int nX=0;
	int nY=0;
	PIXELFORMATDESCRIPTOR pfd =							/**< 设置像素描述结构 */
	{
		sizeof(PIXELFORMATDESCRIPTOR),					/**< 像素描述结构的大小 */ 
		1,												/**< 版本号 */
		PFD_DRAW_TO_WINDOW	|							/**< 缓存区的输出显示在一个窗口中 */
		PFD_SUPPORT_OPENGL	|							/**< 缓存区支持OpenGL绘图 */
		PFD_STEREO			|							/**< 颜色缓存区是立体缓存 */
		PFD_DOUBLEBUFFER,								/**< 颜色缓存区是双缓存 */
		PFD_TYPE_RGBA,									/**< 使用RGBA颜色格式 */
		m_bitsPerPixel,									/**< 颜色缓存区中颜色值所占的位深 */
		0, 0, 0, 0, 0, 0,								/**< 使用默认的颜色设置 */
		0,												/**< 无Alpha缓存 */
		0,												/**< 颜色缓存区中alpha成分的移位计数 */
		0,												/**< 无累计缓存区 */
		0, 0, 0, 0,										/**< 累计缓存区无移位 */
		32,												/**< 32位深度缓存 */
		0,												/**< 无蒙版缓存 */
		0,												/**< 无辅助缓存区 */
		PFD_MAIN_PLANE,									/**< 必须为PFD_MAIN_PLANE，设置为主绘图层 */
		0,												/**< 表示OpenGL实现所支持的上层或下层平面的数量 */
		0, 0, 0											/**< 过时，已不再使用 */
	};

	DWORD windowStyle = WS_TILEDWINDOW;					/**< 定义我们窗口类型，使用常规设定 */
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						

	if (m_isFullScreen)									/**< 如果为全屏模式，尝试转化为全屏模式 */
	{
		if (!ChangeScreenSetting())
		{																/**< 全屏模式转换失败，弹出对话框提示，并尝试窗口模式 */
			MessageBox(HWND_DESKTOP, "模式转换失败.\n在窗口模式下运行.", "Error", MB_OK | MB_ICONEXCLAMATION);
			m_isFullScreen = false;										/**< 设置为窗口模式 */
		}
		else															/**< 如果为窗口模式 */
		{
			ShowCursor(false);											/**< 隐藏鼠标 */
			windowStyle = WS_POPUP;										/**< 设置窗口模式为顶层窗口 */
			windowExtendedStyle |= WS_EX_TOPMOST;						
		}																
	}

	/// 调整我们窗口的大小，使其客户区的大小为我们设置的大小
	RECT windowRect = {GetPosX(), GetPosY(), GetPosX() + GetWidth(), GetPosY() + GetHeight()};
	if (!m_isFullScreen)										/**< 在窗口模式下使用 */
	{	
		windowExtendedStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	/**< 使窗口具有3D外观 */
		int wid = GetSystemMetrics(SM_CXSCREEN);		/**< 获取当前屏幕宽 */
		int hei = GetSystemMetrics(SM_CYSCREEN);		/**< 获取当前屏幕高 */
		nX = (wid - GetWidth()) / 2;                    /**< 计算窗口居中用 */
		nY = (hei - GetHeight()) / 2;			
		/// 调整我们窗口的大小，使其客户区的大小为我们设置的大小
		AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendedStyle);
		/// 判断窗口的左上角是否隐藏在桌面外
		if (windowRect.left < 0)										/**< 如果窗口X坐标为负，移动坐标到0处，并调整窗口的位置 */
		{
			windowRect.right -= windowRect.left;						
			windowRect.left = 0;										
		}
		if (windowRect.top < 0)											/**< 如果窗口Y坐标为负，移动坐标到0处，并调整窗口的位置 */
		{
			windowRect.bottom -= windowRect.top;						
			windowRect.top = 0;											
		}
	}

	/// 创建窗口
	m_hWnd = CreateWindowEx(windowExtendedStyle,						/**< 窗口的扩展风格 */
							class_name,									/**< 窗口的类名 */
							window_title,								/**< 窗口标题 */
							windowStyle,								/**< 窗口的风格 */
							nX,nY,                                      /**< 窗口的左上角位置 */
							windowRect.right - windowRect.left,			/**< 窗口的宽度 */
							windowRect.bottom - windowRect.top,			/**< 窗口的高度 */
                            HWND_DESKTOP,								/**< 窗口的父窗口为桌面 */
							0,											/**< 无菜单 */
							h_instance,									/**< 传入窗口的实例句柄 */
							lpParam);									/**< 传入程序类参数 */

	while (m_hWnd != 0)													/**< 窗口是否创建成功 */
	{
		m_hDC = GetDC(m_hWnd);											/**< 返回窗口的设备描述表 */
		if (m_hDC == 0)													/**< 如果为空 */
			break;														

		GLuint pixelFormat = ChoosePixelFormat(m_hDC, &pfd);			/**< 查找一个兼容的像素格式 */
		if (pixelFormat == 0)											/**< 如果没找到 */
			break;														

		if (!SetPixelFormat(m_hDC, pixelFormat, &pfd))					/**< 设置像素格式 */
			break;														

		m_hRC = wglCreateContext(m_hDC);								/**< 创建OpenGL的渲染描述表 */
		if (m_hRC == 0)													/**< 如果为空 */
			break;														

		if (!wglMakeCurrent(m_hDC, m_hRC))								/**< 设置当前的OpenGL的渲染对象为当前的窗口 */
			break;														
        
		ShowWindow(m_hWnd, SW_NORMAL);									/**< 显示窗口 */
		ReshapeGL();													/**< 告诉OpenGL调整窗口大小 */
		return true;													/**< 成功返回 */
	}																	

	Destroy();															/**< 释放资源 */
	return false;														/**< 返回失败 */
}

/** 删除OpenGL窗口 */
void Window::Destroy()									
{
	if (m_hWnd != 0)										/**< 窗口句柄是否存在 */
	{
		if (m_hDC != 0)										/**< 窗口设备描述表是否存在 */
		{
			wglMakeCurrent(m_hDC, 0);						/**< 设置当前的OpenGL的渲染对象为空NULL */
			if (m_hRC != 0)									/**< OpenGL的渲染描述表是否存在 */
			{
				wglDeleteContext(m_hRC);					/**< 释放OpenGL的渲染描述表 */
				m_hRC = 0;									/**< 设置OpenGL的渲染描述表为0 */
			}
			ReleaseDC(m_hWnd, m_hDC);						/**< 释放窗口的设备描述表 */
			m_hDC = 0;										/**< 设置窗口的设备描述表为0 */
		}
		DestroyWindow(m_hWnd);								/**< 删除窗口 */
		m_hWnd = 0;											/**< 设置窗口句柄为0 */
	}

	if (m_isFullScreen)										/**< 如果为全屏模式，在程序结束后，变换到窗口模式，并显示鼠标 */
	{
		ChangeDisplaySettings(NULL, 0);						/**< 变换到窗口模式 */
		ShowCursor(true);									/**< 显示鼠标 */
	}
}
