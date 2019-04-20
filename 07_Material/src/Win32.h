#pragma once

#include <windows.h>
#include <gl.h>
#include <glu.h>

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




class CWin32
{
public:
	CWin32();
	virtual ~CWin32();

	HWND GetHWnd() const { return _hWnd; }
	bool IsLooping() const { return _isLooping; }
	void SetVisible(bool value) { _isVisible = value; }
	void SetFullScreen(bool value) { _bFullScreen = value; }

	//设置在窗口改变大小的时候，可以绘制
	void ResizeDraw(bool enable) { _resizeDraw = enable; };
	bool IsPressed(unsigned int key);

	bool Init();
	void UnInit();
	void Run();

	LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);


protected:
	bool RegisterMyClass();
	void UnRegisterMyClass();

	HWND CreateMyWindow(LPVOID lpParam);

	void Update(DWORD milliseconds);
	void Draw();
	

private:
	bool ChangeScreenSetting();
	int GetWidth();
	int GetHeight();

	bool InitOpenGL(HWND hWnd, HGLRC& hRC);
	void ReshapeGL();
	void DestroyWin(HWND hWnd, HGLRC hRC);
	void DealWinMessage(HWND hWnd);

	void ToggleFullScreen();
	void TerminateApplication();

protected:
	Keys _keys;


private:
	HINSTANCE _hInstance;
	HWND _hWnd;
	HDC	_hDC;
	HGLRC _hRC;
	bool _bFullScreen;
	bool _resizeDraw;
	bool _isVisible;
	bool _isLooping;

	int _nSrnWidth;
	int _nSrnHeight;
	int _nWinWidth;
	int _nWinHeight;

	DWORD _dwLastTickCount;
};


