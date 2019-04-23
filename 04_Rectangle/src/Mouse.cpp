#include "Mouse.h"


CMouse::CMouse() 
{ 
}

void CMouse::SetKeyStatus(int status) 
{ 
	keyStatus = status; 
}

bool CMouse::IsCtrlDown()
{
	return (keyStatus & MK_CONTROL) == MK_CONTROL;
}

bool CMouse::IsLBtnDown()
{
	return (keyStatus & MK_LBUTTON) == MK_LBUTTON;
}

bool CMouse::IsRBtnDown()
{
	return (keyStatus & MK_RBUTTON) == MK_RBUTTON;
}

bool CMouse::IsMBtnDown()
{
	return (keyStatus & MK_MBUTTON) == MK_MBUTTON;
}

bool CMouse::IsShiftDown()
{
	return (keyStatus & MK_SHIFT) == MK_SHIFT;
}

