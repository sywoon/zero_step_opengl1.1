#pragma once

#include <windows.h>

class CMouse
{
public:
	CMouse();

	void SetKeyStatus(int status);
	bool IsCtrlDown();
	bool IsLBtnDown();
	bool IsRBtnDown();
	bool IsMBtnDown();
	bool IsShiftDown();

	POINT& GetPos() { return pos; }
	void SetPos(int x, int y) { pos.x = x; pos.y = y; }
	void SetPos(POINT& pt) { pos = pt; }


private:
	POINT pos;
	int keyStatus;
};


