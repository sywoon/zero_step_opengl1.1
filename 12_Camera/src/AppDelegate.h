#pragma once

#include "Application.h"
#include "BMPLoader.h"
#include "GLFont.h"
#include "Camera.h"
#include <gl.h>
#include <glu.h>

class AppDelegate : CApplication
{
public:
	virtual bool Init();
	virtual void UnInit();
	virtual void Update(DWORD milliseconds);
	virtual void Draw();

protected:
	bool    LoadTexture();                  /**< 载入纹理 */
    void    UpdateCamera();                 /**< 更新摄像机 */
	void    CaculateFrameRate();            /**< 计算帧速 */
	void    PrintText();                    /**< 输出文字信息 */
	
	void    SetLight();                     /**< 设置光源 */
	void    DrawGrid();                     /**< 绘制网格地面 */
	void    DrawSphere();                   /**< 绘制球体 */
	void    DrawBox();                      /**< 绘制木箱 */

private:
	friend class CApplication;
	AppDelegate();

private:
	CBMPLoader _texture;
	CBMPLoader _texture2;
	GLFont     _font;                        /**< 字体类 */
	Camera     _camera;                      /**< 摄像机类 */     
  	float      _fps;                         /**< 帧速 */
  	float      _rot;
};
