#include "Terrain.h"
#include "Log.h"


const unsigned int MAP_WIDTH = 1024;
const unsigned int CELL_WIDTH = 16;



Terrain::Terrain()
	: _bDetail(true)
	, _detailScale(128)
{
	/** 设置地形大小 */
	setSize(MAP_WIDTH, CELL_WIDTH);
	
	/** 为地形高分配内存,并初始化 */
	_pHeightMap = new BYTE[_nWidth * _nWidth];
	memset(_pHeightMap, 0, sizeof(BYTE) * _nWidth * _nWidth);
}

/** 析构函数 */
Terrain::~Terrain()
{
	SAVE_DELETE_ARRAY(_pHeightMap);
	
	/** 删除纹理对象及其占用内存 */	
	for(int i=0; i<2; i++)
	{
		_texture[i].freeTexture();
	}
}

/** 初始化雾效 */
void Terrain::initFog()
{
	float fogColor[4] = {0.8f, 0.8f, 0.8f, 1.0f};
	
	glEnable(GL_FOG);

	glFogi(GL_FOG_MODE, GL_EXP);             /** 设置雾效的模式 */
	glFogfv(GL_FOG_COLOR, fogColor);         /** 指定雾的颜色 */ 
    glFogf(GL_FOG_DENSITY, 0.001f);          /** 设置雾的浓度 */
	glFogf(GL_FOG_START, 1.0);               /** 设置线性雾的开始位置 */
	glFogf(GL_FOG_END, 1000.0);              /** 设置线性雾的结束位置 */
	glHint(GL_FOG_HINT, GL_DONT_CARE);       /** 规定雾化效果的质量 */ 
}


/** 初始化地形 */
bool Terrain::init()
{
	if(!glActiveTextureARB || !glMultiTexCoord2fARB)
	{
		//输出错误信息
		Log("不支持多重纹理！");
		return false;
		//setDetail(false);
	}

	/** 载入高度文件 */
	loadRawFile("res/terrain.raw");

	/** 载入纹理 */
	loadTexture();

	/** 初始化雾效 */
    initFog();
	
	return true;
}

/** 设置地形大小 */
void Terrain::setSize(unsigned  int width, unsigned  int cell)
{
	_nWidth = width;
	_nCellWidth = cell; 
}

/** 获得高度图中高度值 */
int Terrain::getHeight(int x, int y)
{
	if(!_pHeightMap)
		return 0;

	int xx = x % _nWidth;
	int yy = y % _nWidth;
    /** 返回当前高度 */
	return _pHeightMap[(xx + yy * _nWidth)];
}

/** 获得地面高度 */
float Terrain::getAveHeight(float x, float z)
{
	float cameraX, cameraZ;

	cameraX = x / _nCellWidth;
	cameraZ = z / _nCellWidth;

	/** 计算高程坐标(Col0, Row0)，(Col1, Row1) */
	int col0 = int(cameraX);
	int row0 = int(cameraZ);
	unsigned int col1 = col0 + 1;
	unsigned int row1 = row0 + 1;

	/** 确保单元坐标不超过高程以外 */
	if (col1 > _nWidth) col1 = 0;
	if (row1 > _nWidth) row1 = 0;

	/** 获取单元的四个角的高度 */
	float h00 = (float)(_pHeightMap[col0*_nCellWidth + row0*_nCellWidth*_nWidth]);
	float h01 = (float)(_pHeightMap[col1*_nCellWidth + row0*_nCellWidth*_nWidth]);
	float h11 = (float)(_pHeightMap[col1*_nCellWidth + row1*_nCellWidth*_nWidth]);
	float h10 = (float)(_pHeightMap[col0*_nCellWidth + row1*_nCellWidth*_nWidth]);

	/** 计算机摄像机相对于单元格的位置 */
	float tx = cameraX - int(cameraX);
	float ty = cameraZ - int(cameraZ);

	/** 进行双线性插值得到地面高度 */
	float txty = tx * ty;

	float final_height	= h00 * (1.0f - ty - tx + txty)
						+ h01 * (tx - txty)
						+ h11 * txty
						+ h10 * (ty - txty);
	return final_height;
}

bool Terrain::checkPos(float x, float z)
{
	if (x <= 0 || x >= _nWidth)
		return false;

	if (z <= 0 || z >= _nWidth)
		return false;

	return true;
}


/** 载入高度图 */
bool Terrain::loadRawFile(const char* fileName)
{
	FILE *pFile = NULL;

	/** 打开文件 */
	pFile = fopen( fileName, "rb" );

	/** 错误检查 */
	if (pFile == NULL)	
	{
		Log("打开高度图文件失败!");
		return false;
	}

	/** 读取高度图文件 */
	fread( _pHeightMap, 1, _nWidth*_nWidth, pFile );

	/** 获取错误代码 */
	int result = ferror(pFile);

	/** 检查错误代码 */
	if (result)
	{
		Log("无法获取高度数据!");
		return false;
	}
    
	/** 关闭文件，成功返回 */
	fclose(pFile);
	return true;
}

/** 设置纹理坐标 */
void Terrain::setTexCoord(float x,float z)
{
	float u =  (float)x / (float)_nWidth;
	float v = -(float)z / (float)_nWidth;
	
	///设置地面纹理的纹理坐标
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, u, v);

	///设置细节纹理的纹理坐标
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, u, v);
}


/** 载入地面纹理 */
bool Terrain::loadTexture()
{
	const char* fileName[] = {"res/terrain.bmp","res/detail.bmp"};	
	for(int i=0; i<2; i++)
	{
		if(!_texture[i].loadTexture(fileName[i]))
		{
			Log("装载位图文件失败!");
			return false;
		}
	}
  	
	return true;
}

/** 渲染地形 */
void Terrain::render()
{
	/** 检查高度图是否有效 */
	if(!_pHeightMap) 
		return;					

	/** 绑定纹理 */
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _texture[0].getID());
	
	/** 渲染细节纹理 */
	if(_bDetail)
	{
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);

		glBindTexture(GL_TEXTURE_2D, _texture[1].getID());
		
		/** 变换纹理矩阵 */
		glMatrixMode(GL_TEXTURE);
		    glLoadIdentity();
			glScalef((float)_detailScale, (float)_detailScale, 1);
		glMatrixMode(GL_MODELVIEW);
	}

	/** 绘制三角形带 */
	glBegin( GL_TRIANGLE_STRIP );			

	/** 从行(X)开始循环 */
	int x, y, z;
	bool bSwitchSides = false;
	for (int X = 0; X <= _nWidth; X += _nCellWidth )
	{
		/** 检查该列是否需要从相反顺序绘制 */
		if(bSwitchSides)
		{	
			/** 绘制地形的一列 */
			for (int Y = _nWidth; Y >= 0; Y -= _nCellWidth )
			{
				/** 顶点(X, Y, Z) */		
				x = X;							
				y = getHeight( X, Y );	
				z = Y;							

				/** 指定纹理坐标,并发送顶点 */
				//setFogCoord(m_FogDepth, (float)y);
				setTexCoord((float)x, (float)z);
				glVertex3f((float)x, (float)y, (float)z);

				/** 顶点(X + _nCellWidth, Y, Z) */		
				x = X + _nCellWidth; 
				y = getHeight(X + _nCellWidth, Y); 
				z = Y;

				/** 指定纹理坐标,并发送顶点 */
				//setFogCoord(m_FogDepth, (float)y);
				setTexCoord((float)x, (float)z);
				glVertex3f((float)x, (float)y, (float)z);
			}
		}
		else
		{	
			/** 绘制地形的一列 */
			for (int Y = 0; Y <= _nWidth; Y += _nCellWidth )
			{
				/** 顶点(X + _nCellWidth, Y, Z) */	
				x = X + _nCellWidth; 
				y = getHeight( X + _nCellWidth, Y ); 
				z = Y;

				/** 指定纹理坐标,并发送顶点 */
				//setFogCoord(m_FogDepth, (float)y);
				setTexCoord( (float)x, (float)z );
				glVertex3f((float)x, (float)y, (float)z);

				/** 顶点 (X, Y, Z) */		
				x = X;							
				y = getHeight(X, Y);	
				z = Y;							

				/** 指定纹理坐标,并发送顶点 */
				//setFogCoord(m_FogDepth, (float)y);
				setTexCoord((float)x, (float)z);
				glVertex3f((float)x, (float)y, (float)z);
			}
		}
			
		/** 变换开关 */
		bSwitchSides = !bSwitchSides;
	}

	/** 绘制结束 */
	glEnd();

	///关闭纹理单元1
	glActiveTextureARB(GL_TEXTURE1_ARB);
    glDisable(GL_TEXTURE_2D);

	//关闭纹理单元0
	glActiveTextureARB(GL_TEXTURE0_ARB);		
    glDisable(GL_TEXTURE_2D);
}

