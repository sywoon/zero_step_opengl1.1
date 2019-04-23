#ifndef __CBMPLOADER_H__
#define __CBMPLOADER_H__

#include <windows.h>
#include <gl.h>
#include <glu.h>


#define BITMAP_ID 0x4D42	//位图文件的标志

class CBMPLoader
{
public:
   CBMPLoader();
   ~CBMPLoader();

   const GLuint& getID() const { return _ID; }
   bool LoadTexture(const char* fileName);       //载入位图并创建纹理
   void FreeImage();                      //释放图像数据

private:
   bool ReadBitmap(const char *filename); //装载一个位图文件

private:
   GLuint _ID;                 //生成纹理的ID号
   int _imgWidth;
   int _imgHeight;
   unsigned char *_imgData;   //图像数据
};

#endif //__CBMPLOADER_H__

