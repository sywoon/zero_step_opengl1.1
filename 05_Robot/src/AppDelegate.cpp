#include "AppDelegate.h"

#include <stdio.h>
#include <math.h>
#include <gl.h>
#include <glu.h>
#include <glaux.h>


static CApplication* s_pInstance = NULL;



CApplication* CApplication::Create()
{
	if (s_pInstance)
		return s_pInstance;

	AppDelegate* app = new AppDelegate();
	s_pInstance = app;
	return reinterpret_cast<CApplication*>(app);
}

CApplication* CApplication::GetInstance()
{
	return s_pInstance;
}

void CApplication::Destroy()
{
	if (s_pInstance)
	{
		free(s_pInstance);
		s_pInstance = NULL;
	}
}




AppDelegate::AppDelegate()
	: CApplication()
{
	_angle = 0.0f;                    /**< 设置初始角度为0 */
	_legAngle[0] = _legAngle[1] = 0.0f;				
	_armAngle[0] = _armAngle[1] = 0.0f;
}

bool AppDelegate::Init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	ResizeDraw(true);

	return true;
}

void AppDelegate::UnInit()
{
}



/** 绘制立方体 */
void AppDelegate::DrawCube(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glTranslatef(xPos, yPos, zPos);
		glBegin(GL_POLYGON);
			
		    /** 顶面 */
		    glVertex3f(0.0f, 0.0f, 0.0f);	 
			glVertex3f(0.0f, 0.0f, -1.0f);
			glVertex3f(-1.0f, 0.0f, -1.0f);
			glVertex3f(-1.0f, 0.0f, 0.0f);
			
            /** 前面 */
			glVertex3f(0.0f, 0.0f, 0.0f);	 
			glVertex3f(-1.0f, 0.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glVertex3f(0.0f, -1.0f, 0.0f);
			
			/** 右面 */
			glVertex3f(0.0f, 0.0f, 0.0f);	
			glVertex3f(0.0f, -1.0f, 0.0f);
			glVertex3f(0.0f, -1.0f, -1.0f);
			glVertex3f(0.0f, 0.0f, -1.0f);
			
			/** 左面*/
			glVertex3f(-1.0f, 0.0f, 0.0f);	
			glVertex3f(-1.0f, 0.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			
			/** 底面 */
			glVertex3f(0.0f, 0.0f, 0.0f);	
			glVertex3f(0.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);
				
			
			/** 后面 */
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(-1.0f, 0.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(0.0f, -1.0f, -1.0f);
		glEnd();
	glPopMatrix();
}

/** 绘制一个手臂 */
void AppDelegate::DrawArm(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	    glColor3f(1.0f, 0.0f, 0.0f);    /**< 红色 */	
		glTranslatef(xPos, yPos, zPos);
		glScalef(1.0f, 4.0f, 1.0f);	    /**< 手臂是1x4x1的立方体 */
		DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

/** 绘制头部 */
void AppDelegate::DrawHead(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);	/**< 白色 */
		glTranslatef(xPos, yPos, zPos);
		glScalef(2.0f, 2.0f, 2.0f);		/**<头部是 2x2x2长方体 */
		DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

/** 绘制机器人的躯干 */ 
void AppDelegate::DrawTorso(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f);	 /**< 蓝色 */
		glTranslatef(xPos, yPos, zPos);
		glScalef(3.0f, 5.0f, 2.0f);	     /**< 躯干是3x5x2的长方体 */
		DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

/** 绘制一条腿 */
void AppDelegate::DrawLeg(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glColor3f(1.0f, 1.0f, 0.0f);	/**< 黄色 */
		glTranslatef(xPos, yPos, zPos);
		glScalef(1.0f, 5.0f, 1.0f);		/**< 腿是1x5x1长方体 */
		DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

/** 在指定位置绘制机器人 */
void AppDelegate::DrawRobot(float xPos, float yPos, float zPos)
{
	static bool leg1 = true;		/**< 机器人腿的状态，true向前，flase向后 */
	static bool leg2 = false;		
	
	static bool arm1 = true;
	static bool arm2 = false;

	glPushMatrix();	

		glTranslatef(xPos, yPos, zPos);	/**< 定位 */

	    ///绘制各个部分
		DrawHead(1.0f, 2.0f, 0.0f);     /**< 绘制头部 */		
		DrawTorso(1.5f, 0.0f, 0.0f);   /**< 绘制躯干 */
		
		///绘制胳膊
		glPushMatrix();
			///如果胳膊正在向前运动，则递增角度，否则递减角度 
			if (arm1)
				_armAngle[0] = _armAngle[0] + 0.1f;
			else
				_armAngle[0] = _armAngle[0] - 0.1f;

			///如果胳膊达到其最大角度则改变其状态
			if (_armAngle[0] >= 15.0f)
					arm1 = false;
			if (_armAngle[0] <= -15.0f)
					arm1 = true;

			///平移并旋转后绘制胳膊
			glTranslatef(0.0f, -0.5f, 0.0f);
			glRotatef(_armAngle[0], 1.0f, 0.0f, 0.0f);
			DrawArm(2.5f, 0.0f, -0.5f);
		glPopMatrix();

		///同上arm0
		glPushMatrix();
			
			if (arm2)
				_armAngle[1] = _armAngle[1] + 0.1f;
			else
				_armAngle[1] = _armAngle[1] - 0.1f;

			
			if (_armAngle[1] >= 15.0f)
					arm2 = false;
			if (_armAngle[1] <= -15.0f)
					arm2 = true;

			
			glTranslatef(0.0f, -0.5f, 0.0f);
			glRotatef(_armAngle[1], 1.0f, 0.0f, 0.0f);
			DrawArm(-1.5f, 0.0f, -0.5f);
		glPopMatrix();

		///绘制腿部
		glPushMatrix();					

			///如果腿正在向前运动，则递增角度，否则递减角度 
			if (leg1)
				_legAngle[0] = _legAngle[0] + 0.1f;
			else
				_legAngle[0] = _legAngle[0] - 0.1f;

			///如果腿达到其最大角度则改变其状态
			if (_legAngle[0] >= 15.0f)
					leg1 = false;
			if (_legAngle[0] <= -15.0f)
					leg1 = true;

			///平移并旋转后绘制胳膊
			glTranslatef(0.0f, -0.5f, 0.0f);
			glRotatef(_legAngle[0], 1.0f, 0.0f, 0.0f);
			DrawLeg(-0.5f, -5.0f, -0.5f);

		glPopMatrix();

		///同上leg1
		glPushMatrix();

			if (leg2)
				_legAngle[1] = _legAngle[1] + 0.1f;
			else
				_legAngle[1] = _legAngle[1] - 0.1f;

			if (_legAngle[1] >= 15.0f)
				leg2 = false;
			if (_legAngle[1] <= -15.0f)
				leg2 = true;

			glTranslatef(0.0f, -0.5f, 0.0f);
			glRotatef(_legAngle[1], 1.0f, 0.0f, 0.0f);
			DrawLeg(1.5f, -5.0f, -0.5f);

		glPopMatrix();
	glPopMatrix();
}


void AppDelegate::Update(DWORD milliseconds)
{
	_angle += (float)(milliseconds) / 15.0f;

	if(_angle > 360.0f)
	{
		_angle = 0.0f;
	}
}


void AppDelegate::Draw()
{
	//用户自定义的绘制过程
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	///进行模型变换并绘制机器人
	glPushMatrix();
	  glTranslatef(0.0f, 0.0f, -30.0f);	
	  glRotatef(_angle, 0.0f, 1.0f, 0.0f);
	  DrawRobot(0.0f, 0.0f, 0.0f);
    glPopMatrix();

	glFlush();    //强制执行所有的OpenGL命令
}


