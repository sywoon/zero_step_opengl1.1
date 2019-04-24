#pragma once

#include "Base.h"
#include "Application.h"


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

private:
	friend class Application;
	AppDelegate();

private:
  	float      _fps; 
	bool       _renderMode;	 // 绘制模式 */
	bool       _sp;           // 空格键是否释放 */
};
