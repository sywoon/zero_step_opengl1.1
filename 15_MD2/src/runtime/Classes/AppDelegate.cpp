#include "AppDelegate.h"



static Application* s_pInstance = NULL;



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
	_renderMode = true;
	_sp = false;
	_rot = 0;
}

bool AppDelegate::init()
{
	glClearColor(0.2f, 0.2f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);	
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	initGLExt();

	resizeDraw(true);


	if(!_font.initFont())
	{
		Log("初始化字体失败!");
		return false;
	}

	if (!_texture.loadTexture("../res/cloud.bmp"))
	{
		Log("装载位图文件失败!");
		return false;
	}
	
	
	/** 初始化地形 */	
	if(!_terrain.init())
	{
		Log("初始化地形失败!");
		return false;
	}

	/** 初始化天空 */
	if(!_skyBox.init())
	{
		Log("初始化天空失败!");
		return false;
	}

	_3dsLoader.init("../res/model.3ds");
	_md2.init("../res/hobgoblin.md2", "../res/hobgoblin.bmp");

	_camera.setCamera(500,35,400, 501,35,400, 0,1,0);
	_camera.setSpeed(3.0f);

	return true;
}

void AppDelegate::unInit()
{
	_texture.freeTexture();
}

void AppDelegate::updateCamera()
{
	_camera.setViewByMouse();
	
	/** 键盘按键响应 */
	if(isPressed(VK_SHIFT))                        /**< 按下SHIFT键时加速 */
	{
		_camera.setSpeed(10.0f);
	}
	if(!isPressed(VK_SHIFT))
	{
		_camera.setSpeed(3.0f);
	}

	Vector3 vecPosOld = _camera.getPosition();
	Vector3 vecViewOld = _camera.getView();
	Vector3 vecUpOld = _camera.getUpVector();

	if(isPressed(VK_UP) || isPressed('W'))   /**< 向上方向键或'W'键按下 */
		_camera.moveCamera(_camera.getSpeed());          /**< 移动摄像机 */

	if(isPressed(VK_DOWN) || isPressed('S')) /**< 向下方向键或'S'键按下 */
		_camera.moveCamera(-_camera.getSpeed());         /**< 移动摄像机 */

	if(isPressed(VK_LEFT) || isPressed('A')) /**< 向左方向键或'A'键按下 */
		_camera.yawCamera(-_camera.getSpeed());          /**< 移动摄像机 */

	if(isPressed(VK_RIGHT) || isPressed('D')) /**< 向右方向键或'D'键按下 */
		_camera.yawCamera(_camera.getSpeed());            /**< 移动摄像机 */

	/** 根据地形高度更新摄像机 */
	Vector3 vPos = _camera.getPosition();                  /**< 得到当前摄像机位置 */
	if (!_terrain.checkPos(vPos.x, vPos.z))
	{
		_camera.setCamera(vecPosOld, vecViewOld, vecUpOld);
		return;
	}

    Vector3 vNewPos = vPos; 
	/** 设置摄像机高度为 地形高度 + 10 */
	vNewPos.y = (float)_terrain.getAveHeight(vPos.x, vPos.z) + 10;

	/** 得到高度差值 */
	float temp = vNewPos.y - vPos.y;

	/** 更新摄像机方向 */
	Vector3 vView = _camera.getView();
	vView.y += temp;

	/** 设置摄像机 */
	_camera.setCamera(vNewPos.x,  vNewPos.y,  vNewPos.z,
					   vView.x,	   vView.y,	   vView.z,	 
					   0, 1, 0);							
	
}

void AppDelegate::update(DWORD milliseconds)
{
	updateCamera();
	_rot += milliseconds / 20.0;

	/** 空格键切换绘制模式 */
	if(isPressed(VK_SPACE))
	{
		if (_sp)
		{
			_sp = false;
		} else {
			_sp = true;
			_renderMode = !_renderMode;
			if(_renderMode)
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}

	/** 'M'键播放下一个动作 */
	if(isPressed('M') && !_mp)
	{
		_mp = true;
		
		/** 设置当前动作为下一个动作 */
		_md2.getModel().currentAnim = (_md2.getModel().currentAnim + 1) % (_md2.getModel().numOfAnimations);
		
		/** 设置当前帧为下一个动作的开始帧 */
		_md2.getModel().currentFrame = (_md2.getModel()).pAnimations[_md2.getModel().currentAnim].startFrame;
	}

	if(!isPressed('M'))
	{
		_mp = false;
	}
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
		_fps = framesPerSecond;                  /**< 当前帧数传给m_Fps */
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
	_font.printText(text, -0.0f, logicHH, 0, 2);             /**< 输出字符串 */

	_font.printText("O", 0.0f, 0.0f, 1, 1);
	
	glPopAttrib();
}

void AppDelegate::drawCoordinate()
{
	/** 获得场景中一些状态  */
	GLboolean  lp, tp;
	glGetBooleanv(GL_LIGHTING, &lp);
	glGetBooleanv(GL_TEXTURE_2D, &tp);

	/** 关闭纹理和光照 */
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	//就算在屏幕外 只要线穿过view 就会显示
	const float logicWH = _win32.getLogicWidth() / 2;
	const float logicHH = _win32.getLogicHeight() / 2;

	glPushAttrib(GL_CURRENT_BIT);   /**< 保存当前属性 */
	glPushMatrix();                 /**< 压入堆栈 */
	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);    /**< 设置颜色 */
		glBegin(GL_LINES);
			/** Y轴方向 */
			glVertex3f(0, logicHH, 0);
			glVertex3f(0, -logicHH, 0);

			/** X轴方向 */
			glVertex3f(logicWH, 0, 0);
			glVertex3f(-logicWH, 0, 0);
		glEnd();
	glPopMatrix();
	glPopAttrib();

	/** 恢复场景状态 */
	if (tp)
		glEnable(GL_TEXTURE_2D);
	if (lp)
		glEnable(GL_LIGHTING);
}


/** 绘制球体 */
void AppDelegate::drawSphere()
{
	/** 设置材质属性 */
	GLfloat mat_ambient[] = { 0.9f, 0.5f, 0.8f, 1.0f };
	GLfloat mat_diffuse[] = { 0.9f, 0.5f, 0.8f, 1.0f };
	GLfloat mat_shininess[] = { 100.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _texture.getID());

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glRotatef(_rot, 0.0f, 1.0f, 1.0f);

		GLUquadricObj * sphere = gluNewQuadric();
		gluQuadricOrientation(sphere, GLU_OUTSIDE);
		gluQuadricNormals(sphere, GLU_SMOOTH);
		gluQuadricTexture(sphere, GL_TRUE);  //必须 否则不显示纹理
		gluSphere(sphere, 2, 50, 50);
		gluDeleteQuadric(sphere);
	glPopMatrix();
}

void AppDelegate::show3DS(float x,float z,float scale)
{
	glPushMatrix();
	float y = _terrain.getAveHeight(x,z) ;  /**< 获得此处地形高度 */
	glTranslatef(x,y,z);
	glScalef(scale,scale,scale);
	_3dsLoader.draw();                            /**< 显示3DS模型 */
	glPopMatrix();

}


/** 播放MD2动画 */
void AppDelegate::animateMD2(float x,float z,float h,float scale)
{
	glPushAttrib(GL_CURRENT_BIT); /**< 保存现有颜色属实性 */
	float y = _terrain.getAveHeight(x,z) + h;
	glPushMatrix();
	glTranslatef(x,y,z);
	glScalef(scale,scale,scale);
	_md2.animateMD2Model();
	glPopMatrix();
    glPopAttrib();   /**< 恢复前一属性 */	
}

void AppDelegate::draw()
{
	//用户自定义的绘制过程
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/** 放置摄像机 */	
	_camera.setLook();

	/** 绘制天空 */
	_skyBox.render();
	
	/** 渲染地形 */
	_terrain.render();

	drawSphere();

	drawCoordinate();

	show3DS(260,583,20);
	animateMD2(300,600,18,0.50);
	
	/** 输出屏幕信息 */ 
	printText();
		
	glFlush();	
}


