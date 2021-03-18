#include "AppDelegate.h"

 #include <stdio.h>
#include <gl.h>
#include <glu.h>
#include <glaux.h>



Application* Application::Create(const char* pszClassName)
{
	AppDelegate* app = new AppDelegate(pszClassName);
	return reinterpret_cast<Application*>(app);
}


AppDelegate::AppDelegate(const char* pszClassName)
	: Application(pszClassName)
{
	m_fAngle = 0.0f;
}

bool AppDelegate::Init()
{
	glClearColor(0.2f, 0.2f, 0.2f, 0.5f);
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
	if (m_keys.IsPressed(VK_ESCAPE))
	{
		TerminateApplication();
	}

	if (m_keys.IsPressed(VK_F1))
	{
		ToggleFullScreen();
	}

	m_fAngle += (float)(milliseconds) / 15.0f;
}


void AppDelegate::Draw()
{
	//用户自定义的绘制过程
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -6.0f);

	//基于右手坐标系
	glRotatef(m_fAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(m_fAngle * 0.7f, -1.0f, 0.0f, 0.0f);


	for (int rot1 = 0; rot1 < 2; rot1++)  //绘制四个交错的三角形（想的很巧妙啊）
	{
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
		for (int rot2 = 0; rot2 < 2; rot2++)
		{
			glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			glBegin(GL_TRIANGLES);		
				//按逆时针
				glColor3f(1.f, 0.f, 0.f);	glVertex3f( 0.0f,  1.0f, 0.0f);
				glColor3f(0.f, 1.f, 0.f);	glVertex3f(-1.0f, -1.0f, 1.0f);
				glColor3f(0.f, 0.f, 1.f);	glVertex3f( 1.0f, -1.0f, 1.0f);
			glEnd();
		}
	}
	glFlush();    //强制执行所有的OpenGL命令
}


