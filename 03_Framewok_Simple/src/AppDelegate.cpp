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
	_fAngle = 0.0f;
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


void AppDelegate::Update(DWORD milliseconds)
{
	_fAngle += (float)(milliseconds) / 15.0f;

	if(_fAngle > 360.0f)
	{
		_fAngle = 0.0f;
	}
}


void AppDelegate::Draw()
{
	//用户自定义的绘制过程
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	/** 画线 */
	glPushMatrix();
		glTranslatef(-1.5f, 1.0f, -6.0f);
		glScalef(0.75f, 0.75f, 0.75f);
		glRotatef(_fAngle, 1.0f, 1.0f, 1.0f);
		glColor3f(1.0f, 0.0f, 0.0f);							
		glBegin(GL_LINE_LOOP); /**< 绘制闭合线段 */
			glVertex3f( 0.0f, 1.0f,-0.0f); /**< 第一个点a */ 
			glVertex3f(-1.0f,-1.0f, 0.0f); /**< 第二个点b */ 
			glVertex3f( 1.0f,-1.0f, 0.0f); /**< 第三个点c */ 
		glEnd();
	glPopMatrix();

	/** 画圆 */
	glPushMatrix();	
		glTranslatef(1.5f, 1.0f, -6.0f);
		glScalef(0.75f,0.75f,0.75f);
		glRotatef(_fAngle,1.0f,1.0f,1.0f);
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLE_FAN); /**< 绘制三角形扇 */
			glVertex3f(0,0,0.0f );   
			for ( int i = 0; i <= 370; i += 10)
			{
				float p = (float)(i * 3.14 / 180);
				glVertex3f((float)sin(p), (float)cos(p), 0.0f );
			}
		glEnd();
	glPopMatrix();

	/** 画三角形 */
	glPushMatrix();	
		glTranslatef(-1.5f, -1.0f, -6.0f);
		glScalef(0.75f,0.75f,0.75f);
		glRotatef(_fAngle,1.0f,1.0f,1.0f);
		glColor3f(0.0f, 1.0f, 1.0f);							
		glBegin(GL_TRIANGLES);	/**< 绘制三角形 */							
			glVertex3f( 0.0f,  1.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glVertex3f( 1.0f, -1.0f, 0.0f);
		glEnd();
    glPopMatrix();

    /** 画正方形 */
	glPushMatrix();
		glTranslatef(1.5f,-1.0f,-6.0f);
		glScalef(0.75f,0.75f,0.75f);
		glRotatef(_fAngle,1.0f,1.0f,1.0f);
		glColor3f(1.f, 1.f, 0.f);
		glBegin(GL_QUADS);	/**< 绘制四边形 */
			glVertex3f(-1.0f, 1.0f, 0.0f);	/**< 左上 */
			glVertex3f( 1.0f, 1.0f, 0.0f);	/**< 右上 */				
			glVertex3f( 1.0f,-1.0f, 0.0f);	/**< 左下 */				
			glVertex3f(-1.0f,-1.0f, 0.0f);	/**< 右下 */				
		glEnd();					
	 glPopMatrix(); 

	glFlush();    //强制执行所有的OpenGL命令
}


