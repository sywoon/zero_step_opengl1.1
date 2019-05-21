#pragma once

#include "Core.h"
#include "Application.h"
#include "texture/BMPLoader.h"
#include "3DSLoader.h"


class AppDelegate : Application
{
public:
	virtual bool init();
	virtual void unInit();
	virtual void update(DWORD milliseconds);
	virtual void draw();

	void    updateCamera();                 /**< 更新摄像机 */
	void    caculateFrameRate();            /**< 计算帧速 */
	void    printText();                    /**< 输出文字信息 */
	void    show3DS(float x,float z, float scale);    
	

private:
	void    drawSphere();
	void    drawCoordinate();

private:
	friend class Application;
	AppDelegate();


private:
	BMPLoader  _texture;
	C3DSLoader _3dsLoader;  
  	float      _fps; 
	bool       _renderMode;	 // 绘制模式 */
	bool       _sp;           // 空格键是否释放 */
	float      _rot;
};
