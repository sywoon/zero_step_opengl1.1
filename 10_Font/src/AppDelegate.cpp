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
	cnt1 = 0.0;
	cnt2 = 0.0;	
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

	if(!font.InitFont(wglGetCurrentDC(), "Arial", 60)) 
	   return false;

	return true;
}

void AppDelegate::UnInit()
{
}


void AppDelegate::Update(DWORD milliseconds)
{
	cnt1 += milliseconds / 1500.0;  /**< 更新计数器值 */
	cnt2 += milliseconds / 1000.0;  /**< 更新计数器值 */
}


void AppDelegate::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	 /**< 清除缓存 */		
	glLoadIdentity();	
	
    /** 绘制字符过程 */
	glTranslatef(0.0f, 0.0f, -10.0f);                              /**< 移入屏幕 */
	glColor3f(cos(cnt1), sin(cnt2), 1.0f - 0.5f * cos(cnt1 + cnt2)); /**< 指定颜色 */
    
	/** 调用方法输出字符串 */
	font.PrintText("Hello,OpenGL!",-3.5f + 2.0 * cos(cnt1), 3.5f * sin(cnt2)); 
	
	
	/** 强制执行所有的OpenGL命令 */	
	glFlush();	
}


