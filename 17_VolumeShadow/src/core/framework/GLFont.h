#ifndef	__GLFONT_H__
#define	__GLFONT_H__

#include "Base.h"

class GLFont															
{
public:
	GLFont();
	virtual ~GLFont();
    
	bool initFont(int Size = 20, const char *fontName = "宋体");

	//align 0:左对齐  1:居中  2:右对齐
	void printText(const char *txtUtf8, float x, float y, int alignW=0, int alignH=0);
	void killGLFont();

protected:
	HFONT _hFont; 
};

#endif	// __GLFONT_H__