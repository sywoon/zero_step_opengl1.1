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
	float m_Angle;                                      /**< 用来旋转物体 */
    float m_bRedX;                                      /**< 红色球的旋转角度 */
	float m_bRedY;
	float m_bGreenX;                                    /**< 绿色球的旋转角度 */
	float m_bGreenY;
	float m_bBlueX;                                     /**< 蓝色球的旋转角度 */
	float m_bBlueY; 
};
