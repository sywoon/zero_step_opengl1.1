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

/** �����ڴ�С�ı�ʱ��֪ͨOpenGL������С */
void Window::ReshapeGL()
{
	GLsizei width = GetWidth();
	GLsizei height = GetHeight();
	glViewport(0, 0, width, height);	/**< ���������ӿ� */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/** �ı䴰�ڵ���ʾ���� */
bool Window::ChangeScreenSetting()
{
	DEVMODE dmScreenSettings;											/**< �豸ģʽ */
	ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));						/**< ����ṹ */
	dmScreenSettings.dmSize			= sizeof(DEVMODE);					/**< �ṹ��С */
	dmScreenSettings.dmPelsWidth	= GetWidth();						/**< ���ÿ�� */
	dmScreenSettings.dmPelsHeight	= GetHeight();						/**< ���ø߶� */
	dmScreenSettings.dmBitsPerPel	= m_bitsPerPixel;					/**< ����λ��� */
	//dmScreenSettings.dmDisplayFrequency = 75;                           /**< ������Ļˢ���� */
	dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;//| DM_DISPLAYFREQUENCY;

	/// �ı���ʾģʽ
	if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		return false;

	return true;
}

bool Window::Create(const char * window_title,const char * class_name, HINSTANCE h_instance, LPVOID lpParam)
{
	int nX=0;
	int nY=0;
	PIXELFORMATDESCRIPTOR pfd =							/**< �������������ṹ */
	{
		sizeof(PIXELFORMATDESCRIPTOR),					/**< ���������ṹ�Ĵ�С */ 
		1,												/**< �汾�� */
		PFD_DRAW_TO_WINDOW	|							/**< �������������ʾ��һ�������� */
		PFD_SUPPORT_OPENGL	|							/**< ������֧��OpenGL��ͼ */
		PFD_STEREO			|							/**< ��ɫ�����������建�� */
		PFD_DOUBLEBUFFER,								/**< ��ɫ��������˫���� */
		PFD_TYPE_RGBA,									/**< ʹ��RGBA��ɫ��ʽ */
		m_bitsPerPixel,									/**< ��ɫ����������ɫֵ��ռ��λ�� */
		0, 0, 0, 0, 0, 0,								/**< ʹ��Ĭ�ϵ���ɫ���� */
		0,												/**< ��Alpha���� */
		0,												/**< ��ɫ��������alpha�ɷֵ���λ���� */
		0,												/**< ���ۼƻ����� */
		0, 0, 0, 0,										/**< �ۼƻ���������λ */
		32,												/**< 32λ��Ȼ��� */
		0,												/**< ���ɰ滺�� */
		0,												/**< �޸��������� */
		PFD_MAIN_PLANE,									/**< ����ΪPFD_MAIN_PLANE������Ϊ����ͼ�� */
		0,												/**< ��ʾOpenGLʵ����֧�ֵ��ϲ���²�ƽ������� */
		0, 0, 0											/**< ��ʱ���Ѳ���ʹ�� */
	};

	DWORD windowStyle = WS_TILEDWINDOW;					/**< �������Ǵ������ͣ�ʹ�ó����趨 */
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						

	if (m_isFullScreen)									/**< ���Ϊȫ��ģʽ������ת��Ϊȫ��ģʽ */
	{
		if (!ChangeScreenSetting())
		{																/**< ȫ��ģʽת��ʧ�ܣ������Ի�����ʾ�������Դ���ģʽ */
			MessageBox(HWND_DESKTOP, "ģʽת��ʧ��.\n�ڴ���ģʽ������.", "Error", MB_OK | MB_ICONEXCLAMATION);
			m_isFullScreen = false;										/**< ����Ϊ����ģʽ */
		}
		else															/**< ���Ϊ����ģʽ */
		{
			ShowCursor(false);											/**< ������� */
			windowStyle = WS_POPUP;										/**< ���ô���ģʽΪ���㴰�� */
			windowExtendedStyle |= WS_EX_TOPMOST;						
		}																
	}

	/// �������Ǵ��ڵĴ�С��ʹ��ͻ����Ĵ�СΪ�������õĴ�С
	RECT windowRect = {GetPosX(), GetPosY(), GetPosX() + GetWidth(), GetPosY() + GetHeight()};
	if (!m_isFullScreen)										/**< �ڴ���ģʽ��ʹ�� */
	{	
		windowExtendedStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	/**< ʹ���ھ���3D��� */
		int wid = GetSystemMetrics(SM_CXSCREEN);		/**< ��ȡ��ǰ��Ļ�� */
		int hei = GetSystemMetrics(SM_CYSCREEN);		/**< ��ȡ��ǰ��Ļ�� */
		nX = (wid - GetWidth()) / 2;                    /**< ���㴰�ھ����� */
		nY = (hei - GetHeight()) / 2;			
		/// �������Ǵ��ڵĴ�С��ʹ��ͻ����Ĵ�СΪ�������õĴ�С
		AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendedStyle);
		/// �жϴ��ڵ����Ͻ��Ƿ�������������
		if (windowRect.left < 0)										/**< �������X����Ϊ�����ƶ����굽0�������������ڵ�λ�� */
		{
			windowRect.right -= windowRect.left;						
			windowRect.left = 0;										
		}
		if (windowRect.top < 0)											/**< �������Y����Ϊ�����ƶ����굽0�������������ڵ�λ�� */
		{
			windowRect.bottom -= windowRect.top;						
			windowRect.top = 0;											
		}
	}

	/// ��������
	m_hWnd = CreateWindowEx(windowExtendedStyle,						/**< ���ڵ���չ��� */
							class_name,									/**< ���ڵ����� */
							window_title,								/**< ���ڱ��� */
							windowStyle,								/**< ���ڵķ�� */
							nX,nY,                                      /**< ���ڵ����Ͻ�λ�� */
							windowRect.right - windowRect.left,			/**< ���ڵĿ�� */
							windowRect.bottom - windowRect.top,			/**< ���ڵĸ߶� */
                            HWND_DESKTOP,								/**< ���ڵĸ�����Ϊ���� */
							0,											/**< �޲˵� */
							h_instance,									/**< ���봰�ڵ�ʵ����� */
							lpParam);									/**< ������������ */

	while (m_hWnd != 0)													/**< �����Ƿ񴴽��ɹ� */
	{
		m_hDC = GetDC(m_hWnd);											/**< ���ش��ڵ��豸������ */
		if (m_hDC == 0)													/**< ���Ϊ�� */
			break;														

		GLuint pixelFormat = ChoosePixelFormat(m_hDC, &pfd);			/**< ����һ�����ݵ����ظ�ʽ */
		if (pixelFormat == 0)											/**< ���û�ҵ� */
			break;														

		if (!SetPixelFormat(m_hDC, pixelFormat, &pfd))					/**< �������ظ�ʽ */
			break;														

		m_hRC = wglCreateContext(m_hDC);								/**< ����OpenGL����Ⱦ������ */
		if (m_hRC == 0)													/**< ���Ϊ�� */
			break;														

		if (!wglMakeCurrent(m_hDC, m_hRC))								/**< ���õ�ǰ��OpenGL����Ⱦ����Ϊ��ǰ�Ĵ��� */
			break;														
        
		ShowWindow(m_hWnd, SW_NORMAL);									/**< ��ʾ���� */
		ReshapeGL();													/**< ����OpenGL�������ڴ�С */
		return true;													/**< �ɹ����� */
	}																	

	Destroy();															/**< �ͷ���Դ */
	return false;														/**< ����ʧ�� */
}

/** ɾ��OpenGL���� */
void Window::Destroy()									
{
	if (m_hWnd != 0)										/**< ���ھ���Ƿ���� */
	{
		if (m_hDC != 0)										/**< �����豸�������Ƿ���� */
		{
			wglMakeCurrent(m_hDC, 0);						/**< ���õ�ǰ��OpenGL����Ⱦ����Ϊ��NULL */
			if (m_hRC != 0)									/**< OpenGL����Ⱦ�������Ƿ���� */
			{
				wglDeleteContext(m_hRC);					/**< �ͷ�OpenGL����Ⱦ������ */
				m_hRC = 0;									/**< ����OpenGL����Ⱦ������Ϊ0 */
			}
			ReleaseDC(m_hWnd, m_hDC);						/**< �ͷŴ��ڵ��豸������ */
			m_hDC = 0;										/**< ���ô��ڵ��豸������Ϊ0 */
		}
		DestroyWindow(m_hWnd);								/**< ɾ������ */
		m_hWnd = 0;											/**< ���ô��ھ��Ϊ0 */
	}

	if (m_isFullScreen)										/**< ���Ϊȫ��ģʽ���ڳ�������󣬱任������ģʽ������ʾ��� */
	{
		ChangeDisplaySettings(NULL, 0);						/**< �任������ģʽ */
		ShowCursor(true);									/**< ��ʾ��� */
	}
}
