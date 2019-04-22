#pragma once

#include "Application.h"
#include "GLFont.h"


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
	GLFont font;                /**< 定义一个字体实例 */
	float	cnt1;				/**< 字体移动计数器1 */
    float	cnt2;				/**< 字体移动计数器2 */
};
