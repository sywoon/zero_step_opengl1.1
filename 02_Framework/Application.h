#pragma once

#include "Window.h"


class Keys
{
public:
	Keys();

	void Clear();
	bool IsPressed(unsigned int key);
	void SetPressed(unsigned int key);
	void SetReleased(unsigned int key);

private:
	static const unsigned int MAX_KEYS = 256;
	bool m_keyDown[MAX_KEYS];  //保存256个按键的状态
};


class Application
{
public:
	static Application* Create(const char* pszClassName);
	virtual ~Application() {};

protected:
	virtual bool Init() = 0;
	virtual void UnInit() = 0;
	virtual void Update(DWORD milliseconds) = 0;
	virtual void Draw() = 0;

	void ToggleFullScreen();
	void TerminateApplication();

	//设置在窗口改变大小的时候，可以绘制
	void ResizeDraw(bool enable) { m_resizeDraw = enable; };

	Application(const char* pszClassName);

protected:
	Keys m_keys;

private:
	friend int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreIns,
						LPSTR lpCmdLine, int nCmdShow);
	int Main(HINSTANCE hIns, HINSTANCE hPreIns, LPSTR lpCmdLine, int nCmdShow);

	friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, 
						WPARAM wParam, LPARAM lParam);
	LRESULT Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	//自定义消息，在切换窗口模式的时候发送
	static const UINT WM_TOGGLE_FULLSCREEN = (WM_USER + 1);
	Window m_window;
	const char* m_pszClassName;
	bool m_isLooping;
	bool m_isVisible;
	bool m_resizeDraw;  //是否在改变大小时，调用了绘制函数
	DWORD m_dwLastTickCount;
};




