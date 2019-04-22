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

bool GLFont::InitFont(HDC hDC, const char *fontName, int size)
{
   _hFont = CreateFont(size,					/**< 字体高度 */
						0,						/**< 字体宽度 */
						0,						/**< 字体的旋转角度  */
						0,						/**< 字体底线的旋转角度 */
						FW_BOLD,				/**< 字体的重量 */
						FALSE,					/**< 是否使用斜体 */
						FALSE,					/**< 是否使用下划线 */
						FALSE,					/**< 是否使用删除线 */
						GB2312_CHARSET,			/**< 设置字符集 */
						OUT_TT_PRECIS,			/**< 输出精度 */
						CLIP_DEFAULT_PRECIS,	/**< 裁剪精度 */
						ANTIALIASED_QUALITY,	/**< 输出质量 */
				FF_DONTCARE|DEFAULT_PITCH,		/**< Family And Pitch */
						fontName);				/**< 字体名称 */
	if(!_hFont)
		return false;
  
   return true;
}



/** 在指定位置输出字符串 */
void GLFont::PrintText(const char *text, float x, float y)
{
    HBITMAP hBitmap, hOldBmp; /**< 定义两个位图句柄 */
	BITMAP bm;               /**< 位图结构变量 */
	SIZE size;               /**< 位图尺寸 */
	
	/** 获得场景中的状态 */
	GLboolean lp, tp;
	glGetBooleanv(GL_LIGHTING, &lp);
	glGetBooleanv(GL_TEXTURE_2D, &tp);
	HDC hDC = ::CreateCompatibleDC(0); /**< 暂存设备场景 */
	
	/** 保存和设置一些属性 */
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0,0, -10.0f);
	glDisable(GL_LIGHTING);     /**< 关闭光照 */
	glDisable(GL_TEXTURE_2D);   /**< 关闭纹理 */
	glDisable(GL_DEPTH_TEST);   /**< 关闭深度测试 */ 
	
	SelectObject(hDC, _hFont); /**< 选择字体 */
	
	/** 获取字符位图大小 */
	::GetTextExtentPoint32(hDC, text, strlen(text), &size);
	
	/** 创建与hDC相关单色位图 */
	hBitmap = CreateBitmap(size.cx, size.cy,1, 1, NULL); 
	
	/** 选择位图 */      
	hOldBmp = (HBITMAP)SelectObject(hDC, hBitmap);              
  
	SetBkColor(hDC, RGB(0, 0, 0));              /**< 背景色为黑色 */
	SetTextColor(hDC, RGB(255, 255, 255));      /**< 字体颜色白色 */
	SetBkMode(hDC, OPAQUE);                     /**< 用当前的背景颜色填充背景 */
	TextOut(hDC, 0, 0, text, strlen(text)); 	/**< 输出文字到暂存hDC */
	
	/** 获得相关位图数据结构 */
	GetObject(hBitmap, sizeof(bm), &bm);
	size.cx = (bm.bmWidth + 31) & (~31); /**< 边缘对齐 */
	size.cy = bm.bmHeight;
	int bufsize = size.cx * size.cy/8;  /**< 图形数据长度 */
    
	/**　定义单色位图结构 */  
	struct 
	{
		BITMAPINFOHEADER bih;
		RGBQUAD col[2];
	} bic;
    
	/** 获取单色位图结构信息 */
	BITMAPINFO *binf = (BITMAPINFO *)&bic;
	binf->bmiHeader.biSize = sizeof(binf->bmiHeader); /**< 修改结构信息 */
	binf->bmiHeader.biWidth = bm.bmWidth;
	binf->bmiHeader.biHeight = bm.bmHeight;
	binf->bmiHeader.biPlanes = 1;
	binf->bmiHeader.biBitCount = 1;          /**< 单色 */
	binf->bmiHeader.biCompression = BI_RGB;  /**< 颜色方式 */
	binf->bmiHeader.biSizeImage = bufsize;
	binf->bmiHeader.biXPelsPerMeter = 1;
	binf->bmiHeader.biYPelsPerMeter = 1;
	binf->bmiHeader.biClrUsed = 0;
	binf->bmiHeader.biClrImportant = 0;
	
	/** 定义图形数据块 */ 
	UCHAR* pBmpBits = new UCHAR[bufsize];	
	memset(pBmpBits, 0, sizeof(UCHAR)*bufsize);
	
	/** 将设备无关数据保存在pBmpBits指向的数据块中 */
	::GetDIBits(hDC, hBitmap, 0, bm.bmHeight, pBmpBits, binf, DIB_RGB_COLORS);
	 
	/** 显示字符串 */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); /**< 指定像素存储模式 */
	glRasterPos2f(x, y);                    /**< 定位 */
	
	/** 位图显示 */
	glBitmap(size.cx, size.cy, 0.0, 0.0, 0.0, 0.0, pBmpBits); 
	delete[] pBmpBits;                       /**< 删除指针 */
	SelectObject(hDC, hOldBmp);            /**< 恢复原来位图信息 */
	DeleteObject(hBitmap);
	::DeleteDC(hDC);
	
	/** 恢复一些状态 */
	if(lp)
		glEnable(GL_LIGHTING);     /**< 开启光照 */     
	if(tp)
		glEnable(GL_TEXTURE_2D);   /**< 开启纹理 */
	glEnable(GL_DEPTH_TEST);       /**< 开启深度测试 */
	glPopMatrix();
}

/**< 删除字体 */
void GLFont::KillGLFont()
{
   if(_hFont)
		DeleteObject(_hFont); /**< 删除字体句柄 */
}

