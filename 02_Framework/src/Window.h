#ifndef	__GLWINDOW_H__
#define	__GLWINDOW_H__

#include <windows.h>

#pragma warning(disable: 4311)
#pragma warning(disable: 4312)
#pragma warning(disable: 4244)

class Window
{
public:
	Window();

	bool	Create(const char * window_title, const char * class_name, HINSTANCE h_instance, LPVOID lpParam);
	void	Destroy();
	bool	ChangeScreenSetting();
	void	ReshapeGL();   //当窗口大小改变时，通知OpenGL调整大小
	void	SwapBuffers() { ::SwapBuffers(m_hDC); }

	/** 设置窗口左上角的位置 */
	void	SetPosX(int x);
	void	SetPosX(unsigned short x) { SetPosX((signed short)x); }		
	void	SetPosY(int y);
	void	SetPosY(unsigned short y) { SetPosY((signed short)y); }		
	/** 返回窗口左上角的位置 */
	int		GetPosX();
	int		GetPosY();

	/** 返回窗口的大小 */
	int		GetWidth();
	int		GetHeight();
	/** 设置窗口的大小 */
	void	SetWidth(int width);
	void	SetHeight(int height);

	/** 设置窗口的颜色位深 */
	void	SetHiColor()	{ m_bitsPerPixel = 16; }
	void	SetTrueColor()	{ m_bitsPerPixel = 32; }
	void	SetFullScreen(bool mode)	{ m_isFullScreen = mode; }
	bool	GetFullScreen()	{ return m_isFullScreen; }

	/** 重载运算符，可以让Window m_window声明后的m_window作为窗口句柄使用 */
	operator HWND() { return m_hWnd; }

private:
	HWND	m_hWnd;							/**< 窗口句柄 */
	HDC		m_hDC;							/**< 设备描述表 */
	HGLRC	m_hRC;							/**< OpenGL渲染描述表 */

	int		m_windowPosX;					/**< 窗口的左上角的X位置 */
	int		m_windowPosY;					/**< 窗口的左上角的Y位置 */
	int		m_windowWidth;					/**< 窗口的宽度 */
	int		m_windowHeight;					/**< 窗口的高度 */
	int		m_screenWidth;					/**< 全屏的宽度 */
	int		m_screenHeight;					/**< 全屏的高度 */
	int		m_bitsPerPixel;					/**< 颜色位深 */
	bool	m_isFullScreen;					/**< 是否全屏 */
};

#endif	// __GLWINDOW_H__