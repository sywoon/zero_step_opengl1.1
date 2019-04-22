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
	bool    isExtensionSupported(const char *string);  /**< 检查是否支持扩展 */
	bool    initMultiTexture();                        /**< 初始化 */
	bool    loadTexture();                             /**< 装载纹理 */

private:
	friend class CApplication;
	AppDelegate();

private:
	CBMPLoader   _texture[4];            /**< 载入4幅纹理 */
	bool         _multitexturing;          /**< 标志是否使用多重纹理 */
	bool         _sp;                      /**< 判断空格键是否释放 */
};
