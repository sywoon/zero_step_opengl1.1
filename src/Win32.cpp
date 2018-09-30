#include "Win32.h"
#include "Application.h"
#include "Win32.h"
#include "Log.h"

#include <assert.h>
#include <stdio.h>



static const UINT WM_TOGGLE_FULLSCREEN = (WM_USER + 1);
static CWin32* s_pWin32 = NULL;
const char* _pszClassName = "OpenGL";
const char* _pszWinTitle = "GLFrame";


static LRESULT CALLBACK _WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (s_pWin32 && s_pWin32->GetHWnd() == hWnd)
    {
        return s_pWin32->WindowProc(uMsg, wParam, lParam);
    }
    else
    {
    	switch (uMsg)
		{
		case WM_CREATE:  //��ʱCWin32��û�ӹ�
			{																
				s_pWin32->SetVisible(true);
				return 0;
			}
		default:
        	return DefWindowProc(hWnd, uMsg, wParam, lParam);
    	}
    }
}


CWin32* CWin32::Create()
{
	if (s_pWin32)
		return s_pWin32;

	s_pWin32 = new CWin32();
	return s_pWin32;
}

void CWin32::Destroy()
{
	if (s_pWin32)
	{
		delete s_pWin32;
		s_pWin32 = NULL;
	}
}

CWin32::CWin32()
		: _hWnd(NULL)
		, _hInstance(NULL)
		, _hDC(NULL)
		, _hRC(NULL)
		, _bFullScreen(false)
		, _resizeDraw(false)
		, _isVisible(false)
		, _isLooping(true)
		, _nWinWidth(960)
		, _nWinHeight(640)
		, _dwLastTickCount(0)
{
	assert(s_pWin32 == NULL);
	s_pWin32 = this;

	_nSrnWidth = GetSystemMetrics(SM_CXSCREEN);
	_nSrnHeight = GetSystemMetrics(SM_CYSCREEN);

	_hInstance = GetModuleHandle(NULL);
}

CWin32::~CWin32()
{
}

bool CWin32::Init()
{
	if (!RegisterMyClass())
    	return false;

    HWND hWnd = CreateMyWindow(NULL);
	if (NULL == hWnd)
		return false;

	_hWnd = hWnd;

	HGLRC hRC;
	if (!InitOpenGL(hWnd, hRC))
	{
		DestroyWin(hWnd, hRC);
		return false;
	}
	_hRC = hRC;

	return true;
}

void CWin32::Run()
{
	DealWinMessage(_hWnd);
	DestroyWin(_hWnd, _hRC);
	UnRegisterMyClass();
}

bool CWin32::RegisterMyClass()
{
	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

	windowClass.cbSize			= sizeof(WNDCLASSEX);
	//�ƶ�ʱ�ػ�����Ϊ����ȡ��DC
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc		= (WNDPROC)(_WindowProc);
	windowClass.hInstance		= _hInstance;
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName	= _pszClassName;

	if (RegisterClassEx(&windowClass) == 0)
	{
		MessageBox(HWND_DESKTOP, "���ڴ���ʧ��!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

void CWin32::UnRegisterMyClass()
{
	UnregisterClass(_pszClassName, _hInstance);
}

HWND CWin32::CreateMyWindow(LPVOID lpParam)
{
	DWORD windowStyle = WS_TILEDWINDOW;
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						

	if (_bFullScreen)  //���Ϊȫ��ģʽ������ת��Ϊȫ��ģʽ
	{
		//ȫ��ģʽת��ʧ�ܣ������Ի�����ʾ�������Դ���ģʽ
		if (!ChangeScreenSetting())
		{	
			MessageBox(HWND_DESKTOP, "ģʽת��ʧ��.\n�ڴ���ģʽ������.", "Error", MB_OK | MB_ICONEXCLAMATION);
			_bFullScreen = false;
		}
		else
		{
			// ShowCursor(false);

			//���ô���ģʽΪ���㴰��
			windowStyle = WS_POPUP;  
			windowExtendedStyle |= WS_EX_TOPMOST;
		}																
	}

	/// �������Ǵ��ڵĴ�С��ʹ��ͻ����Ĵ�СΪ�������õĴ�С
	int nX = 0;
	int nY = 0;
	RECT windowRect;  //�������ڴ�С ��ȫ���� ���ڿͻ���
	if (_bFullScreen)
	{
		windowRect.left = 0;
		windowRect.top = 0;
		windowRect.right = GetWidth();
		windowRect.bottom = GetHeight();
	}
	else
	{
		nX = (_nSrnWidth - _nWinWidth) / 2;
		nY = (_nSrnHeight - _nWinHeight) / 2;

		//ʹ���ھ���3D���
		windowRect.left = 0;
		windowRect.top = 0;
		windowRect.right = GetWidth();
		windowRect.bottom = GetHeight();

		//ʹ���ھ���3D���
		windowExtendedStyle |=  WS_EX_WINDOWEDGE;
		// [0,960,0,640]  ת��ǰ
		AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendedStyle);
		// [-8,968,-31,648]  ת����
	}

	Log("win size:%d,%d,", windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);
	HWND hWnd = CreateWindowEx(windowExtendedStyle,						// ���ڵ���չ��� */
							_pszClassName,								// ���ڵ����� */
							_pszWinTitle,								// ���ڱ��� */
							windowStyle,								// ���ڵķ�� */
							nX,nY,                                      // ���ڵ����Ͻ�λ�� */
							windowRect.right - windowRect.left,			// ���ڵĿ�� */
							windowRect.bottom - windowRect.top,			// ���ڵĸ߶� */
                            HWND_DESKTOP,								// ���ڵĸ�����Ϊ���� */
							0,											// �޲˵� */
							_hInstance,									// ���봰�ڵ�ʵ����� */
							lpParam);									// paramָ��

	if (NULL == hWnd)
	{
		_isLooping = false;
		char szError[255] = {0};
		sprintf(szError, "���ڴ���ʧ��:%d!", GetLastError());
		MessageBox(HWND_DESKTOP, szError, "Error", MB_OK | MB_ICONEXCLAMATION);
		return NULL;
	}

	return hWnd;
}



//��Ϊȫ��ģʽ 
bool CWin32::ChangeScreenSetting()
{
	DEVMODE dmScreenSettings;
	ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));
	dmScreenSettings.dmSize			= sizeof(DEVMODE);
	dmScreenSettings.dmPelsWidth	= _nSrnWidth;
	dmScreenSettings.dmPelsHeight	= _nSrnHeight;
	dmScreenSettings.dmBitsPerPel	= 32;					/**< ����λ��� */
	//dmScreenSettings.dmDisplayFrequency = 75;             /**< ������Ļˢ���� */
	dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;//| DM_DISPLAYFREQUENCY;

	/// �ı���ʾģʽ
	if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		return false;

	return true;
}


int CWin32::GetWidth()
{
	return _bFullScreen ? _nSrnWidth : _nWinWidth;
}

int CWin32::GetHeight()
{
	return _bFullScreen ? _nSrnHeight : _nWinHeight;
}

//�л� ȫ��/����ģʽ
void CWin32::ToggleFullScreen()
{
	PostMessage(_hWnd, WM_TOGGLE_FULLSCREEN, 0, 0);
}

void CWin32::TerminateApplication()
{
	PostMessage(_hWnd, WM_QUIT, 0, 0);
	_isLooping = false;
}

bool CWin32::InitOpenGL(HWND hWnd, HGLRC& hRC)
{
	HDC hDC = GetDC(hWnd);  //�豸������
	if (hDC == NULL)
		return false;
	_hDC = hDC;

	PIXELFORMATDESCRIPTOR pfd =	   //���������ṹ
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,								 /**< �汾�� */
		PFD_DRAW_TO_WINDOW	|			 /**< �������������ʾ��һ�������� */
		PFD_SUPPORT_OPENGL	|			 /**< ������֧��OpenGL��ͼ */
		PFD_STEREO			|			 /**< ��ɫ�����������建�� */
		PFD_DOUBLEBUFFER,				 /**< ��ɫ��������˫���� */
		PFD_TYPE_RGBA,					 /**< ʹ��RGBA��ɫ��ʽ */
		32,								 /**< ��ɫ����������ɫֵ��ռ��λ�� */
		0, 0, 0, 0, 0, 0,				 /**< ʹ��Ĭ�ϵ���ɫ���� */
		0,								 /**< ��Alpha���� */
		0,								 /**< ��ɫ��������alpha�ɷֵ���λ���� */
		0,								 /**< ���ۼƻ����� */
		0, 0, 0, 0,						 /**< �ۼƻ���������λ */
		32,								 /**< 32λ��Ȼ��� */
		0,								 /**< ���ɰ滺�� */
		0,								 /**< �޸��������� */
		PFD_MAIN_PLANE,					 /**< ����ΪPFD_MAIN_PLANE������Ϊ����ͼ�� */
		0,								 /**< ��ʾOpenGLʵ����֧�ֵ��ϲ���²�ƽ������� */
		0, 0, 0							 /**< ��ʱ���Ѳ���ʹ�� */
	};

	GLuint pixelFormat = ChoosePixelFormat(hDC, &pfd);	/**< ����һ�����ݵ����ظ�ʽ */
	if (pixelFormat == 0)
		return false;													

	if (!SetPixelFormat(hDC, pixelFormat, &pfd))		/**< �������ظ�ʽ */
		return false;													

	hRC = wglCreateContext(hDC);		/**< ����OpenGL����Ⱦ������ */
	if (hRC == NULL)
		return false;													

	if (!wglMakeCurrent(hDC, hRC))			/**< ���õ�ǰ��OpenGL����Ⱦ����Ϊ��ǰ�Ĵ��� */
		return false;													
    
	ShowWindow(hWnd, SW_NORMAL);			/**< ��ʾ���� */
	ReshapeGL();							/**< ����OpenGL�������ڴ�С */

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	_resizeDraw = true;

	return true;				
}


// �����ڴ�С�ı�ʱ��֪ͨOpenGL������С
// ��������openglͶӰ����
void CWin32::ReshapeGL()
{
	GLsizei width = GetWidth();
	GLsizei height = GetHeight();
	glViewport(0, 0, width, height);	/**< ���������ӿ� */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//�ӵ�  Ŀ���  ����
	gluLookAt(0.0,5.0,0.0,  0,0,0, 1.0,0.0,0);
}

void CWin32::DestroyWin(HWND hWnd, HGLRC hRC)									
{
	if (hWnd != NULL)
	{
		HDC hDC = GetDC(hWnd);
		if (hDC != NULL)
		{
			wglMakeCurrent(hDC, 0);					/**< ���õ�ǰ��OpenGL����Ⱦ����Ϊ��NULL */
			if (hRC != 0)
			{
				wglDeleteContext(hRC);				/**< �ͷ�OpenGL����Ⱦ������ */
			}
			ReleaseDC(hWnd, hDC);					/**< �ͷŴ��ڵ��豸������ */
		}
		DestroyWindow(hWnd);
	}

	if (_bFullScreen)								/**< ���Ϊȫ��ģʽ���ڳ�������󣬱任������ģʽ������ʾ��� */
	{
		ChangeDisplaySettings(NULL, 0);				/**< �任������ģʽ */
		ShowCursor(true);							/**< ��ʾ��� */
	}
}

void CWin32::DealWinMessage(HWND hWnd)
{
	MSG msg;
	bool isMessagePumpActive = true;				/**< ����Ϣ��Ϊ��ʱ��������Ϣѭ�� */
	_dwLastTickCount = GetTickCount();
	while (isMessagePumpActive)
	{
		if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE) != 0)
		{													
			if (msg.message != WM_QUIT)						
			{
				DispatchMessage(&msg);
			}
			else											
			{
				isMessagePumpActive = false;
			}
		}
		else
		{
			if (!_isVisible)
			{
				WaitMessage();								/**< ��ͣ���򣬵ȴ���Ϣ */
			}
			else
			{												/**< ִ����Ϣѭ�� */
				DWORD tickCount = GetTickCount();			/**< ���ؼ�ʱ���ĵ�ǰֵ */
				Update(tickCount - _dwLastTickCount);		/**< �����û��Զ���ĸ��º��� */
				_dwLastTickCount = tickCount;				/**< ����������һ�Σ���������ֵ */
				Draw();										/**< �����û��Զ���Ļ��ƺ��� */
				SwapBuffers(_hDC);						/**< ����ǰ��֡���� */
			}
		}
	}
}



LRESULT CWin32::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		{																
			_isVisible = true;
			return 0;
		}
		break;

	case WM_SYSCOMMAND:  //�ػ�ϵͳ����
		switch (wParam)												
		{
			case SC_SCREENSAVE:   //�ػ���Ļ������������
			case SC_MONITORPOWER: //�ػ���ʾ��ʡ��ģʽ��������
				return 0;	//����������������
			break;
		}
		break;

	case WM_CLOSE:
		{
			TerminateApplication();
			return 0;
		}
		break;

	case WM_EXITMENULOOP:
	case WM_EXITSIZEMOVE:
		{
			_dwLastTickCount = GetTickCount();
			return 0;
		}
		break;

	case WM_MOVE:
		break;

	case WM_PAINT:
		if (_resizeDraw)	//�����Ҫ�ػ�
		{
			ReshapeGL();	//�������ô��ڵĴ�С
			Draw();					//���»���
			SwapBuffers(_hDC);	//����ǰ��֡����
		}
		break;

	case WM_SIZING:	 //�������ڸı��С
		{
			RECT * rect = (RECT *)lParam;

			if (!_bFullScreen)
			{
				_nWinWidth = LOWORD(lParam);
				_nWinHeight = HIWORD(lParam);
				Log("WM_SIZING:%d,%d", _nWinWidth, _nWinHeight);
			}
			return TRUE;
		}
		break;

	case WM_SIZE:		//���ڸı��С��
		switch (wParam)			//����ͬ�Ĵ���״̬
		{
		case SIZE_MINIMIZED:		//�Ƿ���С��?
			{
				_isVisible = false;	//����ǣ������ò��ɼ�
				Log("SIZE_MINIMIZED");
				return 0;											
			}
			break;

		case SIZE_MAXIMIZED:		//�����Ƿ����?
			{
				_nWinWidth = LOWORD(lParam);
				_nWinHeight = HIWORD(lParam);
				Log("SIZE_MAXIMIZED:%d,%d", _nWinWidth, _nWinHeight);
				ReshapeGL();
			}
			break;
		case SIZE_RESTORED:			//���ڱ���ԭ?
			{
				_isVisible = true;
				_nWinWidth = LOWORD(lParam);
				_nWinHeight = HIWORD(lParam);
				Log("SIZE_RESTORED:%d,%d", _nWinWidth, _nWinHeight);
				ReshapeGL();
				_dwLastTickCount = GetTickCount();
				return 0;											
			}
			break;
		}
		break;															

	case WM_KEYDOWN:
		switch (wParam)
		{
			case VK_ESCAPE:
				TerminateApplication();
				break;
			case VK_F1:
				ToggleFullScreen();
				break;
		}
		break;

	case WM_KEYUP:		
		break;

	case WM_TOGGLE_FULLSCREEN:	//�л� ȫ��/����ģʽ
	    {
	    	_bFullScreen = !_bFullScreen;
	  //   	if(!_bFullScreen)
			// 	ShowCursor(true);
			// else
			// 	ShowCursor(false);

			PostMessage(_hWnd, WM_QUIT, 0, 0);
	    }
		break;				
	default:
		break;											
	}

	return DefWindowProc(_hWnd, uMsg, wParam, lParam);
}

void CWin32::Update(DWORD milliseconds)
{
	CApplication::GetInstance()->Update(milliseconds);
}

void CWin32::Draw()
{
	CApplication::GetInstance()->Draw();
}





