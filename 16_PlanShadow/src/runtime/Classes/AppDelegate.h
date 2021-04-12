#pragma once

#include "Core.h"
#include "Application.h"
#include <vector>


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
    void    renderWall(const Wall& wall);      /**< 绘制墙四面体 */
	void    renderObjects();                   /**< 绘制物体 */
	void    renderLight();                     /**< 绘制光源 */
	
	/** 创建投影矩阵 */
	void    createShadowMatrix(float m[16], Vector3 point, 
		                       Vector3 normal, float lp[4]);
	/** 绘制物体和阴影 */
	void    renderShadow();  

private:


private:
	friend class Application;
	AppDelegate();


private:
	GLFont         _font;                        /**< 字体类 */
  	float          _fps;                         /**< 帧速 */
};
