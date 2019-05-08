#include "BMPLoader.h"
#include "Log.h"


BMPLoader::BMPLoader()
{
	_ID = 0;
	_imgData = 0;
	_imgWidth = 0;
	_imgHeight = 0;
}

BMPLoader::~BMPLoader()
{
   freeTexture(); 
}

bool BMPLoader::readBitmap(const char *file)
{
	if(file == NULL || file[0] == '\0') 
     return false;

	freeTexture();

	FILE *pFile = 0;
	
	/** 创建位图文件信息和位图文件头结构 */
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER bitmapInfoHeader;
  
	unsigned char textureColors = 0;/**< 用于将图像颜色从BGR变换到RGB */

	pFile = fopen(file, "rb");
	if (pFile == 0) 
		return false;

	/** 读入位图文件头信息 */ 
	fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile);
	
	/** 检查该文件是否为位图文件 */
	if(header.bfType != BITMAP_ID)
	{
	   fclose(pFile);
	   return false;
	}

	/** 读入位图文件信息 */
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

	/** 保存图像的宽度和高度 */
	_imgWidth = bitmapInfoHeader.biWidth;
    _imgHeight = bitmapInfoHeader.biHeight;

    /** 确保读取数据的大小 */
    //为什么是乘3?  A:测试图片用的24位深度
    //怎么做到通用，兼容32位的图片？  使用biBitCount(位数/像素 1 2 4 24)
    // bitmapInfoHeader.biBitCount / 8 若位数小于24 怎么办？
    // Log("w:%d h:%d bit:%d", bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight, 
    // 		bitmapInfoHeader.biBitCount);
   if(bitmapInfoHeader.biSizeImage == 0)
      bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth *
      				bitmapInfoHeader.biHeight * bitmapInfoHeader.biBitCount / 8;  

	/** 将指针移到数据开始位置 */
	fseek(pFile, header.bfOffBits, SEEK_SET);

	_imgData = new unsigned char[bitmapInfoHeader.biSizeImage];
	if(!_imgData)
	{
	   delete[] _imgData;
	   fclose(pFile);
	   return false;
	}

	/** 读取图像数据 */
	//fread(void* ptr, size_t size, size_t count, FILE* file)
	fread(_imgData, 1, bitmapInfoHeader.biSizeImage, pFile);

	/** 将图像颜色数据格式进行交换,由BGR转换为RGB */
	for(int index = 0; index < (int)bitmapInfoHeader.biSizeImage; index+=3)
	{
	   textureColors = _imgData[index];
	   _imgData[index] = _imgData[index + 2];
	   _imgData[index + 2] = textureColors;
	}
  
	fclose(pFile);
	return true;
}

/** 载入位图文件，并创建纹理 */
bool BMPLoader::loadTexture(const char* fileName)
{
	if(!readBitmap(fileName))
	{
		Log("载入位图文件失败!");
		return false;
	}

	// 生成纹理对象名称
	glGenTextures(1, &_ID);
   
    // 创建纹理对象
    glBindTexture(GL_TEXTURE_2D, _ID);

    //纹理映射方式
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	// 控制滤波
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
   
	// 创建纹理
   	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, _imgWidth,
	                  _imgHeight, GL_RGB, GL_UNSIGNED_BYTE,
	                  _imgData);
   return true;
}

void BMPLoader::freeTexture()
{
	SAVE_DELETE_ARRAY(_imgData);

	if (_ID > 0)
	{
		glDeleteTextures(1, &_ID);
		_ID = 0;
	}
}

void BMPLoader::setFilter(int min, int max)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max);
}

void BMPLoader::setWrap(int s, int t)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
}