#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Base.h"

class Camera
{
public:
	Camera();
	~Camera();
	
	/** 获得摄像机状态方法 */
	Vector3 getPosition()   { return _position; }
	Vector3 getView()	    { return _view; }
	Vector3 getUpVector()   { return _upVector; }
	float   getSpeed()      { return _speed; }

	void setSpeed(float speed);
     
	/** 设置摄像机的位置, 观察点和向上向量 */
	void setCamera(float positionX, float positionY, float positionZ,
			 	   float viewX,     float viewY,     float viewZ,
				   float upVectorX, float upVectorY, float upVectorZ);
	void setCamera(Vector3& pos, Vector3& view, Vector3& up);

	/** 旋转摄像机方向 */
	void rotateView(float angle, float X, float Y, float Z);

	/** 根据鼠标设置摄像机观察方向 */
	void setViewByMouse(); 
	
    /** 左右摄像机移动 */
	void yawCamera(float speed);

	/** 前后移动摄像机 */
	void moveCamera(float speed);
	
	/** 放置摄像机 */
	void setLook();

private:
	void resetView();

private:
	Vector3        _position;      // 位置
	Vector3        _view;          // 朝向
	Vector3        _viewOrigin;    // 初始朝向
	Vector3        _upVector;      // 向上向量
	float          _speed;         // 速度
};

#endif //__CAMERA_H__