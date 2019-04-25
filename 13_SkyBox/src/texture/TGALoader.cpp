#include "TGALoader.h"
#include <stdio.h>
#include "Log.h"


CTGALoader::CTGALoader()
{
    _ID = 0;
    _imgData = 0;
    _tgaType = 0;
    _imgWidth = 0;
    _imgHeight = 0;
}

CTGALoader::~CTGALoader()
{
    freeTexture();
}

/** 载入TGA文件 */
bool CTGALoader::loadTGA(const char* file)
{
    if(file == NULL || file[0] == '\0') 
       return false;
    
    freeTexture();
    
    FILE *pfile;
    unsigned char tempColor;              /**< 用于交换颜色分量 */
    unsigned char bitCount;               /**< 每象素的bit位数 */
    int colorMode;                        /**< 颜色模式 */
    long tgaSize;                         /**< TGA文件大小 */
    unsigned char unCompressHeader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0}; /**< 未压缩TGA文件头 */
    unsigned char tgaHeader[12];          /**< 文件头 */
    unsigned char header[6];              /**< 文件头前6个字节 */

   
   
    pfile = fopen(file, "rb");
    if(!pfile) 
      return false;
    
    /** 读取文件头前12个字节 */
    fread(tgaHeader, 1, sizeof(tgaHeader), pfile);
    
    /** 比较文件是否为未压缩文件 */
    if(memcmp(unCompressHeader, tgaHeader, sizeof(unCompressHeader)) != 0)
    {
        MessageBox(NULL, "文件类型错误,只支持非压缩类型!","错误", MB_OK);
        fclose(pfile);
        return false;
    }

    /** 读取6个字节 */
    fread(header, 1, sizeof(header), pfile);
    
    /** 计算图像的宽度和高度 */
    _imgWidth = header[1] * 256 + header[0];
    _imgHeight = header[3] * 256 + header[2];
    
    /** 获取每象素的bit位数 */
    bitCount = header[4];
    
    /**　计算颜色模式和图像大小 */
    colorMode = bitCount / 8;
    tgaSize = _imgWidth * _imgHeight * colorMode;

    /** 分配内存 */
    _imgData = new unsigned char[sizeof(unsigned char) * tgaSize];
    
    /** 读取数据 */
    fread(_imgData, sizeof(unsigned char), tgaSize, pfile);
    
    /** 将BGA格式转化为RGA格式 */
    for(long index = 0; index < tgaSize; index += colorMode)
    {
        tempColor = _imgData[index];
        _imgData[index] = _imgData[index + 2];
        _imgData[index + 2] = tempColor;
    }
    
    /** 关闭文件 */
    fclose(pfile);
    
    /** 设置图象类型 */
    if(colorMode == 3) 
        _tgaType = GL_RGB;
    else 
        _tgaType = GL_RGBA;
    
    return true;
}

bool CTGALoader::loadTexture(const char* fileName)
{
    if(!loadTGA(fileName))
    {
        Log("载入TGA文件失败!");
      	return false;
    }

	  // 生成纹理对象名称
    glGenTextures(1, &_ID);
   
    // 绑定纹理对象
    glBindTexture(GL_TEXTURE_2D, _ID);

	//纹理映射方式
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
    // 控制滤波
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
   
    // 创建纹理
   	gluBuild2DMipmaps(GL_TEXTURE_2D, _tgaType, _imgWidth,
	                  _imgHeight, _tgaType, GL_UNSIGNED_BYTE,
	                  _imgData);
   return true;
}

void CTGALoader::freeTexture()
{
	SAVE_DELETE_ARRAY(_imgData);

    if (_ID > 0)
    {
        glDeleteTextures(1, &_ID);
		_ID = 0;
    }
}

void CTGALoader::setFilter(int min, int max)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max);
}

void CTGALoader::setWrap(int s, int t)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
}


