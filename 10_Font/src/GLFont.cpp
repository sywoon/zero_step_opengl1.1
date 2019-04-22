#include <windows.h>   
#include <gl.h>    
#include "GLFont.h"


GLFont::GLFont()
{
}

GLFont::~GLFont()
{
	KillGLFont(); 
}

bool GLFont::InitFont(HDC hDC, const char *fontName,int size)
{
   HFONT oldFont;              /**< 保存旧的字体句柄 */
   HFONT hFont;                /**< 字体句柄 */
   
   /** 创建96个显示列表 */
   _listBase = glGenLists(96);
   if(_listBase == 0)          /**< 分配失败,则返回 */
	   return false;
   
    /** 创建字体 */
    hFont = CreateFont(size,					/**< 字体高度 */
						0,						/**< 字体宽度 */
						0,						/**< 字体的旋转角度 Angle Of Escapement */
						0,						/**< 字体底线的旋转角度Orientation Angle */
						FW_BOLD,				/**< 字体的重量 */
						FALSE,					/**< 是否使用斜体 */
						FALSE,					/**< 是否使用下划线 */
						FALSE,					/**< 是否使用删除线 */
						ANSI_CHARSET,			/**< 设置字符集 */
						OUT_TT_PRECIS,			/**< 输出精度 */
						CLIP_DEFAULT_PRECIS,	/**< 裁剪精度 */
						ANTIALIASED_QUALITY,	/**< 输出质量 */
				FF_DONTCARE|DEFAULT_PITCH,		/**< Family And Pitch */
						fontName);				/**< 字体名称 */
	 if(!hFont)
		return false;                           /**< 创建字体失败则返回 */
 
	oldFont = (HFONT)SelectObject(hDC, hFont);  /**< 选择我们需要的字体 */
	wglUseFontBitmaps(hDC, 32, 96, _listBase);   /**< 创建96个显示列表，绘制从ASCII码为32-128的字符 */
	 
    return true;
}



/** 在指定位置输出字符串 */
void GLFont::PrintText(const char *text, float x, float y)
{
   /** 错误检查.如果listBase为0或string为NULL则不用绘制直接返回 */
    if((_listBase == 0 || text == NULL))
       return ;

    glPushMatrix();
	glDisable(GL_DEPTH_TEST); /**< 关闭深度测试 */
	glDisable(GL_LIGHTING);   /**< 关闭光照 */

	glRasterPos2f(x, y);      /**< 将光栅位置移动到指定点 */
	
	/** 将字符串输出到屏幕上 */
	glPushAttrib(GL_LIST_BIT);                       /**< 把显示列表属性压入属性堆栈 */
		glListBase(_listBase - 32);                   /**< 设置显示列表的基值 */
		
		/** 执行多个显示列表输出字符串 */
		glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text); 
	glPopAttrib();                                   /**< 把显示列表属性弹出属性堆栈 */
    
	/** 恢复原来状态 */
	glEnable(GL_LIGHTING);    /**< 启用光照 */
	glEnable(GL_DEPTH_TEST);  /**< 启用深度测试 */
    glPopMatrix();
}

/**< 删除字体 */
void GLFont::KillGLFont()
{
   /** 删除字体显示列表 */
   if(_listBase != 0)
      glDeleteLists(_listBase, 96);
}

