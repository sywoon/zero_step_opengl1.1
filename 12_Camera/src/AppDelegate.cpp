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
	_fps = 0;
	_rot = 0.0f;
}


bool AppDelegate::LoadTexture()
{
	if(!_texture.LoadTexture("res/image.bmp"))
	{
		Log("装载位图文件失败！");
		return false;
	}

	if(!_texture2.LoadTexture("res/bitmap.bmp"))
	{
		Log("装载位图文件失败！");
		return false;
	}

	/** 启用纹理映射 */
	glEnable(GL_TEXTURE_2D);
  	return true;
}

/** 设置光源 */
void AppDelegate::SetLight()
{
    /** 定义光源的属性值 */
	GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; 	/**< 环境光参数 */
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };		/**< 漫射光参数 */
	GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f };	/**< 镜面光参数 */
	GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };	/**< 光源位置 */

	/** 设置光源的属性值 */
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		/**< 设置环境光 */
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		/**< 设置漫射光 */
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);	/**< 设置漫射光 */
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);	/**< 设置光源位置 */
	
	/** 启用光源 */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1); 
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

	if(!_font.InitFont("宋体", 20))
	{
		Log("初始化字体失败!");
	}
	
	if(!LoadTexture())
	{
		Log("载入纹理失败!");
	}
	
	_camera.setCamera(0.0f,1.5f, 6.0f, 0.0f, 1.5f, 0.0f,0.0f, 1.0f, 0.0f);

	SetLight();

	return true;
}

void AppDelegate::UnInit()
{
	_texture.FreeImage();
	glDeleteTextures(1, &_texture.getID()); 

	_texture2.FreeImage();
	glDeleteTextures(1, &_texture2.getID()); 
}

void AppDelegate::UpdateCamera()
{
	_camera.setViewByMouse();
	
	if(IsPressed(VK_SHIFT))                      /**< 按下SHIFT键时加速 */
	{
		_camera.setSpeed(0.6f);
	} else {
		_camera.setSpeed(0.2f);
	}

	if(IsPressed(VK_UP) || IsPressed('W'))   /**< 向上方向键或'W'键按下 */
		_camera.moveCamera(_camera.getSpeed());          /**< 移动摄像机 */

	if(IsPressed(VK_DOWN) || IsPressed('S')) /**< 向下方向键或'S'键按下 */
		_camera.moveCamera(-_camera.getSpeed());         /**< 移动摄像机 */

	if(IsPressed(VK_LEFT) || IsPressed('A')) /**< 向左方向键或'A'键按下 */
		_camera.yawCamera(-_camera.getSpeed());          /**< 移动摄像机 */

	if(IsPressed(VK_RIGHT) || IsPressed('D')) /**< 向右方向键或'D'键按下 */
		_camera.yawCamera(_camera.getSpeed());            /**< 移动摄像机 */
}

void AppDelegate::Update(DWORD milliseconds)
{
	_rot += milliseconds / 20.0;
	UpdateCamera();
}


/** 计算帧速 */
void AppDelegate::CaculateFrameRate()
{
	static float framesPerSecond    = 0.0f;	     /**< 保存显示帧数 */	
    static float lastTime			= 0.0f;	     /**< 记录上次时间 */						
    float currentTime = GetTickCount() * 0.001f; /**< 获得当前时间 */	 			

	++framesPerSecond;                           /**< 显示帧数递增1 */
    /** 如果时间差大于1.0秒 */
	if( currentTime - lastTime > 1.0f )          
    {
		
	    lastTime = currentTime;                   /**< 保存当前时间 */
		_fps = framesPerSecond;                  /**< 当前帧数传给m_Fps */
        framesPerSecond = 0;                      /**< 将帧数置零 */                    
    }
}

/** 输出文字信息 */
void  AppDelegate::PrintText()
{
	char string[128];                               /**< 用于保存输出字符串 */
	glPushAttrib(GL_CURRENT_BIT);                   /**< 保存现有颜色属性信息 */

	glPushMatrix(); 	
	glTranslatef(0.0f, 0.0f, -10.0f); 

	glColor3f(0.0f,1.0f,0.0f);                      /**< 设置文字颜色 */
	sprintf(string,"当前位置:X=%3.1f  Y=%3.1f Speed =%3.1f ",   
		     _camera.getView().x,-_camera.getView().z ,_camera.getSpeed()); /**< 字符串赋值 */
	
	_font.PrintText(string, -3.0f, 3.5f);

	/** 输出帧速 */
	CaculateFrameRate();                               /**< 计算帧速 */
    sprintf(string,"FPS:%3.0f",_fps);                 /**< 字符串赋值 */
	_font.PrintText(string, -3.0f, 3.0f);              /**< 输出字符串 */
	
	glPopMatrix();
	glPopAttrib();
}

/** 绘制网格地面 */
void AppDelegate::DrawGrid()
{
    /** 获得场景中一些状态  */
	GLboolean  lp,tp;
	glGetBooleanv(GL_LIGHTING,&lp);  
	glGetBooleanv(GL_TEXTURE_2D,&tp);
	
	/** 关闭纹理和光照 */
	glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
	
	/** 绘制过程 */
	glPushAttrib(GL_CURRENT_BIT);   /**< 保存当前属性 */
    glPushMatrix();                 /**< 压入堆栈 */
	glTranslatef(0.0f,0.0f,0.0f);  
	glColor3f(0.0f, 0.0f, 1.0f);    /**< 设置颜色 */

	/** 在X,Z平面上绘制网格 */
	for(float i = -50; i <= 50; i += 1)
	{
		/** 绘制线 */
		glBegin(GL_LINES);

		    /** X轴方向 */
			glVertex3f(-50, 0, i);
			glVertex3f(50, 0, i);

			/** Z轴方向 */
			glVertex3f(i, 0, -50);
			glVertex3f(i, 0, 50);

		glEnd();
	}
	glPopMatrix();
	glPopAttrib();
	
	/** 恢复场景状态 */
	if(tp)
	  glEnable(GL_TEXTURE_2D);
	if(lp)
      glEnable(GL_LIGHTING);
}

/** 绘制球体 */
void AppDelegate::DrawSphere()
{
	/** 设置材质属性 */
	GLfloat mat_ambient[] = { 0.9f, 0.5f, 0.8f, 1.0f };
    GLfloat mat_diffuse[] = { 0.9f, 0.5f, 0.8f, 1.0f };
	GLfloat mat_shininess[] = { 100.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
	/** 获得纹理启用状态 */
	GLboolean tp;
	glGetBooleanv(GL_TEXTURE_2D,&tp);
    glDisable(GL_TEXTURE_2D);                   /**< 关闭纹理 */
    
	/** 绘制过程 */
	glPushMatrix();
    glTranslatef(-5.0f, 2.0f,-10.0f);
    GLUquadricObj * sphere = gluNewQuadric();
    gluQuadricOrientation(sphere, GLU_OUTSIDE);
	gluQuadricNormals(sphere,GLU_SMOOTH);
	gluSphere(sphere,2.0,50,50);
	gluDeleteQuadric(sphere);
    glPopMatrix();
    
	/** 恢复状态 */
	if(tp)
	   glEnable(GL_TEXTURE_2D);


	glBindTexture(GL_TEXTURE_2D, _texture2.getID());

	// 绘制第二个
	glPushMatrix();
	    glTranslatef(0.0f, 2.0f, -10.0f);
	    glRotatef(_rot, 0.0f, 1.0f, 1.0f);

	    GLUquadricObj * sphere2 = gluNewQuadric();
	    gluQuadricOrientation(sphere2, GLU_OUTSIDE);
		gluQuadricNormals(sphere2,GLU_SMOOTH);
		gluQuadricTexture(sphere2, GL_TRUE);  //必须 否则不显示纹理
		gluSphere(sphere2, 2.0, 50, 50);
		gluDeleteQuadric(sphere2);
    glPopMatrix();
	
}

/** 绘制木箱 */
void AppDelegate::DrawBox()
{
	/** 设置材质属性 */
	GLfloat mat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glPushMatrix();
	glTranslatef(5.0f,2.0f,-10.0f);
	glScalef(2.0f,2.0f,2.0f);
	
	/** 选择纹理 */
	glBindTexture(GL_TEXTURE_2D, _texture.getID());
	
	/** 开始绘制四边形 */
	glBegin(GL_QUADS);												
		
	    /// 前侧面
		glNormal3f( 0.0f, 0.0f, 1.0f);								/**< 指定法线指向观察者 */
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		
		/// 后侧面
		glNormal3f( 0.0f, 0.0f,-1.0f);								/**< 指定法线背向观察者 */
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		
		/// 顶面
		glNormal3f( 0.0f, 1.0f, 0.0f);								/**< 指定法线向上 */
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		
		/// 底面
		glNormal3f( 0.0f,-1.0f, 0.0f);								/**< 指定法线朝下 */
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		
		/// 右侧面
		glNormal3f( 1.0f, 0.0f, 0.0f);								/**< 指定法线朝右 */
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		
		/// 左侧面
		glNormal3f(-1.0f, 0.0f, 0.0f);								/**< 指定法线朝左 */
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
	glEnd();
	glPopMatrix();
}

void AppDelegate::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();	
	
	/** 放置摄像机 */	
	_camera.setLook();
	
	/** 绘制过程 */
	DrawGrid();
    DrawSphere();
	DrawBox();
	
	/** 输出文字信息 */
	PrintText();
	
	/** 强制执行所有的OpenGL命令 */	
	glFlush();		
}


