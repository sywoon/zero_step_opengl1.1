#include "MD2Loader.h"

using namespace MD2;

CMD2Loader::CMD2Loader()
{
	/** 为文件头分配内存 */
	memset(&_header, 0, sizeof(tMd2Header));

	/** 初时化指针为空 */
	_pSkins=NULL;
	_pTexCoords=NULL;
	_pTriangles=NULL;
	_pFrames=NULL;

	/** 初时化纹理数组 */
	for(int i=0; i<MAX_TEXTURES; i++)
		_textures[i]=0;

}


CMD2Loader::~CMD2Loader()
{
	/** 所有数目清零 */
	_3DModel.numOfObjects = 0;
	_3DModel.numOfMaterials = 0;
	_3DModel.numOfAnimations = 0;
	
	/** 将容器清空 */
	_3DModel.pObject.clear();
	_3DModel.pMaterials.clear();
	_3DModel.pAnimations.clear();
	
	/** 删除纹理 */
	glDeleteTextures(MAX_TEXTURES, _textures);
}

/** 释放内存 */
void CMD2Loader::cleanUp()
{
	/** 关闭指针 */
	fclose(m_FilePointer);						

	/** 释放内存 */
	if(_pSkins)	 delete [] _pSkins;		
	if(_pTexCoords) delete _pTexCoords;		
	if(_pTriangles) delete _pTriangles;		
	if(_pFrames)	 delete _pFrames;	

}

/** 初始化 */
void CMD2Loader::init(char *strFileName, char *strTexture)
{
	/** 载入MD2文件 */
	importMD2(&_3DModel,strFileName,strTexture);
   
	for(int i =0; i<_3DModel.numOfMaterials;i++)
	{ 
		if(strlen(_3DModel.pMaterials[i].strFile)>0)                 /**< 判断是否是一个文件名 */
		   loadTexture(_3DModel.pMaterials[i].strFile,_textures, i);/**< 使用纹理文件名称来装入位图 */		 
	   
		/** 设置材质的纹理ID */
		_3DModel.pMaterials[i].texureId = i;                     
	} 
}

/** 载入MD2文件 */
bool CMD2Loader::importMD2(t3DModel *pModel, char *strFileName, char *strTexture)
{
	char strMessage[255] = {0};

	/** 打开文件 */
	m_FilePointer = fopen(strFileName, "rb");

	/** 检查文件指针 */
	if(!m_FilePointer) 
	{
		
		sprintf(strMessage, "打开文件: %s错误!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}
	
	/** 读取文件头 */
	fread(&_header, 1, sizeof(tMd2Header), m_FilePointer);

	/** 检查版本号 */
	if(_header.version != 8)
	{
		
		sprintf(strMessage, "Invalid file format (Version not 8): %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}

	/** 读取MD2文件数据 */
	readMD2Data();
	
	/** 将MD2数据转换为模型结构 */
	convertDataStructures(pModel);

	/** 如果具有纹理文件 */
	if(strTexture)
	{
		/** 材质信息结构体 */
		tMaterialInfo texture;

		/** 复制文件名 */
		strcpy(texture.strFile, strTexture);

		/** 纹理ID号为0 */
		texture.texureId = 0;

		texture.uTile = texture.uTile = 1;

		/** 模型材质数为1 */
		pModel->numOfMaterials = 1;

		/** 添加材质信息 */
		pModel->pMaterials.push_back(texture);
	}

	/** 释放内存 */
	cleanUp();
	
	return true;     /**< 成功返回 */
}

/** 读取数据 */
void CMD2Loader::readMD2Data()
{
	/** 为帧动画分配足够大内存 */
	unsigned char buffer[MD2_MAX_FRAMESIZE];

	/** 分配内存 */
	_pSkins     = new tMd2Skin [_header.numSkins];
	_pTexCoords = new tMd2TexCoord [_header.numTexCoords];
	_pTriangles = new tMd2Face [_header.numTriangles];
	_pFrames    = new tMd2Frame [_header.numFrames];

	/** 指针移到纹理数据位置,并读取纹理 */
	fseek(m_FilePointer, _header.offsetSkins, SEEK_SET);
	fread(_pSkins, sizeof(tMd2Skin), _header.numSkins, m_FilePointer);
		
	/** 指针移到纹理坐标位置,并读取纹理坐标 */
	fseek(m_FilePointer, _header.offsetTexCoords, SEEK_SET);
	fread(_pTexCoords, sizeof(tMd2TexCoord), _header.numTexCoords, m_FilePointer);

	/** 指针移到三角形面位置,并读取面信息 */
	fseek(m_FilePointer, _header.offsetTriangles, SEEK_SET);
	fread(_pTriangles, sizeof(tMd2Face), _header.numTriangles, m_FilePointer);
			
	/** 指针移到顶点数据位置 */
	fseek(m_FilePointer, _header.offsetFrames, SEEK_SET);

	/** 对每帧解压顶点数据 */
	for (int i=0; i < _header.numFrames; i++)
	{
		
		tMd2AliasFrame *pFrame = (tMd2AliasFrame *) buffer;

		/** 分配顶点内存 */
		_pFrames[i].pVertices = new tMd2Triangle [_header.numVertices];

		/** 读取第一帧 */
		fread(pFrame, 1, _header.frameSize, m_FilePointer);

		/** 保存动画名称 */
		strcpy(_pFrames[i].strName, pFrame->name);
			
		tMd2Triangle *pVertices = _pFrames[i].pVertices;

		/** 对所有顶点通过缩放和偏移进行解压,并交换坐标 */
		for (int j=0; j < _header.numVertices; j++)
		{
			pVertices[j].vertex[0] = pFrame->aliasVertices[j].vertex[0] * pFrame->scale[0] + pFrame->translate[0];
			pVertices[j].vertex[2] = -1 * (pFrame->aliasVertices[j].vertex[1] * pFrame->scale[1] + pFrame->translate[1]);
			pVertices[j].vertex[1] = pFrame->aliasVertices[j].vertex[2] * pFrame->scale[2] + pFrame->translate[2];
		}
	}
}

/** 装载纹理 */
void CMD2Loader::loadTexture(char* filename, GLuint textureArray[], GLuint textureID)
{

	if(!filename)
		return;
	
	/** 载入位图 */
	if(!_bmpTexture.readBitmap(filename))
	{
		MessageBox(NULL,"载入位图失败!","错误",MB_OK);
		exit(0);
	}
	glGenTextures(1,&_textures[textureID]);
	
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, _textures[textureID]);
	
	/** 控制滤波 */
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	
	/** 创建纹理 */
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, _bmpTexture.getImgWidth(), _bmpTexture.getImgHeight(), GL_RGB,
		              GL_UNSIGNED_BYTE, _bmpTexture.getImgData());
		
}

/** 分析并获取帧信息 */
void CMD2Loader::parseAnimations(t3DModel *pModel)
{
	tAnimationInfo animation;
	string strLastName = "";

	/** 循环处理 */
	for(int i = 0; i < pModel->numOfObjects; i++)
	{
		/** 保存帧名 */
		string strName  = _pFrames[i].strName;
		int frameNum = 0;
		
		/** 分析该帧名字 */
		for(unsigned int j = 0; j < strName.length(); j++)
		{
			/** 如果它是数字而且是最后两个字符之一 */
			if( isdigit(strName[j]) && j >= strName.length() - 2)
			{
				/** 将字符转换为数字 */
				frameNum = atoi(&strName[j]);

				/** 清除数字*/
				strName.erase(j, strName.length() - j);
				break;
			}
		}

		/** 如果该帧名字和前一帧不同或者该帧为结束帧 */
		if(strName != strLastName || i == pModel->numOfObjects - 1)
		{
			/** 检查是否为开始帧 */
			if(strLastName != "")
			{
				/** 复制帧名称 */
				strcpy(animation.strName, strLastName.c_str());

				/** 设置动作结束帧序号为i */
				animation.endFrame = i;

				/** 将该帧添加到动作列表中 */
				pModel->pAnimations.push_back(animation);
				memset(&animation, 0, sizeof(tAnimationInfo));

				/** 动作数加一 */
				pModel->numOfAnimations++;
			}

			/** 设置开始帧 */
			animation.startFrame = frameNum - 1 + i;
		}

		/** 保存帧的名字 */
		strLastName = strName;
	}
}

/** 将各帧数据保存在模型结构中 */
void CMD2Loader::convertDataStructures(t3DModel *pModel)
{
	int j = 0, i = 0;

	/** 分配内存 */
	memset(pModel, 0, sizeof(t3DModel));
	
	/** 设置对象数目为关键帧数目 */
	pModel->numOfObjects = _header.numFrames;

	/** 产生动作信息 */
	parseAnimations(pModel);

	/** 将各帧顶点数据添加到模型对象链表中 */
	for (i=0; i < pModel->numOfObjects; i++)
	{
		/** 保存第一帧数据 */
		t3DObject currentFrame = {0};

		/** 指定顶点 纹理坐标和面的个数 */
		currentFrame.numOfVerts   = _header.numVertices;
		currentFrame.numTexVertex = _header.numTexCoords;
		currentFrame.numOfFaces   = _header.numTriangles;

		/** 分配内存 */
		currentFrame.pVerts    = new Vector3 [currentFrame.numOfVerts];

		/** 循环处理所有顶点 */
		for (j=0; j < currentFrame.numOfVerts; j++)
		{
			currentFrame.pVerts[j].x = _pFrames[i].pVertices[j].vertex[0];
			currentFrame.pVerts[j].y = _pFrames[i].pVertices[j].vertex[1];
			currentFrame.pVerts[j].z = _pFrames[i].pVertices[j].vertex[2];
		}

		/** 释放内存 */
		delete _pFrames[i].pVertices;
	
	    /** 检查是否超过第一帧 */
		if(i > 0)
		{
			/** 将该帧添加到模型对象中 */
			pModel->pObject.push_back(currentFrame);
			continue;	
		}

		/** 分配内存 */
		currentFrame.pTexVerts = new Vector2 [currentFrame.numTexVertex];
		currentFrame.pFaces    = new tFaceTriangle [currentFrame.numOfFaces];

		/** 处理纹理坐标 */
		for(j=0; j < currentFrame.numTexVertex; j++)
		{
			currentFrame.pTexVerts[j].x = _pTexCoords[j].u / float(_header.skinWidth);
			currentFrame.pTexVerts[j].y = 1 - _pTexCoords[j].v / float(_header.skinHeight);
		}

		/** 保存面数据信息 */
		for(j=0; j < currentFrame.numOfFaces; j++)
		{
			/** 为面顶点坐标索引赋值 */
			currentFrame.pFaces[j].vertIndex[0] = _pTriangles[j].vertexIndices[0];
			currentFrame.pFaces[j].vertIndex[1] = _pTriangles[j].vertexIndices[1];
			currentFrame.pFaces[j].vertIndex[2] = _pTriangles[j].vertexIndices[2];

			/** 为面顶点纹理坐标索引赋值 */
			currentFrame.pFaces[j].coordIndex[0] = _pTriangles[j].textureIndices[0];
			currentFrame.pFaces[j].coordIndex[1] = _pTriangles[j].textureIndices[1];
			currentFrame.pFaces[j].coordIndex[2] = _pTriangles[j].textureIndices[2];
		}

		/** 将该帧添加到模型中 */
		pModel->pObject.push_back(currentFrame);
	}
}

/** 返回时间t,用于在两帧之间进行插值 */
float CMD2Loader::returnCurrentTime(t3DModel *pModel, int nextFrame)
{
	static float elapsedTime   = 0.0f;
	static float lastTime	  = 0.0f;

	/** 获取当前时间 */
	float time = (float)GetTickCount();

	/** 计算已用时间 */
	elapsedTime = time - lastTime;

	/** 根据已用时间计算t */
	float t = elapsedTime / (1000.0f / kAnimationSpeed);
	
	/** 如果elapsedTime大于 1s/kAnimationSpeed */
	if (elapsedTime >= (1000.0f / kAnimationSpeed) )
	{
		/** 进行下一帧 */
		pModel->currentFrame = nextFrame;

		/** 保存时间值 */
		lastTime = time;
	}

	/** 返回t */
	return t;
}

/** 通过关键帧插值实现模型绘制和动画 */
void CMD2Loader::animateMD2Model()
{
	
	if(_3DModel.pObject.size() <= 0) return;

	/** 获取当前动作信息 */
	tAnimationInfo *pAnim = &(_3DModel.pAnimations[_3DModel.currentAnim]);

	/** 计算下一帧 */
	int nextFrame = (_3DModel.currentFrame + 1) % pAnim->endFrame;

	/** 如果下一帧为0,则设为开始帧 */
	if(nextFrame == 0) 
		nextFrame =  pAnim->startFrame;

	/** 获取当前帧信息 */
	t3DObject *pFrame =		 &_3DModel.pObject[_3DModel.currentFrame];

	/** 获取下一帧信息 */
	t3DObject *pNextFrame =  &_3DModel.pObject[nextFrame];

	/** 获取第一帧信息 */
	t3DObject *pFirstFrame = &_3DModel.pObject[0];

	/** 返回t,进行帧插值 */
	float t = returnCurrentTime(&_3DModel, nextFrame);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textures[pFrame->materialID]);
	
	/** 开始绘制 */
	glBegin(GL_TRIANGLES);

		/** 遍历所有面 */
		for(int j = 0; j < pFrame->numOfFaces; j++)
		{
			/** 遍历面的所有顶点 */
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				/** 获得该面中顶点和纹理索引 */
				int vertIndex = pFirstFrame->pFaces[j].vertIndex[whichVertex];
				int texIndex  = pFirstFrame->pFaces[j].coordIndex[whichVertex];
						
				/** 检查是否具有纹理 */
				if(pFirstFrame->pTexVerts) 
				{
					/** 指定纹理坐标 */
					glTexCoord2f(pFirstFrame->pTexVerts[ texIndex ].x, 
								 pFirstFrame->pTexVerts[ texIndex ].y);
				}

								
				/** 下面开始进行帧插值 */
				Vector3 vPoint1 = pFrame->pVerts[ vertIndex ];
				Vector3 vPoint2 = pNextFrame->pVerts[ vertIndex ];

				/** 利用公式: p(t) = p0 + t(p1 - p0),得到插值点 */
				glVertex3f(vPoint1.x + t * (vPoint2.x - vPoint1.x), 
						   vPoint1.y + t * (vPoint2.y - vPoint1.y), 
						   vPoint1.z + t * (vPoint2.z - vPoint1.z));
			}
		}
    /** 渲染结束 */
	glEnd();	
}


