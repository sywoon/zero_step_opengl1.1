#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "Base.h"
#include "texture/BMPLoader.h"

#define GL_CLAMP_TO_EDGE	0x812F


class SkyBox
{
public:
	SkyBox();
	~SkyBox();

	bool init();
	void render();


private:
	BMPLoader   _texture[5];     // 天空盒纹理  
	float       _length;         // 长度
	float       _width;          // 宽度
	float       _height;         // 高度
	float       _yRot;           // 绕Y轴旋转
		
};


#endif ///__SKYBOX_H__