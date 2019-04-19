#include <windows.h>

static LPCTSTR lpszAppName = "GDI App";

HBRUSH hBlueBrush, hRedBrush;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wc;
	HWND hWnd;

	hBlueBrush	= CreateSolidBrush(RGB(0, 0, 255));
	hRedBrush	= CreateSolidBrush(RGB(255, 0, 0));

	wc.style	= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance	  = hInstance;
	wc.hIcon	  = NULL;
	wc.hCursor	  = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = hBlueBrush;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = lpszAppName;

	if (RegisterClass(&wc) == 0)
		return FALSE;

	hWnd = CreateWindow(lpszAppName, lpszAppName, WS_OVERLAPPEDWINDOW,
						100, 100, 250, 250,
						NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
		return FALSE;

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DeleteObject(hBlueBrush);
	DeleteObject(hRedBrush);

	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HBRUSH hOldBrush;

			BeginPaint(hWnd, &ps);
			hOldBrush = SelectObject(ps.hdc, hRedBrush);
			Rectangle(ps.hdc, 100, 100, 150, 150);
			SelectObject(ps.hdc, hOldBrush);
			EndPaint(hWnd, &ps);
		}
		break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0L;
}



