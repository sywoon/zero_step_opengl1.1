#include "Camera.h"
#include <math.h>
#include <windows.h>
#include <gl.h>
#include <glu.h>
#include <stdio.h>
#include "Mouse.h"
#include "Application.h"

Camera::Camera()
{
	/** 初始化向量值 */
	Vector3 zero = Vector3(0.0, 0.0, 0.0);		
	Vector3 view = Vector3(0.0, 1.0, 0.5);		
	Vector3 up   = Vector3(0.0, 0.0, 1.0);		
    
	/** 初始化摄像机 */
	_position	= zero;					
	_view		= view;				
	_upVector	= up;	
	_speed     = 0.1f;
}


Camera::~Camera()
{
}

void Camera::resetView()
{
	_view = _viewOrigin;
}

/** 设置摄像机的位置,朝向和向上向量 */
void Camera::setCamera( float positionX, float positionY, float positionZ,
				  		float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ)
{
	Vector3 position	= Vector3(positionX, positionY, positionZ);
	Vector3 view		= Vector3(viewX, viewY, viewZ);
	Vector3 upVector	= Vector3(upVectorX, upVectorY, upVectorZ);

	/** 设置摄像机 */
	_position = position;	
	_view     = view;			
	_viewOrigin = view;
	_upVector = upVector;	
}

/**  旋转摄像机方向  */
void Camera::rotateView(float angle, float x, float y, float z)
{
	Vector3 newView;

	/** 计算方向向量 */
	Vector3 view = _view - _position;		

	/** 计算 sin 和cos值 */
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	/** 计算旋转向量的x值 */
	newView.x  = (cosTheta + (1 - cosTheta) * x * x)		* view.x;
	newView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* view.y;
	newView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* view.z;

	/** 计算旋转向量的y值 */
	newView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* view.x;
	newView.y += (cosTheta + (1 - cosTheta) * y * y)		* view.y;
	newView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* view.z;

	/** 计算旋转向量的z值 */
	newView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* view.x;
	newView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* view.y;
	newView.z += (cosTheta + (1 - cosTheta) * z * z)		* view.z;

	/** 更新摄像机的方向 */
	_view = _position + newView;
}

/** 用鼠标旋转摄像机 */
void Camera::setViewByMouse()
{
	static POINT lstPos = {0, 0};
	static Vector3 lstView = _view;

	CWin32& win32 = CApplication::GetInstance()->GetWin32();
	CMouse& mouse = win32.GetMouse();
	if (!mouse.IsLBtnDown())
	{
		if (lstPos.x != 0 && lstPos.y != 0)
		{
			lstView = _view;
		}
		lstPos.x = lstPos.y = 0;
		return;
	}	

	POINT& curPos = mouse.GetPos();
	if (lstPos.x == 0)
	{
		lstPos = curPos;
		return;
	}

	if (lstPos.x == curPos.x && lstPos.y == curPos.y)
		return;

	// 得到鼠标移动方向
	float angleY = (float)(lstPos.x - curPos.x)/win32.GetWidth();	// 摄像机左右旋转角度	
	float angleX = (float)(lstPos.y - curPos.y)/win32.GetHeight() * 2.0;	

	//resetView();  // 这种方式会导致每次都充值到原点
	_view = lstView;

	// 摄像机左右旋转角度
	rotateView(angleY, 0, 1, 0);

	// 摄像机上下旋转角度	
	{
		// 通过叉积找到与旋转方向垂直的向量
		Vector3 vAxis = _view - _position;
		vAxis = vAxis.crossProduct(_upVector);
		vAxis = vAxis.normalize();

		///旋转
		rotateView(angleX, vAxis.x, vAxis.y, vAxis.z);
	}


	// POINT mousePos;									  /**< 保存当前鼠标位置 */
	// int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1; /**< 得到屏幕宽度的一半 */
	// int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1; /**< 得到屏幕高度的一半 */
	// float angleY = 0.0f;							  /**< 摄像机左右旋转角度 */
	// float angleZ = 0.0f;		                      /**< 摄像机上下旋转角度 */					
	// static float currentRotX = 0.0f;
	
	// /** 得到当前鼠标位置 */
	// GetCursorPos(&mousePos);						
	// ShowCursor(TRUE);
	
	// /** 如果鼠标没有移动,则不用更新 */
	// if( (mousePos.x == middleX) && (mousePos.y == middleY) )
	// 	return;

	// /** 设置鼠标位置在屏幕中心 */
	// SetCursorPos(middleX, middleY);	
	
	// /**< 得到鼠标移动方向 */
	// angleY = (float)( (middleX - mousePos.x) ) / 1000.0f;		
	// angleZ = (float)( (middleY - mousePos.y) ) / 1000.0f;		

 //    static float lastRotX = 0.0f;      /**< 用于保存旋转角度 */
 // 	lastRotX = currentRotX; 
	
	// /** 跟踪摄像机上下旋转角度 */
	// currentRotX += angleZ;
 
	// /** 如果上下旋转弧度大于1.0,我们截取到1.0并旋转 */
	// if(currentRotX > 1.0f)     
	// {
	// 	currentRotX = 1.0f;
		
	// 	/** 根据保存的角度旋转方向 */
	// 	if(lastRotX != 1.0f) 
	// 	{
	// 		/** 通过叉积找到与旋转方向垂直的向量 */
	// 		Vector3 vAxis = _view - _position;
	// 		vAxis = vAxis.crossProduct(_upVector);
	// 		vAxis = vAxis.normalize();
			
	// 		///旋转
	// 		rotateView( 1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
	// 	}
	// }
	// /** 如果旋转弧度小于-1.0,则也截取到-1.0并旋转 */
	// else if(currentRotX < -1.0f)
	// {
	// 	currentRotX = -1.0f;
				
	// 	if(lastRotX != -1.0f)
	// 	{
	// 		/** 通过叉积找到与旋转方向垂直的向量 */
	// 		Vector3 vAxis = _view - _position;
	// 		vAxis = vAxis.crossProduct(_upVector);
	// 		vAxis = vAxis.normalize();
			
	// 		///旋转
	// 		rotateView( -1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
	// 	}
	// }
	// /** 否则就旋转angleZ度 */
	// else 
	// {	
	// 	/** 找到与旋转方向垂直向量 */
	// 	Vector3 vAxis = _view - _position;
	// 	vAxis = vAxis.crossProduct(_upVector);
	// 	vAxis = vAxis.normalize();
	
	// 	///旋转
	// 	rotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
	// }

	// /** 总是左右旋转摄像机 */
	// rotateView(angleY, 0, 1, 0);
}


/** 左右移动摄像机 */
void Camera::yawCamera(float speed)
{
	Vector3 yaw;
	Vector3 cross = _view - _position;
	cross = cross.crossProduct(_upVector);

	///归一化向量
	yaw = cross.normalize();
 
	_position.x += yaw.x * speed;
	_position.z += yaw.z * speed;

	_view.x += yaw.x * speed;
	_view.z += yaw.z * speed;
}

/** 前后移动摄像机 */
void Camera::moveCamera(float speed)
{
	/** 计算方向向量 */
	Vector3 vector = _view - _position;
	vector = vector.normalize();         /**< 单位化 */
	
	/** 更新摄像机 */
	_position.x += vector.x * speed;    /**< 根据速度更新位置 */
	_position.z += vector.z * speed;	
	_position.y += vector.y * speed;
	
	_view.x += vector.x * speed;		 /**< 根据速度更新方向 */	
	_view.y += vector.y * speed;
	_view.z += vector.z * speed;
}

/** 设置视点 */
void Camera::setLook()
{
	/** 设置视口 */
	gluLookAt(_position.x, _position.y, _position.z,	
			  _view.x,	 _view.y,     _view.z,	
			  _upVector.x, _upVector.y, _upVector.z);
}

