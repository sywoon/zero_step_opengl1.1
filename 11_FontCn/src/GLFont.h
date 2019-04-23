#ifndef	__GLFONT_H__
#define	__GLFONT_H__


class GLFont															
{
public:
	GLFont();
	virtual ~GLFont();
    
	bool InitFont(const char *fontName = "宋体",int Size = 60);  /**< 初始化字体 */
	void PrintText(const char *txtUtf8, float x, float y);    /**< 在位置(x,y)处绘制字符串string */
	void KillGLFont();

protected:
	HFONT _hFont;     /**< 字体句柄 */
};

#endif	// __GLFONT_H__