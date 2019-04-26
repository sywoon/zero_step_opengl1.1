#pragma once

#include <windows.h>

class Mouse
{
public:
	Mouse();

	void setKeyStatus(int status);
	bool isCtrlDown();
	bool isLBtnDown();
	bool isRBtnDown();
	bool isMBtnDown();
	bool iIsShiftDown();

	POINT& getPos() { return _pos; }
	void setPos(int x, int y) { _pos.x = x; _pos.y = y; }
	void setPos(POINT& pt) { _pos = pt; }


private:
	POINT _pos;
	int _keyStatus;
};



