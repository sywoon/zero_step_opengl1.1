#include "AppDelegate.h"

#include <stdio.h>
#include <math.h>
#include <gl.h>
#include <glu.h>
#include <glaux.h>
#include <glext.h>



/** 多重纹理函数 */
PFNGLMULTITEXCOORD2FARBPROC     glMultiTexCoord2fARB = NULL;
PFNGLACTIVETEXTUREARBPROC       glActiveTextureARB = NULL;



static CApplication* s_pInstance = NULL;

CApplication* CApplication::Create()
{
	if (s_pInstance)
		return s_pInstance;

	AppDelegate* app = new AppDelegate();
	s_pInstance = app;
	return reinterpret_cast<CApplication*>(app);
}

CApplication* CApplication::GetInstance()
{
	return s_pInstance;
}

void CApplication::Destroy()
{
	if (s_pInstance)
	{
		free(s_pInstance);
		s_pInstance = NULL;
	}
}



AppDelegate::AppDelegate()
	: CApplication()
{
	_multitexturing = true;
	_sp = false;
}

bool AppDelegate::Init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	ResizeDraw(true);

	if(!initMultiTexture())
	{
		Log("不支持多重纹理!");
		return false;
	}

	/** 载入纹理 */
	if(!loadTexture())
	{
		Log("载入纹理错误!");
		return false;
	}

	return true;
}

void AppDelegate::UnInit()
{
	for(int i=0; i<4; i++)
	{
		_texture[i].FreeImage();
		glDeleteTextures(1, &_texture[i].getID());
	}
}


void AppDelegate::Update(DWORD milliseconds)
{
	/** 当按下空格键时,开启或关闭多重纹理 */
	if(IsPressed(VK_SPACE))
	{
		if (_sp)
		{
			_sp = false;
		} else {
			_sp = true;
			_multitexturing = !_multitexturing;
		}
	}
}


/** 绘制函数 */
void AppDelegate::Draw()											
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();
	
	glTranslatef(0.0f,0.0f,-10.0f);

	/** 激活纹理0,并绑定纹理 */
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D); 
	glBindTexture(GL_TEXTURE_2D, _texture[0].getID());

	/** 激活纹理1,并绑定纹理 */
	glActiveTextureARB(GL_TEXTURE1_ARB);

	/** 如果多重纹理启用,则启用该纹理 */
	if (_multitexturing) 
		glEnable(GL_TEXTURE_2D); 
	else 
		glDisable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, _texture[1].getID());

	/** 绘制一个四方形墙面 */
	glPushMatrix();
		glTranslatef(-2.5, 0, 0);
		glScalef(2.0f,2.0f,2.0f);
		glBegin(GL_QUADS);

			/** 左上点 */
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
			glVertex3f(-1, 1, 0);

			/** 左下点 */
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
			glVertex3f(-1, -1, 0);

			/** 右下点 */
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
			glVertex3f(1, -1, 0);

			/** 右上点 */
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
			glVertex3f(1, 1, 0);

		glEnd();    /**< 绘制结束 */											
	glPopMatrix();

	
	/** 激活纹理0,并绑定纹理 */
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _texture[2].getID());

	/** 激活纹理1,并绑定纹理 */
	glActiveTextureARB(GL_TEXTURE1_ARB);
	
	/** 如果多重纹理启用,则启用该纹理 */
	if (_multitexturing) 
		glEnable(GL_TEXTURE_2D); 
	else 
		glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _texture[3].getID());

	static float wrap = 0;      /**< 用于雾的流动 */     

	glTranslatef(2.5, 0, 0);
	glScalef(2.0f,2.0f,2.0f);
	glBegin(GL_QUADS);

		/** 左上点 */
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f - wrap, 1.0f);
		glVertex3f(-1, 1, 0);

		/** 左下点 */
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f - wrap, 0.0f);
		glVertex3f(-1, -1, 0);

		/** 右下点 */
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f - wrap, 0.0f);
		glVertex3f(1, -1, 0);

		/** 右上点 */
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f - wrap, 1.0f);
		glVertex3f(1, 1, 0);
	glEnd();											

	wrap += 0.001f;                   /**< 递增 */

			
	glFlush();						    
}


/** 检查是否支持扩展 */
bool AppDelegate::isExtensionSupported(const char* string)
{
	char *extension;			/**< 指向扩展字符串的指针 */
	char *end;				    /**< 最后一个字符指针 */
	int idx;

	extension = (char*)glGetString(GL_EXTENSIONS);
	if (extension == NULL)
		return false;	
	
	/** 得到最后一个字符 */
	end = extension + strlen(extension);	

	/** 循环查找字符串string */
	while (extension < end)
	{
		/** 找到空格前的一个字符串 */
		idx = strcspn(extension, " ");
		
		/** 检查是否找到字符串string */
		if ( (strlen(string) == idx) && (strncmp(string, extension, idx) == 0))
		{
			return true;
		}
		
		/** 当前串不是string,继续下一个字符串 */
		extension += (idx + 1);
	}
	/** 没有找到,则返回false */
	return false;   
}

/** 初始化 */
bool AppDelegate::initMultiTexture()
{
	/** 检查是否支持扩展 */
	if (isExtensionSupported("GL_ARB_multitexture"))
	{
				
		/** 获得函数指针地址 */
		glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
		glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) wglGetProcAddress("glMultiTexCoord2fARB");
		
		return true;
	}
	else 
		return false;
}

/** 载入纹理数据  */
bool AppDelegate::loadTexture()
{
	const char* fileName[4] ={ "res/wall.bmp", 
						"res/lightmap.bmp", 
						"res/bitmap.bmp", 
						"res/fog.bmp"};
		
	/// 载入四幅位图 
	for(int i=0; i<4; i++)
	{
		if(!_texture[i].LoadTexture(fileName[i]))
		{
			Log("载入位图文件失败!");
			return false;
		}		 
		
	}
	return true;
}