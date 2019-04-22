#ifndef	__GLFONT_H__
#define	__GLFONT_H__


class GLFont															
{
public:
	GLFont();
	virtual ~GLFont();
    
	bool InitFont(HDC hDC, const char *fontName,int Size );  /**< 初始化字体 */
	void PrintText(const char *text, float x, float y);    /**< 在位置(x,y)处绘制字符串string */
	void KillGLFont();

protected:
	HFONT _hFont;     /**< 字体句柄 */
};

#endif	// __GLFONT_H__