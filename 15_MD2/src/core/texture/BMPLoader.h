#ifndef __CBMPLOADER_H__
#define __CBMPLOADER_H__

#include "Base.h"


#define BITMAP_ID 0x4D42	//位图文件的标志

class BMPLoader
{
public:
    BMPLoader();
    ~BMPLoader();

    const GLuint&  getID() const { return _ID; }
    bool           loadTexture(const char* fileName); //载入位图并创建纹理
    void           freeTexture();

	void		   setFilter(int min, int max);
	void		   setWrap(int s, int t);

    bool           readBitmap(const char *filename);

	int			   getImgWidth()	{ return _imgWidth; }
	int			   getImgHeight()	{ return _imgHeight; }
	unsigned char* getImgData()		{ return _imgData;  }

private:
    GLuint      _ID;           //生成纹理的ID号
    int         _imgWidth;
    int         _imgHeight;
    unsigned char *_imgData;   //图像数据
};

#endif //__CBMPLOADER_H__

