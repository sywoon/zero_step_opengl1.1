#ifndef	__SCREENDLG_H__
#define	__SCREENDLG_H__

#include "Window.h"

/** ����ģʽѡ��Ի����� */
class ScreenDlg
{
public:
	ScreenDlg(void);
	~ScreenDlg(void);

	void SetupWindow(Window *pWindow);
    
	static LRESULT CALLBACK ScreenDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static Window* m_pWindow;
};
#endif //__SCREENDLG_H__