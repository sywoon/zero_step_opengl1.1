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
}

bool AppDelegate::init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	resizeDraw(true);

	initGLExt();

	if(!_font.initFont())
	{
		Log("初始化字体失败!");
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

	_camera.setCamera( 500, 35, 400,  501, 35, 400,  0, 1, 0);

	return true;
}

void AppDelegate::unInit()
{
}

void AppDelegate::updateCamera()
{
	_camera.setViewByMouse();
	
	/** 键盘按键响应 */
	if(isPressed(VK_SHIFT))                        /**< 按下SHIFT键时加速 */
	{
		_camera.setSpeed(0.2f);
	}
	if(!isPressed(VK_SHIFT))
	{
		_camera.setSpeed(0.1f);
	}
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
    Vector3 vNewPos = vPos; 
	

	/** 设置摄像机高度为 地形高度 + 10 */
	vNewPos.y = (float)_terrain.getAveHeight(vPos.x,vPos.z ) + 10;

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

	/** 空格键切换绘制模式 */
	if(isPressed(VK_SPACE) && !_sp)
	{
		_sp = true;
		_renderMode = !_renderMode;
		if(_renderMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
			

	if(!isPressed(VK_SPACE))
		_sp = false;
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
	char string[128];                               /**< 用于保存输出字符串 */
	glPushAttrib(GL_CURRENT_BIT);                   /**< 保存现有颜色属性信息 */
	glPushMatrix(); 	
	glTranslatef(0.0f, 0.0f, -10.0f); 

		glColor3f(0.0f,1.0f,1.0f);                      /**< 设置文字颜色 */
		sprintf(string,"当前位置:X=%3.1f  Y=%3.1f Speed =%3.1f ",   
			_camera.getPosition().x,_camera.getPosition().z ,_camera.getSpeed()); /**< 字符串赋值 */
		_font.printText(string,-5.0f,3.5f);

		/** 输出帧速 */
	    caculateFrameRate();                               /**< 计算帧速 */
	    sprintf(string,"FPS:%d",(int)_fps);               /**< 字符串赋值 */
		_font.printText(string, -5.0f,3.0f);              /**< 输出字符串 */
	
	glPopMatrix();	
	glPopAttrib();
}

void AppDelegate::draw()
{
	//用户自定义的绘制过程
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();
	
	/** 放置摄像机 */	
	_camera.setLook();

	/** 绘制天空 */
	_skyBox.render();
	
	/** 渲染地形 */
	_terrain.render();
	
	/** 输出屏幕信息 */ 
	printText();
		
	glFlush();	
}


