#ifndef __TGALOADER_H__
#define __TGALOADER_H__

#include <windows.h>
#include <gl.h>
#include <glu.h>



class CTGALoader
{
public:
    CTGALoader();
    ~CTGALoader();

    const GLuint& getID() const { return _ID; }
    bool LoadTexture(const char* fileName);  //载入TGA文件为纹理
    void FreeImage();

private:
    bool LoadTGA(const char *file);

private:
    GLuint _ID;                        //生成纹理的ID号
    int _imgWidth;
    int _imgHeight;
    unsigned char *_imgData;
    unsigned int _tgaType;                      //图象类型GL_RGB 或GL_RGBA
};


#endif