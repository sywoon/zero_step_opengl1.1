#include <windows.h>
#include <assert.h>
#include <stdio.h>


#include "Log.h"
#include "Application.h"


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

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

	InitLogFile();
	//InitLogFile("Log");
	//InitLogFile("Log", "output.log");
	//ClearLog();
	Log("=========start===========");
	Log("游戏中文测试 utf-8");
	
	CApplication* app = CApplication::Create();
	app->Run();
    CApplication::Destroy();
	
	Log("=========end===========\n");

#ifdef USE_WIN32_CONSOLE
	FreeConsole();
#endif

	return 0;
}

