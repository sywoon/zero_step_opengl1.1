#ifndef	__GLFONT_H__
#define	__GLFONT_H__

#include "Base.h"

class GLFont															
{
public:
	GLFont();
	virtual ~GLFont();
    
	bool initFont(const char *fontName = "宋体",int Size = 30);
	void printText(const char *txtUtf8, float x, float y);
	void killGLFont();

protected:
	HFONT _hFont; 
};

#endif	// __GLFONT_H__