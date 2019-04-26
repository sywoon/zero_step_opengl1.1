#include "Mouse.h"


Mouse::Mouse() 
{ 
}

void Mouse::setKeyStatus(int status) 
{ 
	_keyStatus = status; 
}

bool Mouse::isCtrlDown()
{
	return (_keyStatus & MK_CONTROL) == MK_CONTROL;
}

bool Mouse::isLBtnDown()
{
	return (_keyStatus & MK_LBUTTON) == MK_LBUTTON;
}

bool Mouse::isRBtnDown()
{
	return (_keyStatus & MK_RBUTTON) == MK_RBUTTON;
}

bool Mouse::isMBtnDown()
{
	return (_keyStatus & MK_MBUTTON) == MK_MBUTTON;
}

bool Mouse::iIsShiftDown()
{
	return (_keyStatus & MK_SHIFT) == MK_SHIFT;
}

