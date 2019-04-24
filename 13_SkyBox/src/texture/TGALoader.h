#ifndef __TGALOADER_H__
#define __TGALOADER_H__

#include "Base.h"



class CTGALoader
{
public:
    CTGALoader();
    ~CTGALoader();

    const GLuint&  getID() const { return _ID; }
    bool           loadTexture(const char* fileName); //载入位图并创建纹理
    void           freeTexture();


private:
    bool           loadTGA(const char *file);

private:
    GLuint      _ID;             //生成纹理的ID号
    int         _imgWidth;
    int         _imgHeight;
    unsigned char *_imgData;
    unsigned int _tgaType;       //图象类型GL_RGB 或GL_RGBA
};


#endif