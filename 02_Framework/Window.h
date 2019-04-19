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
	void	ReshapeGL();   //�����ڴ�С�ı�ʱ��֪ͨOpenGL������С
	void	SwapBuffers() { ::SwapBuffers(m_hDC); }

	/** ���ô������Ͻǵ�λ�� */
	void	SetPosX(int x);
	void	SetPosX(unsigned short x) { SetPosX((signed short)x); }		
	void	SetPosY(int y);
	void	SetPosY(unsigned short y) { SetPosY((signed short)y); }		
	/** ���ش������Ͻǵ�λ�� */
	int		GetPosX();
	int		GetPosY();

	/** ���ش��ڵĴ�С */
	int		GetWidth();
	int		GetHeight();
	/** ���ô��ڵĴ�С */
	void	SetWidth(int width);
	void	SetHeight(int height);

	/** ���ô��ڵ���ɫλ�� */
	void	SetHiColor()	{ m_bitsPerPixel = 16; }
	void	SetTrueColor()	{ m_bitsPerPixel = 32; }
	void	SetFullScreen(bool mode)	{ m_isFullScreen = mode; }
	bool	GetFullScreen()	{ return m_isFullScreen; }

	/** �����������������Window m_window�������m_window��Ϊ���ھ��ʹ�� */
	operator HWND() { return m_hWnd; }

private:
	HWND	m_hWnd;							/**< ���ھ�� */
	HDC		m_hDC;							/**< �豸������ */
	HGLRC	m_hRC;							/**< OpenGL��Ⱦ������ */

	int		m_windowPosX;					/**< ���ڵ����Ͻǵ�Xλ�� */
	int		m_windowPosY;					/**< ���ڵ����Ͻǵ�Yλ�� */
	int		m_windowWidth;					/**< ���ڵĿ�� */
	int		m_windowHeight;					/**< ���ڵĸ߶� */
	int		m_screenWidth;					/**< ȫ���Ŀ�� */
	int		m_screenHeight;					/**< ȫ���ĸ߶� */
	int		m_bitsPerPixel;					/**< ��ɫλ�� */
	bool	m_isFullScreen;					/**< �Ƿ�ȫ�� */
};

#endif	// __GLWINDOW_H__