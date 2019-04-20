#include "Application.h"
#include "ScreenDlg.h"      
#include "Log.h"

#include <assert.h>
#include <stdio.h>
#include <gl.h>
#include <glu.h>


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

//无法解析的外部符号 __imp__RegCloseKey@4
#pragma comment (lib, "Advapi32.lib")

//无法解析的外部符号 _sscanf
// vs2015默认编译时将许多标准库采用内联方式处理
#pragma comment (lib, "legacy_stdio_definitions.lib")



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

    Log("=========start===========");
    Log("中文测试 utf-8");

	int ret = -1;
	Application * appl = Application::Create("OpenGL");
	if (appl != NULL)
	{
		ret = appl->Main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
		delete appl;
	}
	else
	{
		MessageBox(HWND_DESKTOP, "创建程序出错", "Error", MB_OK | MB_ICONEXCLAMATION);
	}
	return ret;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//回用户自定义的32位的程序附加值
	LONG userData = GetWindowLong(hWnd, GWL_USERDATA);
	if (userData == 0)
	{	
		/// 如果程序第一次运行
		if (uMsg == WM_CREATE)
		{																
			/// 返回窗口结构的指针，它保存刚创建的程序实例的类
			CREATESTRUCT * creation = reinterpret_cast<CREATESTRUCT *>(lParam);
			/// 获得程序实例的指针
			Application * appl = reinterpret_cast<Application *>(creation->lpCreateParams);
			/// 保存程序实例的指针为用户自定义的程序附加值
			SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast<LONG>(appl));
			appl->m_isVisible = true;									/**< 设置程序可见 */
			return 0;
		}
	}
	else
	{	/// 如果不是第一次窗口，返回程序实例的指针
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

//切换 全屏/窗口模式
void Application::ToggleFullScreen()
{
	PostMessage(m_window, WM_TOGGLE_FULLSCREEN, 0, 0);	//发送自定的切换消息
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
	case WM_SYSCOMMAND:  //截获系统命令
		switch (wParam)												
		{
			case SC_SCREENSAVE:   //截获屏幕保护启动命令
			case SC_MONITORPOWER: //截获显示其省电模式启动命令
				return 0;	//不启用这两个命令
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
		if (m_resizeDraw)	//如果需要重绘
		{
			m_window.ReshapeGL();	//重新设置窗口的大小
			Draw();					//重新绘制
			m_window.SwapBuffers();	//交换前后帧缓存
		}
		break;

	case WM_SIZING:	 //窗口正在改变大小
		{
			RECT * rect = (RECT *)lParam;
			m_window.SetWidth(rect->right - rect->left);	//设置窗口宽度
			m_window.SetHeight(rect->bottom - rect->top);	//设置窗口高度
			return TRUE;
		}
		break;

	case WM_SIZE:		//窗口改变大小后
		switch (wParam)			//处理不同的窗口状态
		{
		case SIZE_MINIMIZED:		//是否最小化?
			{
				m_isVisible = false;	//如果是，则设置不可见
				return 0;											
			}
			break;

		case SIZE_MAXIMIZED:		//窗口是否最大化?
		case SIZE_RESTORED:			//窗口被还原?
			{
				m_isVisible = true;									/**< 设置为可见 */
				m_window.SetWidth(LOWORD(lParam));					/**< 设置窗口宽度 */
				m_window.SetHeight(HIWORD(lParam));					/**< 设置窗口高度 */
				m_window.ReshapeGL();								/**< 改变窗口大小 */
				m_dwLastTickCount = GetTickCount();					/**< 更新计数器的值 */
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

	case WM_TOGGLE_FULLSCREEN:	//切换 全屏/窗口模式
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



/** 程序的主循环 */
int Application::Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/// 注册一个窗口
	WNDCLASSEX windowClass;												/**< 窗口类 */
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));						/**< 清空结构为0 */
	windowClass.cbSize			= sizeof(WNDCLASSEX);					/**< 窗口结构的大小 */
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	/**< 设置窗口类型为，移动时重画，并为窗口取得DC */
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				/**< WndProc处理消息 */
	windowClass.hInstance		= hInstance;							/**< 设置实例 */
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			/**< 设置背景 */
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			/**< 载入光标 */
	windowClass.lpszClassName	= m_pszClassName;							/**< 设置类名 */
	if (RegisterClassEx(&windowClass) == 0)								/**< 尝试注册窗口类 */
	{																	/**< NOTE: Failure, Should Never Happen */
		MessageBox(HWND_DESKTOP, "注册窗口失败!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														/**< 退出并返回FALSE */
	}

	/// 询问是否在全屏状态下运行?
	//if (MessageBox(HWND_DESKTOP, "你想在全屏状态下运行么 ?", "是否全屏运行?", MB_YESNO | MB_ICONQUESTION) == IDNO)
	//{
	//	m_CreateFullScreen = false;
	//}
	ScreenDlg sd;
	sd.SetupWindow(&m_window);
    //m_CreateFullScreen = m_window.GetFullScreen();
	while (m_isLooping)											/**< 循环直道WM_QUIT退出程序 */
	{																	
		/// 创建一个窗口
		if (m_window.Create("GLFrame", m_pszClassName, hInstance, this))
		{	
			/// 如果初始化失败，则退出
			if (!Init())									/**< 调用自定义的初始化函数 */
			{															/**< 失败 */
				TerminateApplication();									/**< 关闭窗口退出程序 */
			}
			else														/**< 成功开始消息循环 */
			{															 
				MSG msg;												/**< Window消息结构 */ 
				bool isMessagePumpActive = true;						/**< 当消息不为空时，处理消息循环 */
				m_dwLastTickCount = GetTickCount();						/**< 返回当前的计时器的值 */
				m_keys.Clear();											/**< 清空所有的按键信息 */
				while (isMessagePumpActive)						/**< 当消息不为空时，处理消息循环 */
				{														/**< 成功创建窗口，检测窗口消息 */
					if (PeekMessage(&msg, m_window, 0, 0, PM_REMOVE) != 0)
					{													
						/// 检测是否为WM_QUIT消息
						if (msg.message != WM_QUIT)						
						{
							DispatchMessage(&msg);						/**< 如果不是发送消息到消息回调函数中处理 */
						}
						else											
						{
							isMessagePumpActive = false;				/**< 如果是，则退出 */
						}
					}
					/// 如果没有消息
					else												
					{
						if (!m_isVisible)								/**< 如果窗口不可见 */
						{
							WaitMessage();								/**< 暂停程序，等待消息 */
						}
						else											/**< 如果窗口可见 */
						{												/**< 执行消息循环 */
							DWORD tickCount = GetTickCount();			/**< 返回计时器的当前值 */
							Update(tickCount - m_dwLastTickCount);		/**< 调用用户自定义的更新函数 */
							m_dwLastTickCount = tickCount;				/**< 重新设置上一次，计数器的值 */
							Draw();										/**< 调用用户自定义的绘制函数 */
							m_window.SwapBuffers();						/**< 交换前后帧缓存 */
						}
					}
				}														/**< 如果isMessagePumpActive == true，则循环 */
			}															
																		/**< 程序结束 */
			UnInit();												/**< 用户自定义的卸载函数 */
			m_window.Destroy();											/**< 删除窗口 */
		}
		else															/**< 如果创建窗口失败 */
		{																
			MessageBox(HWND_DESKTOP, "创建OpenGL窗口错误", "Error", MB_OK | MB_ICONEXCLAMATION);
			m_isLooping = false;									/**< 停止程序循环 */
		}
	}																	

	UnregisterClass(m_pszClassName, hInstance);							/**< 取消注册的窗口 */
	return 0;
}
