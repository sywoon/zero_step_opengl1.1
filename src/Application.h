#pragma once

#include <windows.h>


class CApplication
{
public:
	static CApplication* Create();
	static CApplication* GetInstance();
	static void Destroy();

	void Update(DWORD milliseconds);
	void Draw();

protected:
	CApplication();
	~CApplication();

private:
	float _fAngle;
};