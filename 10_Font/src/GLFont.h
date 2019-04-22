#ifndef	__GLFONT_H__
#define	__GLFONT_H__


class GLFont															
{
public:
	GLFont();
	virtual ~GLFont();
    
	bool InitFont(HDC hDC, const char *fontName,int Size );  /**< 初始化字体 */
	void PrintText(const char *text, float x, float y);    /**< 在位置(x,y)处绘制字符串string */
	void KillGLFont();                                 /**< 删除字体 */

protected:
	unsigned int _listBase;                             /**< 绘制字体的显示列表的开始位置 */
};

#endif	// __GLFONT_H__