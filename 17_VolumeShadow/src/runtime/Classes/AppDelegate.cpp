#include "AppDelegate.h"



static Application* s_pInstance = NULL;


/** 定义一些变量 */
Vector4 lightPos(2.0f, 5.0f, 0.0f, 1.0f);

#define MAX_SHADOW_CASTER_POINTS 4  /**< 定义遮挡体顶点个数 */
bool displayVolume = false;         /**< 绘制阴影体标志 */  
float extendAmount = 5.0f;          /**< 延伸因子 */

/** 遮挡体顶点数组 */
Vector4 ShadowObject[MAX_SHADOW_CASTER_POINTS];


Application* Application::create()
{
	if (s_pInstance)
		return s_pInstance;

	AppDelegate* app = new AppDelegate();
	s_pInstance = app;
	return reinterpret_cast<Application*>(app);
}

Application* Application::getInstance()
{
	return s_pInstance;
}

void Application::destroy()
{
	SAVE_FREE(s_pInstance);
}



AppDelegate::AppDelegate()
	: Application()
{
	_fps = 0;
}

bool AppDelegate::init()
{
	glClearColor(0.2f, 0.2f, 0.2f, 0.5f);						
	glClearDepth(1.0f);											
	glEnable(GL_DEPTH_TEST);

	/** 设置光源属性 */
	float ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
   float diffuseLight[] = {1.0, 1.0, 1.0, 1.0}; 
   float specularLight[] = {1.0, 1.0, 1.0, 1.0};

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
   glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
   glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	initGLExt();

	resizeDraw(true);

	glEnable(GL_TEXTURE_2D);
	
	if(!_ground.loadTexture("res/ground.tga"))
	{
		MessageBox(NULL,"载入纹理失败!","错误",MB_OK);
		exit(-1);
	}


	if(!_font.initFont())
	{
		Log("初始化字体失败!");
		return false;
	}

	ShadowObject[0] = Vector4(-1.0f, 2.5f, -1.0f);
   ShadowObject[1] = Vector4(-1.0f, 2.5f,  1.0f);
   ShadowObject[2] = Vector4(1.0f, 2.5f,  1.0f);
   ShadowObject[3] = Vector4(1.0f, 2.5f, -1.0f);

	return true;
}

void AppDelegate::unInit()
{
}


void AppDelegate::update(DWORD milliseconds)
{
	/** 控制光源位置移动 */
	if(isPressed(VK_UP))
		lightPos.z -= 0.01f;
	if(isPressed(VK_DOWN))
		lightPos.z += 0.01f;

	if(isPressed(VK_LEFT))
		lightPos.x -= 0.01f;

    if(isPressed(VK_RIGHT))
		lightPos.x += 0.01f;
   
	/** 空格键切换是否绘制阴影体 */
	if(isPressed(VK_SPACE))
	   displayVolume = !displayVolume;
}


void AppDelegate::caculateFrameRate()
{
	static float framesPerSecond    = 0.0f;	     /**< 保存显示帧数 */	
    static float lastTime			= 0.0f;	     /**< 记录上次时间 */						
    float currentTime = GetTickCount() * 0.001f; /**< 获得当前时间 */	 			

	framesPerSecond++;                           /**< 显示帧数递增1 */
    /** 如果时间差大于1.0秒 */
	if( currentTime - lastTime > 1.0f )          
    {
		
	    lastTime = currentTime;                   /**< 保存当前时间 */
		_fps = framesPerSecond;                  /**< 当前帧数传给_Fps */
        framesPerSecond = 0;                      /**< 将帧数置零 */                    
    }
}

/** 输出文字信息 */
void  AppDelegate::printText()
{
	char string[128];                               /**< 用于保存输出字符串 */
	glPushAttrib(GL_CURRENT_BIT);                   /**< 保存现有颜色属性信息 */
	glColor3f(0.0f,1.0f,0.0f);                      /**< 设置文字颜色 */
	
	/** 输出帧速 */
	caculateFrameRate();                               /**< 计算帧速 */
    sprintf(string,"FPS:%3.0f", _fps);                 /**< 字符串赋值 */
	_font.printText(string, -5.0f,3.0f);              /**< 输出字符串 */
	glPopAttrib();
}

void AppDelegate::render()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
  	glTranslatef(0.0f, -2.0f, -10.0f);
	
	/** 绘制地面 */
	glDisable(GL_LIGHTING);
	glPushMatrix();

	   /** 指定纹理 */
	   glBindTexture(GL_TEXTURE_2D, _ground.getID());

       ///绘制四边形
      glBegin(GL_QUADS);
		 glNormal3f(0.0f, 1.0f, 0.0f); /**< 指定法线 */
         glTexCoord2f(0.0, 0.0); glVertex3f(15.0f, -0.01f, 15.0f);
         glTexCoord2f(1.0, 0.0); glVertex3f(-15.0f, -0.01f, 15.0f);
         glTexCoord2f(1.0, 1.0); glVertex3f(-15.0f, -0.01f, -15.0f);
         glTexCoord2f(0.0, 1.0); glVertex3f(15.0f, -0.01f, -15.0f);
      glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);
	
	/** 绘制圆环 */
	glPushMatrix();
        glDisable(GL_TEXTURE_2D);
		glTranslatef(-2.0f, 0.8f, -0.2f);
		auxSolidTorus(0.2f, 0.4f);
	glPopMatrix();

   /** 绘制立方体 */
	glPushMatrix();
		glTranslatef(0.0f, 0.8f, -0.2f );
		auxSolidCube(1.0f);
	glPopMatrix();

   /** 绘制球体 */
	glPushMatrix();
		glTranslatef(2.0f, 0.8f, -0.2f);
		auxSolidSphere(0.7f);
    glPopMatrix();

   /** 绘制遮挡体 */
   glPushMatrix();
     glBegin(GL_QUADS);
  			glNormal3f(0, 1, 0);
        	glVertex3f(ShadowObject[0].x, ShadowObject[0].y, ShadowObject[0].z);
        	glVertex3f(ShadowObject[1].x, ShadowObject[1].y, ShadowObject[1].z);
			glVertex3f(ShadowObject[2].x, ShadowObject[2].y, ShadowObject[2].z);
			glVertex3f(ShadowObject[3].x, ShadowObject[3].y, ShadowObject[3].z);
	glEnd();
  glPopMatrix();


   /** 绘制光源 */
	glDisable(GL_LIGHTING);
	glPushMatrix();

	    /** 设置光源位置 */
		float lp[4];		
		
		lp[0] = lightPos.x; 
		lp[1] = lightPos.y;
		lp[2] = lightPos.z; 
		lp[3] = lightPos.w;
		glLightfv(GL_LIGHT0, GL_POSITION, lp);
	
		/** 开始绘制 */
		glTranslatef(lightPos.x,lightPos.y, lightPos.z);
		glColor3f(1.0f, 1.0f, 0.5f);
		auxSolidSphere(0.1);
		glColor3f(1.0f,1.0f,1.0f);  
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

}

/** 绘制物体 */
void AppDelegate::drawSilhouette(Vector4 lightPos, float Extend)
{
	Vector4 extended;     
   int A, B;

   /** 关闭光照 */
   glDisable(GL_LIGHTING);

   /** 设置颜色 */
   glColor3f(0.5f, 0.7f, 0.5f);

   /** 对每个顶点都延伸 */
   for(int i = 0; i < MAX_SHADOW_CASTER_POINTS; i++)
      {
          
          A = i; B = i + 1;

          /** 边界检查 */
          if(B >= MAX_SHADOW_CASTER_POINTS) B = 0;

         /** 绘制四边形 */
         glBegin(GL_QUADS);

            /** 原始顶点A */
            glVertex3f(ShadowObject[A].x, ShadowObject[A].y, ShadowObject[A].z);

            /** 顶点A延伸后的点 */
            extended.extendVertexPos(ShadowObject[A], lightPos, Extend);
            glVertex3f(extended.x, extended.y, extended.z);

            /** 顶点B延伸后的点 */
            extended.extendVertexPos(ShadowObject[B], lightPos, Extend);
            glVertex3f(extended.x, extended.y, extended.z);

            /** 原始顶点B */
            glVertex3f(ShadowObject[B].x, ShadowObject[B].y, ShadowObject[B].z);

         glEnd();
      }

   glEnable(GL_LIGHTING);

}


void AppDelegate::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);			
	glLoadIdentity();
    
	/** 关闭颜色缓存写入 */
	 glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	 
	 /** 渲染场景 */
	 render();
	

	/** 指定模版缓存操作 */
    glEnable(GL_CULL_FACE);
    glEnable(GL_STENCIL_TEST);   
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_ALWAYS, 0, 0);

	/** 绘制阴影体,模版值加1 */   
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	glCullFace(GL_BACK);
	drawSilhouette(lightPos, extendAmount);

	/** 再次绘制阴影体,模版值减1 */
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
	glCullFace(GL_FRONT);
	drawSilhouette(lightPos, extendAmount);

	///恢复以前的属性
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	/** 绘制阴影部分 */
    glDisable(GL_LIGHT0);
	glStencilFunc(GL_EQUAL, 1, 1);
	render();

	/** 绘制阴影体外的部分 */
    glEnable(GL_LIGHT0);
	glStencilFunc(GL_EQUAL, 0, 1);
	render();

	/** 关闭模版测试 */
	glDepthFunc(GL_LESS);
	glDisable(GL_STENCIL_TEST);

    /** 绘制阴影体 */
	if(displayVolume == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   /**< 指定线框模式 */
		drawSilhouette(lightPos, extendAmount);      /**< 绘制阴影体 */
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   /**< 恢复填充模式 */
	}

	/** 输出屏幕信息 */ 
	printText();
		
	glFlush();	
}


