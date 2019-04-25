#include "Application.h"
#include <assert.h>
#include <gl.h>
#include <glu.h>


static CApplication* s_pInstance = NULL;


CApplication* CApplication::Create()
{
	assert(s_pInstance == NULL);

	s_pInstance = new CApplication();
	return s_pInstance;
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

CApplication::CApplication()
		: _fAngle(0.0f)
{
}

CApplication::~CApplication()
{
}

void CApplication::Update(DWORD milliseconds)
{
	_fAngle += (float)(milliseconds) / 15.0f;
}

void CApplication::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();
	//glTranslatef(0.0f, 0.0f, -6.0f);
	
	glPushMatrix();
	glRotatef(_fAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(_fAngle * 0.7f, -1.0f, 0.0f, 0.0f);

	for (int rot1 = 0; rot1 < 2; rot1++)
	{
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
		for (int rot2 = 0; rot2 < 2; rot2++)
		{
			glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			glBegin(GL_TRIANGLES);								
				glColor3f(1.f, 0.f, 0.f);	glVertex3f( 0.0f,  1.0f, 0.0f);
				glColor3f(0.f, 1.f, 0.f);	glVertex3f(-1.0f, -1.0f, 1.0f);
				glColor3f(0.f, 0.f, 1.f);	glVertex3f( 1.0f, -1.0f, 1.0f);
			glEnd();
		}
	}
	glPopMatrix();
	
	glFlush();
}


