#pragma once

#include "Application.h"
#include "BMPLoader.h"
#include "TGALoader.h" 


class AppDelegate : CApplication
{
public:
	virtual bool Init();
	virtual void UnInit();
	virtual void Update(DWORD milliseconds);
	virtual void Draw();

private:
	bool    LoadTexture();                  /**< 载入纹理 */
	void    SetLight();                     /**< 设置光源 */
	void    DrawSphere();                   /**< 绘制球体 */
	void    DrawBox();                      /**< 绘制木箱 */

private:
	friend class CApplication;
	AppDelegate();

private:
	CBMPLoader _texture1;                     /**< 位图载入类 */
	CTGALoader _texture2;                     /**< TGA文件载入类 */
	float  _rot;					             /**< 用于旋转物体 */	
};
