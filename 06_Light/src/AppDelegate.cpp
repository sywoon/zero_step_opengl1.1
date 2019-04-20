#include "AppDelegate.h"

#include <stdio.h>
#include <math.h>
#include <gl.h>
#include <glu.h>
#include <glaux.h>


//无法解析的外部符号 __imp__RegCloseKey@4
#pragma comment (lib, "Advapi32.lib")

//无法解析的外部符号 _sscanf
// vs2015默认编译时将许多标准库采用内联方式处理
#pragma comment (lib, "legacy_stdio_definitions.lib")



/** 定义三个光源（红色 绿色和蓝色）的位置 */
static float lightPositionR[] = { 0.0f, 0.0f, 5.0f, 1.0f };
static float lightPositionG[] = { 0.0f, 0.0f, 5.0f, 1.0f };
static float lightPositionB[] = { 0.0f, 0.0f, 5.0f, 1.0f };

/** 定义三个光源的漫射光 */
static float diffuseLightR[] = { 1.0f, 0.0f, 0.0f, 1.0f };
static float diffuseLightG[] = { 0.0f, 1.0f, 0.0f, 1.0f };
static float diffuseLightB[] = { 0.0f, 0.0f, 1.0f, 1.0f };

/** 定义三个光源的镜面光 */
static float specularLightR[] = { 1.0f, 0.0f, 0.0f, 1.0f };
static float specularLightG[] = { 0.0f, 1.0f, 0.0f, 1.0f };
static float specularLightB[] = { 0.0f, 0.0f, 1.0f, 1.0f };

/** 再定义一个默认的光源 */
static float diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };	 
static float specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };	 
static float lightPosition[] = { 0.0f, 0.0f, 10.0f, 1.0f };





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
	m_Angle = 0.0f;												/**< 设置初始角度为0 */
	m_bRedX = 0.0f;
	m_bGreenX = 0.0f;
	m_bBlueX = 0.0f;
	m_bRedY = 0.0f;
	m_bGreenY = 0.0f;
	m_bBlueY = 0.0f;
}

bool AppDelegate::Init()
{
	glClearColor(0.2f, 0.2f, 0.2f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	
	/** 设置0号光源 */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	
	/** 设置1号红色光源 */
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLightR);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLightR);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPositionR);
	
	/** 设置2号绿色光源 */
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLightG);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specularLightG);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPositionG);
	
	/** 设置3号蓝色光源 */
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseLightB);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specularLightB);
	glLightfv(GL_LIGHT3, GL_POSITION, lightPositionB);

	glEnable(GL_LIGHT0);     /**< 启用0号灰色光源,让物体可见 */

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	ResizeDraw(true);

	return true;
}

void AppDelegate::UnInit()
{
}


void AppDelegate::Update(DWORD milliseconds)
{
	/**　更新红色光源 */
	if(IsPressed(0x31) == false)       /**< 当数字'1'键没有被按下时 */
	{
		glEnable(GL_LIGHT1);                  /**< 启用1号红色光源 */
		m_bRedX += (float)milliseconds / 30;  /**< 更新旋转角度 */
		m_bRedY += (float)milliseconds / 20;
	}
	else
		glDisable(GL_LIGHT1);                 /**< 当被按下时，禁用该光源 */
 
	/**　更新绿色光源 */
	if(IsPressed(0x32) == false)        /**< 当数字'2'键没有被按下时 */
	{
		glEnable(GL_LIGHT2);                   /**< 启用2号绿色光源 */
		m_bGreenX += (float)milliseconds / 20; /**< 更新旋转角度 */
		m_bGreenY += (float)milliseconds / 30;
	}
	else
		glDisable(GL_LIGHT2);                 /**< 当被按下时，禁用该光源 */

	/**　更新蓝色光源 */
	if(IsPressed(0x33) == false)       /**< 当数字'3'键没有被按下时 */
	{
		glEnable(GL_LIGHT3);                  /**< 启用3号蓝色光源 */
		m_bBlueX += (float)milliseconds / 30; /**< 更新旋转角度 */
		m_bBlueY += (float)milliseconds / 30;
	}
	else
		glDisable(GL_LIGHT3);               /**< 当被按下时，禁用该光源 */

     m_Angle += (float)milliseconds / 30;   /**< 更新物体旋转角度 */
}


void AppDelegate::Draw()
{
	//用户自定义的绘制过程
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -10.0f); /**< 移入屏幕 */
	
	/** 红色光源 */
	glPushMatrix();
		/** 旋转红色光源球 */
		glRotatef(m_bRedX, 1.0f, 0.0f, 0.0f);
		glRotatef(m_bRedY, 0.0f, 1.0f, 0.0f);
		
		/** 设置红色光源位置 */
		glLightfv(GL_LIGHT1, GL_POSITION, lightPositionR);

		/**< 在光源位置处绘制一个球代表光源 */
		glTranslatef(lightPositionR[0], lightPositionR[1], lightPositionR[2]);
		glColor3f(1.0f, 0.0f, 0.0f);	/**< 红色 */
		
		glPushAttrib(GL_LIGHTING_BIT);
			glDisable(GL_LIGHTING);	/**< 关闭光照 */
			  auxSolidSphere(0.1);
			glEnable(GL_LIGHTING);  /**< 启用光照 */
		glPopAttrib();	
	glPopMatrix();


	/** 绿色光源 */
	glPushMatrix();
		/** 旋转绿色光源球 */
		glRotatef(m_bGreenX, 1.0f, 0.0f, 0.0f);
		glRotatef(m_bGreenY, 0.0f, 1.0f, 0.0f);
		
		/** 设置绿色光源位置 */
		glLightfv(GL_LIGHT2, GL_POSITION, lightPositionG);
	
		/**< 在光源位置处绘制一个球代表光源 */
		glTranslatef(lightPositionG[0], lightPositionG[1], lightPositionG[2]);
		glColor3f(0.0f, 1.0f, 0.0f);	/**< 绿色 */
		
		glPushAttrib(GL_LIGHTING_BIT);
			glDisable(GL_LIGHTING);	/**< 关闭光照 */
			  auxSolidSphere(0.1);
			glEnable(GL_LIGHTING);  /**< 启用光照 */
		glPopAttrib();	
	glPopMatrix();
	
	/** 蓝色光源 */
	glPushMatrix();
		
	    /** 旋转蓝色光源球 */
		glRotatef(m_bBlueX, 1.0f, 0.0f, 0.0f);
		glRotatef(m_bBlueY, 0.0f, 1.0f, 0.0f);

		/** 设置蓝色光源位置 */
		glLightfv(GL_LIGHT3, GL_POSITION, lightPositionB);
	
		/**< 在光源位置处绘制一个球代表光源 */
		glTranslatef(lightPositionB[0], lightPositionB[1], lightPositionB[2]);
		glColor3f(0.0f, 0.0f, 1.0f);	/**< 蓝色 */
		
		glPushAttrib(GL_LIGHTING_BIT);
			glDisable(GL_LIGHTING);	/**< 关闭光照 */
			  auxSolidSphere(0.1);
			glEnable(GL_LIGHTING);  /**< 启用光照 */
		glPopAttrib();	
	glPopMatrix();
	
	/** 绘制一个球体 */
	glPushMatrix();
		/**< 设置旋转 */
		glRotatef(m_Angle, 1.0f, 0.0f, 0.0f);
		glRotatef(m_Angle, 0.0f, 1.0f, 0.0f);
		glRotatef(m_Angle, 0.0f, 0.0f, 1.0f);
		glColor3f(1.0, 1.0, 1.0);
		auxSolidSphere(2.5);
	glPopMatrix();


	glFlush();    //强制执行所有的OpenGL命令
}


