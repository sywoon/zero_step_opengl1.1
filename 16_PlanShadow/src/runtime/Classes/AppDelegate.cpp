#include "AppDelegate.h"



static Application* s_pInstance = NULL;


/** 定义一些变量 */
float lightPos[4] = {0.0f, 0.0f, 13.0f, 1.0f};  /**< 光源位置 */
float direction = -0.06f;                       /**< X轴上移动增量 */

/** 下面这些变量控制3个球的位置 */
float objPos1;
float objPos2;
float objPos3;

float obj1 = -0.01f;
float obj2 = -0.02f;
float obj3 = -0.03f;


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
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	initGLExt();

	resizeDraw(true);


	if(!_font.initFont())
	{
		Log("初始化字体失败!");
		return false;
	}

	float size = 6.0f;	      /**< 定义墙的边长 */
	Wall wall;                /**< 定义结构体 */
	
	///创建左面
	wall.vVerts.clear();									/**< 清空顶点数组 */
	wall.vNormal = Vector3(1, 0, 0);						/**< 设置法向量 */
	wall.vVerts.push_back(Vector3(-size, size, size));		/**< 下面定义顶点 */
	wall.vVerts.push_back(Vector3(-size, -size, size));	
	wall.vVerts.push_back(Vector3(-size, -size, 0));		
	wall.vVerts.push_back(Vector3(-size, size, 0));		
	wall.vColor = Vector3(1, 0, 0);						    /**< 指定颜色 */
	walls.push_back(wall);								    /**< 添加该面到容器中 */

	///创建右面
	wall.vVerts.clear();									/**< 清空顶点数组 */
	wall.vNormal = Vector3(-1, 0, 0);						/**< 设置法向量 */
	wall.vVerts.push_back(Vector3(size, size, 0));			/**< 下面定义顶点 */
	wall.vVerts.push_back(Vector3(size, -size, 0));		
	wall.vVerts.push_back(Vector3(size, -size, size));		
	wall.vVerts.push_back(Vector3(size, size, size));		
	wall.vColor = Vector3(0, 0, 1);						    /**< 指定颜色 */
	walls.push_back(wall);								    /**< 添加该面到容器中 */
	
	
	///创建底面
	wall.vVerts.clear();									/**< 清空顶点数组 */
	wall.vNormal = Vector3(0, 1, 0);						/**< 设置法向量 */
	wall.vVerts.push_back(Vector3(size, -size, size));		/**< 下面定义顶点 */
	wall.vVerts.push_back(Vector3(size, -size, 0));		
	wall.vVerts.push_back(Vector3(-size, -size, 0));		
	wall.vVerts.push_back(Vector3(-size, -size, size));	
	wall.vColor = Vector3(0, 1, 0);						    /**< 指定颜色 */
	walls.push_back(wall);								    /**< 添加该面到容器中 */

	///创建顶面
	wall.vVerts.clear();									/**< 清空顶点数组 */
	wall.vNormal = Vector3(0, -1, 0);						/**< 设置法向量 */
	wall.vVerts.push_back(Vector3(size, size, size));		/**< 下面定义顶点 */
	wall.vVerts.push_back(Vector3(size, size, 0));		
	wall.vVerts.push_back(Vector3(-size,size, 0));		
	wall.vVerts.push_back(Vector3(-size,size, size));	
	wall.vColor = Vector3(0, 1, 1);						    /**< 指定颜色 */
	walls.push_back(wall);								    /**< 添加该面到容器中 */
	
	///创建后面
	wall.vVerts.clear();									/**< 清空顶点数组 */
	wall.vNormal = Vector3(0, 0, 1);						/**< 设置法向量 */
	wall.vVerts.push_back(Vector3(-size, size, 0));		    /**< 下面定义顶点 */
	wall.vVerts.push_back(Vector3(-size, -size, 0));		
	wall.vVerts.push_back(Vector3(size, -size, 0));		
	wall.vVerts.push_back(Vector3(size, size, 0));			
	wall.vColor = Vector3(1, 0, 1);						    /**< 指定颜色 */
	walls.push_back(wall);	 

	return true;
}

void AppDelegate::unInit()
{
}


void AppDelegate::update(DWORD milliseconds)
{
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
	char text[128];                               /**< 用于保存输出字符串 */
	glPushAttrib(GL_CURRENT_BIT);                 /**< 保存现有颜色属性信息 */

	const float logicWH = _win32.getLogicWidth() / 2;
	const float logicHH = _win32.getLogicHeight() / 2;

	glColor3f(0.0f, 1.0f, 1.0f);                  /**< 设置文字颜色 */
	sprintf(text,"当前位置:[%.1f,%.f,%.1f] Speed =%2.1f",   
				_camera.getPosition().x,
				_camera.getPosition().y,
				_camera.getPosition().z, 
				_camera.getSpeed());
	_font.printText(text, -logicWH, -logicHH, 0, 0);

	/** 输出帧速 */
    caculateFrameRate();                            /**< 计算帧速 */
    sprintf(text, "FPS:%d", (int)_fps);             /**< 字符串赋值 */
	// _font.printText(text, -0.0f, logicHH, 0, 2);    /**< 输出字符串 */
	_font.printText(text, -5, 3, 0, 2); 

	_font.printText("O", 0.0f, 0.0f, 1, 1);
	
	glPopAttrib();
}
void AppDelegate::renderWall(const Wall& wall)
{
	glPushMatrix();
	    /** 开始绘制 */
		glBegin(GL_QUADS);
			
			/** 指定法线 */
			glNormal3f(wall.vNormal.x, wall.vNormal.y, wall.vNormal.z);

			/** 指定颜色 */
			glColor4f(wall.vColor.x, wall.vColor.y, wall.vColor.z, 1.0);

			/** 指定顶点 */			 
			for(int i = 0; i < (int)wall.vVerts.size(); i++)
			{
				glVertex3f(wall.vVerts[i].x, wall.vVerts[i].y, wall.vVerts[i].z);
			}

		/** 绘制结束 */
		glEnd();

	glPopMatrix();

}

/** 绘制物体 */
void AppDelegate::renderObjects()
{
	/** 创建一个二次对象指针 */
    GLUquadricObj *pObj = gluNewQuadric();			
	gluQuadricDrawStyle(pObj, GLU_FILL);
	

	/** 绘制球体1 */
	glColor4f(0.0f, 1.0f, 0.0f, 1.0);    /**< 指定颜色 */

	glPushMatrix();
		glTranslatef(-3.0f, objPos1, 4.0f);
		gluSphere(pObj, 0.5f, 75, 75);
	glPopMatrix();
	

	/** 绘制球体2 */
	glColor4f(1.0f, 1.0f, 0.1f, 1.0);   /**< 指定颜色 */

	glPushMatrix();
		glTranslatef(3, objPos2, 4);
		gluSphere(pObj, 0.5f, 75, 75);
	glPopMatrix();

	
	/** 绘制球体3 */
	glColor4f(0.0f, 0.0f, 1.0f, 1.0);   /**< 指定颜色 */
	glPushMatrix();
		glTranslatef(0, objPos3, 4);
		gluSphere(pObj, 0.5f, 75, 75);
	glPopMatrix();

	/** 下面是对物体位置进行限定 */
	///球体1
	if(objPos1 < -4.5f)
	{
		obj1 = obj1 * -1;
		objPos1 = -4.5f; 
	}
	if(objPos1 > 4.5f)
	{
		obj1 = obj1 * -1;
		objPos1 = 4.5f; 
	}
	
	///球体2
	if(objPos2 < -4.5f)
	{
		obj2 = obj2 * -1;
		objPos2 = -4.5f; 
	}
	if(objPos2 > 4.5f)
	{
		obj2 = obj2 * -1;
		objPos2 = 4.5f; 
	}	

	///球体3
	if(objPos3 < -4.5f)
	{
		obj3 = obj3 * -1;
		objPos3 = -4.5f; 
	}
	if(objPos3 > 4.5f)
	{
		obj3 = obj3 * -1;
		objPos3 = 4.5f; 
	}	
	
	/** 更新位置 */
	objPos1 += obj1;
	objPos2 += obj2;
	objPos3 += obj3;

	/** 恢复颜色 */
	glColor4f(1.0f, 1.0f, 1.0f, 1.0);

	/** 删除二次对象 */
	gluDeleteQuadric(pObj);

}

/** 绘制光源 */
void AppDelegate::renderLight()
{
	/** 关闭光照 */
	glDisable(GL_LIGHTING);

	glPushMatrix();
		GLUquadricObj *pObj = gluNewQuadric();				/**< 创建二次对象 */
		gluQuadricDrawStyle(pObj, GLU_FILL);				

		///在光源位置处绘制一个黄色小球
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
		glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
		gluSphere(pObj, 0.03f, 25, 25);
		
		gluDeleteQuadric(pObj);								/**< 删除二次对象 */
	glPopMatrix();

	/** 开启光照 */
	glEnable(GL_LIGHTING);

	/** 对光源位置移动位置和方向进行更新 */
	if(lightPos[0] < -8.0f)
	{
		direction = direction * -1;
		lightPos[0] = -8.0f; 
	}
	if(lightPos[0] > 8.0f)
	{
		direction = direction * -1;
		lightPos[0] = 8.0f; 
	}	

	/** 更新光源位置 */
	lightPos[0] += direction;
	glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
}

/** 创建投射矩阵 */
void AppDelegate::createShadowMatrix(float m[16], Vector3 point, Vector3 normal, float lp[4])
{
	/** 计算顶点到平面的距离 */
	float d = - ((normal.x * point.x) + (normal.y * point.y) + (normal.z * point.z));
	
	/** 计算光源向量和法向量的点积 */
	float dot = normal.x*lp[0]  + normal.y*lp[1] + normal.z*lp[2] + d*lp[3];

	/** 设置矩阵元素值 */
	m[0]  = dot - lp[0]*normal.x;  m[1]  =	   -lp[1]*normal.x;	 m[2]  =      -lp[2]*normal.x;  m[3]  =	    -lp[3]*normal.x;
	m[4]  =		 -lp[0]*normal.y;  m[5]  = dot -lp[1]*normal.y;  m[6]  =	  -lp[2]*normal.y;  m[7]  =	    -lp[3]*normal.y;
	m[8]  =		 -lp[0]*normal.z;  m[9]  =	   -lp[1]*normal.z;	 m[10] = dot  - lp[2]*normal.z; m[11] =	    -lp[3]*normal.z;
	m[12] =		 -lp[0]*d;	       m[13] =	   -lp[1]*d;		 m[14] =	  -lp[2]*d;	        m[15] = dot -lp[3]*d;

}


/** 渲染墙面和阴影 */
void  AppDelegate::renderShadow()
{
	glClear(GL_STENCIL_BUFFER_BIT);							/**< 清除模版缓存 */
	glEnable(GL_STENCIL_TEST);								/**< 开始模版测试 */

	/** 循环处理每个墙面 */
	for(int i = 0; i < (int)walls.size(); i++)
	{
		glStencilFunc(GL_ALWAYS, 1, 1);						/**< 设置每个像素模版值为1 */
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);			

		/** 绘制当前墙面 */
		glDisable(GL_LIGHT0);							
		renderWall(walls[i]);								
		glEnable(GL_LIGHT0);                          

		glDisable(GL_DEPTH_TEST);							/**< 关闭深度测试 */
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);/**< 禁止颜色缓存写 */
		glStencilFunc(GL_EQUAL, 1, 1);                      /**< 绘制模版值为1的象素 */
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);             /**< 模版值加1 */

		/** 开始投射阴影 */
		glPushMatrix();

			/** 创建投射矩阵 */
			float m[16] = {0};								
			createShadowMatrix(m, walls[i].vVerts[0], walls[i].vNormal, lightPos);
			glMultMatrixf(m);

			/** 绘制物体,得到阴影 */
			glPushMatrix();
			    renderObjects();
			glPopMatrix();

		glPopMatrix();

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);	/**< 恢复颜色缓存写 */

		renderWall(walls[i]);								/**< 绘制除阴影外的部分 */
		glEnable(GL_DEPTH_TEST);							/**< 开始深度测试 */
	}

	/** 关闭模版测试 */
	glDisable(GL_STENCIL_TEST);  				

}

void AppDelegate::draw()
{
	//用户自定义的绘制过程
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
    glTranslatef(0.0f,0.0f,-18.0f);
	
	/** 绘制物体 */
	renderObjects();
	
	/** 渲染墙面和阴影 */
	renderShadow();

	/** 绘制光源 */
	renderLight();
	
	/** 输出屏幕信息 */ 
	printText();
		
	glFlush();	
}


