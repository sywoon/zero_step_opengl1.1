#ifndef	__GLFONT_H__
#define	__GLFONT_H__


class GLFont															
{
public:
	GLFont();
	virtual ~GLFont();
    
	bool InitFont(int Size = 20, const char *fontName = "宋体");

	//align 0:左对齐  1:居中  2:右对齐
	void PrintText(const char *txtUtf8, float x, float y, int alignW=0, int alignH=0);
	void KillGLFont();

protected:
	HFONT _hFont;     /**< 字体句柄 */
};

#endif	// __GLFONT_H__