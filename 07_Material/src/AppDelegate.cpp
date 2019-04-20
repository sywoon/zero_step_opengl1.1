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
}

bool AppDelegate::Init()
{
	///定义光源GL_LIGHT0
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 }; /**< 环境光 */
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; /**< 漫射光 */
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };/**< 镜面光 */
	GLfloat position[] = { 0.0, 3.0, 2.0, 0.0 };/**< 光源位置 */
	
	///设置光源属性
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	///初始化
	glClearColor(0.0f, 0.2f, 0.2f, 0.0f);
			
	glClearDepth(1.0f);	
	glDepthFunc(GL_LEQUAL);	
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_LIGHTING); /**< 启用光源 */
	glEnable(GL_LIGHT0);  /**< 启用0号光源 */                                  
	
	ResizeDraw(true);		

	return true;
}

void AppDelegate::UnInit()
{
}


void AppDelegate::Update(DWORD milliseconds)
{
}


void AppDelegate::Draw()
{
	///定义材质属性值
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 }; /** < 无材质颜色 */
	GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 }; /** < 环境颜色 */
	GLfloat mat_ambient_color[] = { 0.8, 0.6, 0.2, 1.0 };
	GLfloat mat_diffuse[] = { 0.2, 0.5, 0.8, 1.0 };/** < 散射颜色 */
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };/** < 镜面反射颜色 */
	GLfloat no_shininess[] = { 0.0 };              /** < 镜面反射指数为0 */
	GLfloat low_shininess[] = { 5.0 };             /** < 镜面反射指数为5.0 */
	GLfloat high_shininess[] = { 100.0 };          /** < 镜面反射指数为100.0 */
	GLfloat mat_emission[] = {0.3, 0.2, 0.3, 0.0}; /** < 发射光颜色 */

	//清除缓存并重置单位矩阵
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();	
	
	/** 第一行第一列绘制的球仅有漫反射光而无环境光和镜面光。*/
	glPushMatrix();
		glTranslatef (-2.0, 1.75, -6.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		auxSolidSphere(0.5);
	glPopMatrix();

	/** 第一行第二列绘制的球有漫反射光和镜面光，并有低高光，而无环境光 。*/
	glPushMatrix();
		glTranslatef (-0.75, 1.75, -6.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
        auxSolidSphere(0.5);
	glPopMatrix();

	/** 第一行第三列绘制的球有漫反射光和镜面光，并有很亮的高光，而无环境光 。*/
	glPushMatrix();
		glTranslatef (0.75, 1.75, -6.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		auxSolidSphere(0.5);
	glPopMatrix();

	/** 第一行第四列绘制的球有漫反射光和辐射光，而无环境和镜面反射光。*/
	glPushMatrix();
		glTranslatef (2.0, 1.75, -6.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		auxSolidSphere(0.5);
	glPopMatrix();

	/** 第二行第一列绘制的球有漫反射光和环境光，而无镜面反射光。*/
	glPushMatrix();
		glTranslatef (-2.0, 0.0, -6.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		auxSolidSphere(0.5);
	glPopMatrix();

	/** 第二行第二列绘制的球有漫反射光、环境光和镜面光，且有低高光。*/
	glPushMatrix();
		glTranslatef (-0.75, 0.0, -6.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		auxSolidSphere(0.5);
	glPopMatrix();

	/** 第二行第三列绘制的球有漫反射光、环境光和镜面光，且有很亮的高光。*/
	glPushMatrix();
		glTranslatef (0.75, 0.0, -6.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		auxSolidSphere(0.5);
	glPopMatrix();

	/** 第二行第四列绘制的球有漫反射光、环境光和辐射光，而无镜面光。*/
	glPushMatrix();
		glTranslatef (2, 0.0, -6.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		auxSolidSphere(0.5); 
	glPopMatrix();

	/** 第三行第一列绘制的球有漫反射光和有颜色的环境光，而无镜面光。*/
	glPushMatrix();
		glTranslatef (-2.0, -1.75, -6.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		auxSolidSphere(0.5);
	glPopMatrix();

	/** 第三行第二列绘制的球有漫反射光和有颜色的环境光以及镜面光，且有低高光。*/
	glPushMatrix();
		glTranslatef (-0.75, -1.75, -6.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		auxSolidSphere(0.5);
	glPopMatrix();

	/** 第三行第三列绘制的球有漫反射光和有颜色的环境光以及镜面光，且有很亮的高光。*/
	glPushMatrix();
		glTranslatef (0.75, -1.75, -6.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		auxSolidSphere(0.5);
	glPopMatrix();

	/** 第三行第四列绘制的球有漫反射光和有颜色的环境光以及辐射光，而无镜面光。*/
	glPushMatrix();
		glTranslatef (2, -1.75, -6.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		auxSolidSphere(0.5);
	glPopMatrix();

   glFlush();	
}


