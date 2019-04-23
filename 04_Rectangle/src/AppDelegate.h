#pragma once

#include "Application.h"

class AppDelegate : CApplication
{
public:
	virtual bool Init();
	virtual void UnInit();
	virtual void Update(DWORD milliseconds);
	virtual void Draw();

private:
	friend class CApplication;
	AppDelegate();

private:
	float _fAngle;
};
