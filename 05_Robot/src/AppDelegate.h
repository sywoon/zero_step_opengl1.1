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
	void    DrawRobot(float xPos, float yPos, float zPos); /**< 绘制机器人 */
	
	void    DrawHead(float xPos,float yPos,float zPos);    /**< 绘制头部 */
	void    DrawTorso(float xPos, float yPos, float zPos); /**< 绘制躯干 */
	void    DrawLeg(float xPos, float yPos, float zPos);   /**< 绘制腿 */
	void    DrawArm(float xPos, float yPos, float zPos);   /**< 绘制胳膊 */
   
	void    DrawCube(float xPos, float yPos, float zPos);  /**< 绘制立方体 */




private:
	float _angle;            /**< 机器人绕视点旋转的角度 */
	float _legAngle[2];		/**< 腿的当前旋转角度 */
    float _armAngle[2];      /**< 胳膊的当前旋转角度 */
};
