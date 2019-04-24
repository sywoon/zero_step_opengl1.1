#ifndef	__GLFONT_H__
#define	__GLFONT_H__

#include "Base.h"

class GLFont															
{
public:
	GLFont();
	virtual ~GLFont();
    
	bool initFont(const char *fontName = "宋体",int Size = 30);  /**< 初始化字体 */
	void printText(const char *txtUtf8, float x, float y);    /**< 在位置(x,y)处绘制字符串string */
	void killGLFont();

protected:
	HFONT _hFont;     /**< 字体句柄 */
};

#endif	// __GLFONT_H__