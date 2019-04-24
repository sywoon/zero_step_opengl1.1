#pragma once

#include "Base.h"
#include "Mouse.h"

class Keys
{
public:
	Keys();

	void clear();
	bool isPressed(unsigned int key);
	void setPressed(unsigned int key);
	void setReleased(unsigned int key);

private:
	static const unsigned int MAX_KEYS = 256;
	bool _keyDown[MAX_KEYS];  //保存256个按键的状态
};




class Win32
{
public:
	Win32();
	virtual ~Win32();

	HWND getHWnd() const { return _hWnd; }
	bool isLooping() const { return _isLooping; }
	void setVisible(bool value) { _isVisible = value; }
	void setFullScreen(bool value) { _bFullScreen = value; }

	//设置在窗口改变大小的时候，可以绘制
	void resizeDraw(bool enable) { _resizeDraw = enable; };
	bool isPressed(unsigned int key);

	int getWidth();
	int getHeight();

	bool init();
	void unInit();
	void run();

	Mouse& getMouse() { return _mouse; }

	LRESULT windowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);


protected:
	bool registerMyClass();
	void unRegisterMyClass();

	HWND createMyWindow(LPVOID lpParam);

	void update(DWORD milliseconds);
	void draw();
	

private:
	bool changeScreenSetting();

	bool initOpenGL(HWND hWnd, HGLRC& hRC);
	void reshapeGL();
	void destroyWin(HWND hWnd, HGLRC hRC);
	void dealWinMessage(HWND hWnd);

	void toggleFullScreen();
	void terminateApplication();

protected:
	Keys _keys;
	Mouse _mouse;


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


