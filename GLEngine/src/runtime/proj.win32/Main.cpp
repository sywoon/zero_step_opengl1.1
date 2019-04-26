#include "Base.h"
#include "Application.h"


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
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef USE_WIN32_CONSOLE
    AllocConsole();

    //freopen废弃用 用freopen_s 或 /D _CRT_SECURE_NO_WARNINGS
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

	mylog::InitLogFile();
	//mylog::InitLogFile("Log");
	//mylog::InitLogFile("Log", "output.log");
	//mylog::ClearLog();
	Log("=========start===========");
	Log("游戏中文测试 utf-8");
	
	Application::create();
	g_app.run();
    Application::destroy();
	
	Log("=========end===========\n");

#ifdef USE_WIN32_CONSOLE
	FreeConsole();
#endif

	return 0;
}

