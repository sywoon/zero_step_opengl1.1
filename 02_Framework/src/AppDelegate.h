#pragma once

#include "Application.h"

class AppDelegate : Application
{
public:
	virtual bool Init();
	virtual void UnInit();
	virtual void Update(DWORD milliseconds);
	virtual void Draw();

private:
	friend class Application;
	AppDelegate(const char* pszClassName);

private:
	float m_fAngle;
};
