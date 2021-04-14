#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "Base.h"
#include "texture/BMPLoader.h"


// 地形类
class Terrain
{
public:
	Terrain();
	~Terrain();

	/** 初始化地形 */
	bool init();

	/** 渲染地形 */
	void render();

	/** 设置是否使用细节纹理标志 */ 
	void setDetail(bool bDetail) { _bDetail = bDetail;}
		
	/** 获得地面高度 */
	float getAveHeight(float x, float z);

	bool checkPos(float x, float z);
	

private:
	/** 设置地形的大小 */
	void setSize(unsigned  int width, unsigned  int cell); 

	/** 载入'.raw'高度图 */
	bool loadRawFile(const char* fileName);

	/** 装载纹理 */
	bool loadTexture();

	/** 获得点(x,y)的高度信息 */
	int getHeight(int x, int y);

	/** 设置纹理坐标 */
	void setTexCoord(float x, float z);

	/** 设置雾效 */
	void initFog();

                      
private:
	unsigned  int     _nWidth;          // 地形网格数
	unsigned  int     _nCellWidth;      // 每一格宽度
   	BYTE*             _pHeightMap;      // 存放高度信息
	BMPLoader         _texture[2];      // 地面纹理和细节纹理
	bool              _bDetail;         // 是否使用细节纹理标志
	int               _detailScale;     // 缩放比例
                                         	
};

#endif //__TERRAIN_H__

