#include "Application.h"
#include "ScreenDlg.h"      

#include <assert.h>
#include <stdio.h>
#include <gl.h>
#include <glu.h>


#define USE_WIN32_CONSOLE

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				LPSTR lpCmdLine, int nCmdShow)
{
#ifdef USE_WIN32_CONSOLE
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

	int ret = -1;
	Application * appl = Application::Create("OpenGL");
	if (appl != NULL)
	{
		ret = appl->Main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
		delete appl;
	}
	else
	{
		MessageBox(HWND_DESKTOP, "�����������", "Error", MB_OK | MB_ICONEXCLAMATION);
	}
	return ret;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//���û��Զ����32λ�ĳ��򸽼�ֵ
	LONG userData = GetWindowLong(hWnd, GWL_USERDATA);
	if (userData == 0)
	{	
		/// ��������һ������
		if (uMsg == WM_CREATE)
		{																
			/// ���ش��ڽṹ��ָ�룬������մ����ĳ���ʵ������
			CREATESTRUCT * creation = reinterpret_cast<CREATESTRUCT *>(lParam);
			/// ��ó���ʵ����ָ��
			Application * appl = reinterpret_cast<Application *>(creation->lpCreateParams);
			/// �������ʵ����ָ��Ϊ�û��Զ���ĳ��򸽼�ֵ
			SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast<LONG>(appl));
			appl->m_isVisible = true;									/**< ���ó���ɼ� */
			return 0;
		}
	}
	else
	{	/// ������ǵ�һ�δ��ڣ����س���ʵ����ָ��
		Application * appl = reinterpret_cast<Application *>(userData);
		return appl->Message(hWnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



Keys::Keys() 
{ 
	Clear(); 
}

void Keys::Clear() 
{ 
	ZeroMemory(&m_keyDown, sizeof(m_keyDown)); 
}

bool Keys::IsPressed(unsigned int key) 
{ 
	assert(key < MAX_KEYS);
	return  m_keyDown[key];
}

void Keys::SetPressed(unsigned int key) 
{
	assert(key < MAX_KEYS);
	m_keyDown[key] = true;
}

void Keys::SetReleased(unsigned int key) 
{
	assert(key < MAX_KEYS);
	m_keyDown[key] = false;
}



Application::Application(const char * pszClassName)
	: m_pszClassName(pszClassName)
	, m_isLooping(true)			
	, m_isVisible(false)
	, m_resizeDraw(false)
	, m_dwLastTickCount(0)
{
}

//�л� ȫ��/����ģʽ
void Application::ToggleFullScreen()
{
	PostMessage(m_window, WM_TOGGLE_FULLSCREEN, 0, 0);	//�����Զ����л���Ϣ
}

void Application::TerminateApplication()
{
	PostMessage(m_window, WM_QUIT, 0, 0);
	m_isLooping = false;	
}

LRESULT Application::Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
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
			m_dwLastTickCount = GetTickCount();
			return 0;
		}
		break;

	case WM_MOVE:
		{
			m_window.SetPosX(LOWORD(lParam));
			m_window.SetPosY(HIWORD(lParam));							
			return 0;
		}
		break;

	case WM_PAINT:
		if (m_resizeDraw)	//�����Ҫ�ػ�
		{
			m_window.ReshapeGL();	//�������ô��ڵĴ�С
			Draw();					//���»���
			m_window.SwapBuffers();	//����ǰ��֡����
		}
		break;

	case WM_SIZING:	 //�������ڸı��С
		{
			RECT * rect = (RECT *)lParam;
			m_window.SetWidth(rect->right - rect->left);	//���ô��ڿ��
			m_window.SetHeight(rect->bottom - rect->top);	//���ô��ڸ߶�
			return TRUE;
		}
		break;

	case WM_SIZE:		//���ڸı��С��
		switch (wParam)			//����ͬ�Ĵ���״̬
		{
		case SIZE_MINIMIZED:		//�Ƿ���С��?
			{
				m_isVisible = false;	//����ǣ������ò��ɼ�
				return 0;											
			}
			break;

		case SIZE_MAXIMIZED:		//�����Ƿ����?
		case SIZE_RESTORED:			//���ڱ���ԭ?
			{
				m_isVisible = true;									/**< ����Ϊ�ɼ� */
				m_window.SetWidth(LOWORD(lParam));					/**< ���ô��ڿ�� */
				m_window.SetHeight(HIWORD(lParam));					/**< ���ô��ڸ߶� */
				m_window.ReshapeGL();								/**< �ı䴰�ڴ�С */
				m_dwLastTickCount = GetTickCount();					/**< ���¼�������ֵ */
				return 0;											
			}
			break;
		}
		break;															

	case WM_KEYDOWN:
		{
			m_keys.SetPressed(wParam);									
			return 0;													
		}
		break;

	case WM_KEYUP:		
		{
			m_keys.SetReleased(wParam);									
			return 0;
		}
		break;

	case WM_TOGGLE_FULLSCREEN:	//�л� ȫ��/����ģʽ
	    {
	    	m_window.SetFullScreen(!m_window.GetFullScreen());
	    	if(!m_window.GetFullScreen())
				ShowCursor(true);
			else
				ShowCursor(false);

			PostMessage(hWnd, WM_QUIT, 0, 0);
	    }
		break;				
	default:
		break;											
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



/** �������ѭ�� */
int Application::Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/// ע��һ������
	WNDCLASSEX windowClass;												/**< ������ */
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));						/**< ��սṹΪ0 */
	windowClass.cbSize			= sizeof(WNDCLASSEX);					/**< ���ڽṹ�Ĵ�С */
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	/**< ���ô�������Ϊ���ƶ�ʱ�ػ�����Ϊ����ȡ��DC */
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				/**< WndProc������Ϣ */
	windowClass.hInstance		= hInstance;							/**< ����ʵ�� */
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			/**< ���ñ��� */
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			/**< ������ */
	windowClass.lpszClassName	= m_pszClassName;							/**< �������� */
	if (RegisterClassEx(&windowClass) == 0)								/**< ����ע�ᴰ���� */
	{																	/**< NOTE: Failure, Should Never Happen */
		MessageBox(HWND_DESKTOP, "ע�ᴰ��ʧ��!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														/**< �˳�������FALSE */
	}

	/// ѯ���Ƿ���ȫ��״̬������?
	/*if (MessageBox(HWND_DESKTOP, "������ȫ��״̬������ô ?", "�Ƿ�ȫ������?", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		m_CreateFullScreen = false;										/**< m_CreateFullScreen��¼��ǰ����ʾģʽΪ���� */
	//}
	ScreenDlg sd;
	sd.SetupWindow(&m_window);
    //m_CreateFullScreen = m_window.GetFullScreen();
	while (m_isLooping)											/**< ѭ��ֱ��WM_QUIT�˳����� */
	{																	
		/// ����һ������
		if (m_window.Create("GLFrame", m_pszClassName, hInstance, this))
		{	
			/// �����ʼ��ʧ�ܣ����˳�
			if (!Init())									/**< �����Զ���ĳ�ʼ������ */
			{															/**< ʧ�� */
				TerminateApplication();									/**< �رմ����˳����� */
			}
			else														/**< �ɹ���ʼ��Ϣѭ�� */
			{															 
				MSG msg;												/**< Window��Ϣ�ṹ */ 
				bool isMessagePumpActive = true;						/**< ����Ϣ��Ϊ��ʱ��������Ϣѭ�� */
				m_dwLastTickCount = GetTickCount();						/**< ���ص�ǰ�ļ�ʱ����ֵ */
				m_keys.Clear();											/**< ������еİ�����Ϣ */
				while (isMessagePumpActive)						/**< ����Ϣ��Ϊ��ʱ��������Ϣѭ�� */
				{														/**< �ɹ��������ڣ���ⴰ����Ϣ */
					if (PeekMessage(&msg, m_window, 0, 0, PM_REMOVE) != 0)
					{													
						/// ����Ƿ�ΪWM_QUIT��Ϣ
						if (msg.message != WM_QUIT)						
						{
							DispatchMessage(&msg);						/**< ������Ƿ�����Ϣ����Ϣ�ص������д��� */
						}
						else											
						{
							isMessagePumpActive = false;				/**< ����ǣ����˳� */
						}
					}
					/// ���û����Ϣ
					else												
					{
						if (!m_isVisible)								/**< ������ڲ��ɼ� */
						{
							WaitMessage();								/**< ��ͣ���򣬵ȴ���Ϣ */
						}
						else											/**< ������ڿɼ� */
						{												/**< ִ����Ϣѭ�� */
							DWORD tickCount = GetTickCount();			/**< ���ؼ�ʱ���ĵ�ǰֵ */
							Update(tickCount - m_dwLastTickCount);		/**< �����û��Զ���ĸ��º��� */
							m_dwLastTickCount = tickCount;				/**< ����������һ�Σ���������ֵ */
							Draw();										/**< �����û��Զ���Ļ��ƺ��� */
							m_window.SwapBuffers();						/**< ����ǰ��֡���� */
						}
					}
				}														/**< ���isMessagePumpActive == true����ѭ�� */
			}															
																		/**< ������� */
			UnInit();												/**< �û��Զ����ж�غ��� */
			m_window.Destroy();											/**< ɾ������ */
		}
		else															/**< �����������ʧ�� */
		{																
			MessageBox(HWND_DESKTOP, "����OpenGL���ڴ���", "Error", MB_OK | MB_ICONEXCLAMATION);
			m_isLooping = false;									/**< ֹͣ����ѭ�� */
		}
	}																	

	UnregisterClass(m_pszClassName, hInstance);							/**< ȡ��ע��Ĵ��� */
	return 0;
}
