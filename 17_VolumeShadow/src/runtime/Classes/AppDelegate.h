#pragma once

#include "Core.h"
#include "Application.h"
#include <vector>
#include "texture/TGALoader.h"


using std::vector;



/** 墙结构体 */
struct Wall
{
	vector<Vector3> vVerts;	/**< 顶点数组 */
	Vector3 vNormal;		/**< 法向量 */
	Vector3 vColor;			/**< 颜色 */
};

/** 墙四面体的信息数组 */
vector<Wall> walls;


class AppDelegate : Application
{
public:
	virtual bool init();
	virtual void unInit();
	virtual void update(DWORD milliseconds);
	virtual void draw();

	void    caculateFrameRate();               /**< 计算帧速 */
	void    printText();                       /**< 输出文字信息 */

    /** 绘制场景中所有物体 */
	void    render();    

	/** 绘制阴影体 */
	void    drawSilhouette(Vector4 lightPos, float Extend);

private:


private:
	friend class Application;
	AppDelegate();


private:
	GLFont         _font;                        /**< 字体类 */
  	float          _fps;                         /**< 帧速 */
	CTGALoader   _ground;                        /**< 地面纹理 */
};
