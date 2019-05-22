#include "SkyBox.h"
#include "Application.h"
#include "Log.h"

	
SkyBox::SkyBox()
	: _length(750.0f)
	, _width(550.0f)
	, _height(400.0f)
	, _yRot(0.01f)
{	
}

SkyBox::~SkyBox()
{
	for(int i=0; i<5; i++)
	{
		_texture[i].freeTexture();
	}
}


bool SkyBox::init()
{
	char filename[128]; 
	char *bmpName[] = {"back", "front", "top", "left", "right"};
	for(int i=0; i<5; i++)
	{
		sprintf(filename,"../res/%s", bmpName[i]);
		strcat(filename,".bmp");
		if(!_texture[i].loadTexture(filename))
		{
			Log("装载位图文件失败!");
			return false;
		}

		_texture[i].setFilter(GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);
		_texture[i].setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	}
	return true;
}


void SkyBox::render()
{
	// 获得场景中光照状态
	GLboolean bLight;
	glGetBooleanv(GL_LIGHTING, &bLight);

	glDisable(GL_LIGHTING);            // 关闭光照
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();

	// 天空盒跟随镜头移动  避免创建超大盒体问题
	Camera& camera = g_app.getCamera();
	Vector3 cameraPos = camera.getPosition();
	glTranslatef(cameraPos.x, cameraPos.y, cameraPos.z);

	//模拟云流动效果  实际是整个在旋转
	glRotatef(_yRot, 0.0f, 1.0f, 0.0f);
			
	// 绘制背面
	glBindTexture(GL_TEXTURE_2D, _texture[0].getID());
    glBegin(GL_QUADS);		
		// 指定纹理坐标和顶点坐标
		glTexCoord2f(1.0f, 0.0f); glVertex3f( _width, -_height, -_length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( _width,  _height, -_length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-_width,  _height, -_length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-_width, -_height, -_length);
	glEnd();


	// 绘制前面
	glBindTexture(GL_TEXTURE_2D, _texture[1].getID());
	glBegin(GL_QUADS);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-_width, -_height, _length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-_width,  _height, _length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( _width,  _height, _length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f( _width, -_height, _length);
	glEnd();


	// 绘制顶面
	glBindTexture(GL_TEXTURE_2D,  _texture[2].getID());
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 1.0f); glVertex3f( _width, _height, -_length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( _width, _height,  _length); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-_width, _height,  _length);
	    glTexCoord2f(1.0f, 1.0f); glVertex3f(-_width, _height, -_length);
	glEnd();


	// 绘制左面
	glBindTexture(GL_TEXTURE_2D, _texture[3].getID());
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 1.0f);  glVertex3f(-_width,  _height, -_length);		
		glTexCoord2f(0.0f, 1.0f);  glVertex3f(-_width,  _height,  _length); 
		glTexCoord2f(0.0f, 0.0f);  glVertex3f(-_width, -_height,  _length);
		glTexCoord2f(1.0f, 0.0f);  glVertex3f(-_width, -_height, -_length);	
	glEnd();

	// 绘制右面
	glBindTexture(GL_TEXTURE_2D, _texture[4].getID());
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(_width, -_height, -_length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(_width, -_height,  _length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(_width,  _height,  _length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(_width,  _height, -_length);
	glEnd();

    glPopMatrix();                 

	if(bLight)       // 恢复光照状态
	{
		glEnable(GL_LIGHTING);
	}

	glDisable(GL_TEXTURE_2D);

	_yRot += 0.01f;
	if(_yRot > 360.0f)
		_yRot = 0.0f;
}
