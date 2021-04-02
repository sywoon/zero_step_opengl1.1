#include "3DSLoader.h"


C3DSLoader::C3DSLoader()
	: _filePointer(NULL)
{
	_currentChunk = new tChunk;
	_tempChunk = new tChunk;

	_3dModel.numOfObjects = 0;
	_3dModel.numOfMaterials = 0;

	for (int i=0; i<MAX_TEXTURES; i++) 
		_textures[i] = 0;
}


C3DSLoader::~C3DSLoader()
{
	_3dModel.numOfObjects = 0;
	_3dModel.numOfMaterials = 0;

	_3dModel.pObject.clear();
	_3dModel.pMaterials.clear();

	glDeleteTextures(MAX_TEXTURES, _textures);
	cleanUp();
}

/** 释放内存并关闭文件 */ 
void C3DSLoader::cleanUp()
{
	if (_filePointer)
	{
		fclose(_filePointer);
		_filePointer = NULL;
	}

	SAVE_DELETE(_currentChunk);
	SAVE_DELETE(_tempChunk);
}

/** 初时化3DS文件 */
void C3DSLoader::init(char *filename)
{	
	import3DS(&_3dModel, filename);	

	for(int i =0; i<_3dModel.numOfMaterials; i++)
	{ 
		if(strlen(_3dModel.pMaterials[i].strFile)>0)                 /**< 判断是否是一个文件名 */
		   loadTexture(_3dModel.pMaterials[i].strFile, _textures, i);/**< 使用纹理文件名称来装入位图 */		 
	   
		/** 设置材质的纹理ID */
		_3dModel.pMaterials[i].texureId = i;                     
	} 
}

/** 显示3ds模型 */
void C3DSLoader::draw() 
{
	glPushAttrib(GL_CURRENT_BIT); /**< 保存现有颜色属实性 */
	glDisable(GL_TEXTURE_2D);
	
	/**< 遍历模型中所有的对象 */
	for(int i = 0; i < _3dModel.numOfObjects; i++)
	{
		if(_3dModel.pObject.size() <= 0) 
			break;                   /**< 如果对象的大小小于0，则退出 */
	 
		t3DObject *pObject = &_3dModel.pObject[i];/**< 获得当前显示的对象 */
	 
		if(pObject->bHasTexture)                  /**< 判断该对象是否有纹理映射 */
		 {	
			 glEnable(GL_TEXTURE_2D);             /**< 打开纹理映射 */
			 glBindTexture(GL_TEXTURE_2D, _textures[pObject->materialID]);
		 } 
	 else	
		 glDisable(GL_TEXTURE_2D);                /**< 关闭纹理映射 */
	 
	 glColor3ub(255, 255, 255);

	 /** 开始绘制 */
	 glBegin(GL_TRIANGLES);
	 for(int j = 0; j < pObject->numOfFaces; j++)		/**< 遍历所有的面 */
	 {for(int tex = 0; tex < 3; tex++)					/**< 遍历三角形的所有点 */
		{
			int index = pObject->pFaces[j].vertIndex[tex];	/**< 获得面对每个点的索引 */
		 
			glNormal3f(pObject->pNormals[index].x,pObject->pNormals[index].y,  
			        pObject->pNormals[index].z);		/**< 给出法向量 */
		 
		 if(pObject->bHasTexture)						/**< 如果对象具有纹理 */
		 {	
			 if(pObject->pTexVerts)						/**< 确定是否有UVW纹理坐标 */
				glTexCoord2f(pObject->pTexVerts[index].x,pObject->pTexVerts[index].y);
		 }
		 else
		 {	
			 if(_3dModel.pMaterials.size() && pObject->materialID>= 0) 
			{	
				BYTE *pColor = _3dModel.pMaterials[pObject->materialID].color;
				glColor3ub(pColor[0],pColor[1],pColor[2]);
			}
		 }
		 glVertex3f(pObject->pVerts[index].x,pObject->pVerts[index].y,pObject->pVerts[index].z);
		}
	 }
	glEnd(); /**< 绘制结束 */
	}
	glEnable(GL_TEXTURE_2D);
	
	glPopAttrib();   /**< 恢复前一属性 */
}

/** 转载纹理 */
void C3DSLoader::loadTexture(char* filename, GLuint textureArray[], GLuint textureID)
{

	if (!filename)
		return;

	/** 载入位图 */
	std::string path = "res/";
	path += filename;
	if (!_bmpTexture.readBitmap(path.c_str()))
	{
		Log("c3d::loadTexture failed:%s", path.c_str());
		MessageBox(NULL, "c3d::loadTexture failed!", "error", MB_OK);
		exit(0);
	}
	glGenTextures(1, &_textures[textureID]);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, _textures[textureID]);

	/** 控制滤波 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	/** 创建纹理 */
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, _bmpTexture.getImgWidth(), _bmpTexture.getImgHeight(), GL_RGB,
		GL_UNSIGNED_BYTE, _bmpTexture.getImgData());
}

/** 载入3DS文件到模型结构中 */
bool C3DSLoader::import3DS(t3DModel *pModel, char *strFileName)
{	
	_filePointer = fopen(strFileName, "rb");
	if(!_filePointer) 
	{	
		char strMessage[255] = {0};
		sprintf(strMessage, "找不到文件: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}
	
	/** 将文件的第一块读出并判断是否是3ds文件 */
	readChunk(_currentChunk);
	
	/** 确保是3ds文件 */
	if (_currentChunk->ID != PRIMARY)
	{	
		MessageBox(NULL, "不能加载主块!", "Error", MB_OK);
		return false;
	}
	
	/** 递归读出对象数据 */
	readNextChunk(pModel, _currentChunk);
	
	/** 计算顶点的法线 */
	computeNormals(pModel);
	
	/** 释放内存空间 */
	cleanUp();
	
	return true;
}

/**  读入一个字符串 */
int C3DSLoader::getNextString(char *pBuffer)
{	
	int index = 0;
	
	/** 读入一个字节的数据 */
	fread(pBuffer, 1, 1, _filePointer);
	
	/** 直到结束 */
	while (*(pBuffer + index++) != 0) 
	{
		/** 读入一个字符直到NULL */
		fread(pBuffer + index, 1, 1, _filePointer);
	}
	
	/** 返回字符串的长度 */
	return strlen(pBuffer) + 1;
}

/** 读入块的ID号和它的字节长度 */
void C3DSLoader::readChunk(tChunk *pChunk)
{	
	// 读入块的ID号 */
	pChunk->bytesRead = fread(&pChunk->ID, 1, 2, _filePointer);
	
	/** 读入块占用的长度 */
	pChunk->bytesRead += fread(&pChunk->length, 1, 4, _filePointer);
}


/** 读出3ds文件的主要部分 */
void C3DSLoader::readNextChunk(t3DModel *pModel, tChunk *pPreChunk)
{	
	t3DObject newObject = {0};					/**< 用来添加到对象链表 */
	tMatInfo newTexture = {0};				    /**< 用来添加到材质链表 */
	unsigned int version = 0;					/**< 保存文件版本 */
	int buffer[50000] = {0};					/**< 用来跳过不需要的数据 */
	_currentChunk = new tChunk;					/**< 为新的块分配空间		 */
	
	/** 继续读入子块 */
	while (pPreChunk->bytesRead < pPreChunk->length)
	{	
		/** 读入下一个块 */
		readChunk(_currentChunk);
		
		/** 判断块的ID号 */
		switch (_currentChunk->ID)
		{
		/** 文件版本号 */
		case VERSION:							
			/** 读入文件的版本号 */
			_currentChunk->bytesRead += fread(&version, 1, _currentChunk->length - _currentChunk->bytesRead, _filePointer);
			
			/** 如果文件版本号大于3，给出一个警告信息 */
			if (version > 0x03)
				MessageBox(NULL, " 该3DS文件版本大于3.0,可能不能正确读取", "警告", MB_OK);
			break;
		
			/** 网格版本信息 */
		case OBJECTINFO:						
			
			/** 读入下一个块 */
			readChunk(_tempChunk);
			
			/** 获得网格的版本号 */
			_tempChunk->bytesRead += fread(&version, 1, _tempChunk->length - _tempChunk->bytesRead, _filePointer);
			
			/** 增加读入的字节数 */
			_currentChunk->bytesRead += _tempChunk->bytesRead;
			
			/** 进入下一个块 */
			readNextChunk(pModel, _currentChunk);
			break;
		
		/** 材质信息 */
		case MATERIAL:							
			
			/** 材质的数目递增 */
			pModel->numOfMaterials++;
			
			/** 在纹理链表中添加一个空白纹理结构 */
			pModel->pMaterials.push_back(newTexture);
			
			/** 进入材质装入函数 */
			readNextMatChunk(pModel, _currentChunk);
			break;

		/** 对象名称 */
		case OBJECT:							
			
			/** 对象数递增 */
			pModel->numOfObjects++;
			
			/** 添加一个新的tObject节点到对象链表中 */
			pModel->pObject.push_back(newObject);
			
			/** 初始化对象和它的所有数据成员 */
			memset(&(pModel->pObject[pModel->numOfObjects - 1]), 0, sizeof(t3DObject));
			
			/** 获得并保存对象的名称，然后增加读入的字节数 */
			_currentChunk->bytesRead += getNextString(pModel->pObject[pModel->numOfObjects - 1].strName);
			
			/** 进入其余的对象信息的读入 */
			readNextObjChunk(pModel, &(pModel->pObject[pModel->numOfObjects - 1]), _currentChunk);
			break;
		
		/** 关键帧 */
		case EDITKEYFRAME:
			/** 跳过关键帧块的读入 */
			_currentChunk->bytesRead += fread(buffer, 1, _currentChunk->length - _currentChunk->bytesRead, _filePointer);
			break;

		default: 
			/**  跳过所有忽略的块的内容的读入 */
			_currentChunk->bytesRead += fread(buffer, 1, _currentChunk->length - _currentChunk->bytesRead, _filePointer);
			break;
		}
		
		/** 增加从最后块读入的字节数 */
		pPreChunk->bytesRead += _currentChunk->bytesRead;
	}
	
	/** 释放当前块的内存空间 */
	delete _currentChunk;
	_currentChunk = pPreChunk;

}
/** 处理所有的文件中对象的信息 */
void C3DSLoader::readNextObjChunk(t3DModel *pModel, t3DObject *pObject, tChunk *pPreChunk)
{	
	int buffer[50000] = {0};					/** 用于读入不需要的数据 */
	
	/** 对新的块分配存储空间 */
	_currentChunk = new tChunk;
	
	/** 继续读入块的内容直至本子块结束 */
	while (pPreChunk->bytesRead < pPreChunk->length)
	{	
		/** 读入下一个块 */
		readChunk(_currentChunk);
		
		/** 区别读入是哪种块 */
		switch (_currentChunk->ID)
		{
		case OBJ_MESH:					/**< 正读入的是一个新块 */
			
			/** 使用递归函数调用，处理该新块 */
			readNextObjChunk(pModel, pObject, _currentChunk);
			break;
		case OBJ_VERTICES:				/**< 读入是对象顶点 */
			readVertices(pObject, _currentChunk);
			break;
		case OBJ_FACES:					/**< 读入的是对象的面 */
			readVertexIndices(pObject, _currentChunk);
			break;
		case OBJ_MATERIAL:				/**< 读入的是对象的材质名称 */
			
			/** 读入对象的材质名称 */
			readObjMat(pModel, pObject, _currentChunk);			
			break;
		case OBJ_UV:						/**< 读入对象的UV纹理坐标 */
			/** 读入对象的UV纹理坐标 */
			readUVCoordinates(pObject, _currentChunk);
			break;
		default:  
			/** 略过不需要读入的块 */
			_currentChunk->bytesRead += fread(buffer, 1, _currentChunk->length - _currentChunk->bytesRead, _filePointer);
			break;
		}
		/** 添加从最后块中读入的字节数到前面的读入的字节中 */
		pPreChunk->bytesRead += _currentChunk->bytesRead;
	}
	/** 释放当前块的内存空间，并把当前块设置为前面块 */
	delete _currentChunk;
	_currentChunk = pPreChunk;
}
/** 处理所有的材质信息 */
void C3DSLoader::readNextMatChunk(t3DModel *pModel, tChunk *pPreChunk)
{	
	int buffer[50000] = {0};					/**< 用于读入不需要的数据 */
	/** 给当前块分配存储空间 */
	_currentChunk = new tChunk;
	/** 继续读入这些块 */
	while (pPreChunk->bytesRead < pPreChunk->length)
	{	
		/** 读入下一块 */
		readChunk(_currentChunk);
		/** 判断读入的是什么块 */
		switch (_currentChunk->ID)
		{
		case MATNAME:							/**< 材质的名称 */
			/** 读入材质的名称 */
			_currentChunk->bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strName, 1, _currentChunk->length - _currentChunk->bytesRead, _filePointer);
			break;
		case MATDIFFUSE:						/**< 对象的R G B颜色 */
			readColor(&(pModel->pMaterials[pModel->numOfMaterials - 1]), _currentChunk);
			break;
		case MATMAP:							/**< 纹理信息的头部 */
			/** 下一个材质块信息 */
			readNextMatChunk(pModel, _currentChunk);
			break;
		case MATMAPFILE:						/**< 材质文件的名称 */
			/** 读入材质的文件名称 */
			_currentChunk->bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strFile, 1, _currentChunk->length - _currentChunk->bytesRead, _filePointer);
			break;
		default:  
			/** 跳过不需要读入的块 */
			_currentChunk->bytesRead += fread(buffer, 1, _currentChunk->length - _currentChunk->bytesRead, _filePointer);
			break;
		}
		/** 添加从最后块中读入的字节数 */
		pPreChunk->bytesRead += _currentChunk->bytesRead;
	}
	/** 删除当前块，并将当前块设置为前面的块 */
	delete _currentChunk;
	_currentChunk = pPreChunk;
}


/** 读入RGB颜色 */
void C3DSLoader::readColor(tMatInfo *pMaterial, tChunk *pChunk)
{	
	/** 读入颜色块信息 */
	readChunk(_tempChunk);
	/** 读入RGB颜色 */
	_tempChunk->bytesRead += fread(pMaterial->color, 1, _tempChunk->length - _tempChunk->bytesRead, _filePointer);
	/** 增加读入的字节数 */
	pChunk->bytesRead += _tempChunk->bytesRead;
}
/** 读入顶点索引 */
void C3DSLoader::readVertexIndices(t3DObject *pObject, tChunk *pPreChunk)
{	
	unsigned short index = 0;					/**< 用于读入当前面的索引 */
	/** 读入该对象中面的数目 */
	pPreChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, _filePointer);
	/** 分配所有面的存储空间，并初始化结构 */
	pObject->pFaces = new tFace [pObject->numOfFaces];
	memset(pObject->pFaces, 0, sizeof(tFace) * pObject->numOfFaces);
	/** 遍历对象中所有的面 */
	for(int i = 0; i < pObject->numOfFaces; i++)
	{	for(int j = 0; j < 4; j++)
		{	
			/** 读入当前面的第一个点  */
			pPreChunk->bytesRead += fread(&index, 1, sizeof(index), _filePointer);
			if(j < 3)
			{	
				/** 将索引保存在面的结构中 */
				pObject->pFaces[i].vertIndex[j] = index;
			}
		}
	}
}

/** 读入对象的UV坐标 */
void C3DSLoader::readUVCoordinates(t3DObject *pObject, tChunk *pPreChunk)
{	
	/** 读入UV坐标的数量 */
	pPreChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, _filePointer);
	
	/** 分配保存UV坐标的内存空间 */
	pObject->pTexVerts = new Vector2 [pObject->numTexVertex];
	
	/** 读入纹理坐标 */
	pPreChunk->bytesRead += fread(pObject->pTexVerts, 1, pPreChunk->length - pPreChunk->bytesRead, _filePointer);
}

/**  读入对象的顶点 */
void C3DSLoader::readVertices(t3DObject *pObject, tChunk *pPreChunk)
{	
	/** 读入顶点的数目 */
	pPreChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, _filePointer);
	
	/** 分配顶点的存储空间，然后初始化结构体 */
	pObject->pVerts = new Vector3 [pObject->numOfVerts];
	memset(pObject->pVerts, 0, sizeof(Vector3) * pObject->numOfVerts);
	
	/** 读入顶点序列 */
	pPreChunk->bytesRead += fread(pObject->pVerts, 1, pPreChunk->length - pPreChunk->bytesRead, _filePointer);
	
	/** 遍历所有的顶点将Y轴和Z轴交换，然后将Z轴反向 */
	for(int i = 0; i < pObject->numOfVerts; i++)
	{	
		/** 保存Y轴的值 */
		float fTempY = pObject->pVerts[i].y;
		/** 设置Y轴的值等于Z轴的值 */
		pObject->pVerts[i].y = pObject->pVerts[i].z;
		/** 设置Z轴的值等于-Y轴的值  */
		pObject->pVerts[i].z = -fTempY;
	}
}

/** 读入对象的材质名称 */
void C3DSLoader::readObjMat(t3DModel *pModel, t3DObject *pObject, tChunk *pPreChunk)
{	
	char strMaterial[255] = {0};			/**< 用来保存对象的材质名称 */
	int buffer[50000] = {0};				/**< 用来读入不需要的数据 */
	
	/** 读入赋予当前对象的材质名称 */
	pPreChunk->bytesRead += getNextString(strMaterial);
	
	/** 遍历所有的纹理 */
	for(int i = 0; i < pModel->numOfMaterials; i++)
	{	
		/** 如果读入的纹理与当前的纹理名称匹配 */
		if(strcmp(strMaterial, pModel->pMaterials[i].strName) == 0)
		{	
			/** 设置材质ID */
			pObject->materialID = i;
			
			/** 判断是否是纹理映射 */
			if(strlen(pModel->pMaterials[i].strFile) > 0) {
				
				/** 设置对象的纹理映射标志 */
				pObject->bHasTexture = true;
			}	
			break;
		}
		else
		{	
			/** 如果该对象没有材质，则设置ID为-1 */
			pObject->materialID = -1;
		}
	}
	pPreChunk->bytesRead += fread(buffer, 1, pPreChunk->length - pPreChunk->bytesRead, _filePointer);
}


/** 计算对象的法向量 */
void C3DSLoader::computeNormals(t3DModel *pModel)
{	
	Vector3 vVector1, vVector2, vNormal, vPoly[3];
	
	/** 如果模型中没有对象，则返回 */
	if(pModel->numOfObjects <= 0)
		return;
	
	/** 遍历模型中所有的对象 */
	for(int index = 0; index < pModel->numOfObjects; index++)
	{	
		/** 获得当前的对象 */
		t3DObject *pObject = &(pModel->pObject[index]);
		
		/** 分配需要的存储空间 */
		Vector3 *pNormals		= new Vector3 [pObject->numOfFaces];
		Vector3 *pTempNormals	= new Vector3 [pObject->numOfFaces];
		pObject->pNormals		= new Vector3 [pObject->numOfVerts];
		
		/** 遍历对象的所有面 */
		for(int i=0; i < pObject->numOfFaces; i++)
		{	vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];
			
			/** 计算面的法向量 */
			vVector1 = vPoly[0] - vPoly[2];		        /**< 获得多边形的矢量 */
			vVector2 = vPoly[2] - vPoly[1];		        /**< 获得多边形的第二个矢量 */
			vNormal  = vVector1.crossProduct(vVector2);	/**< 计算两个矢量的叉积 */
			pTempNormals[i] = vNormal;					
			vNormal  = vNormal.normalize();				/**< 规一化叉积 */
			pNormals[i] = vNormal;						/**< 将法向量添加到法向量列表中 */
		}
		
		/** 计算顶点法向量 */
		Vector3 vSum(0.0,0.0,0.0);
		Vector3 vZero = vSum;
		int shared=0;
		
		/** 遍历所有的顶点 */
		for (int i = 0; i < pObject->numOfVerts; i++)			
		{	for (int j = 0; j < pObject->numOfFaces; j++)	/**< 遍历所有的三角形面 */
			{												/**< 判断该点是否与其它的面共享 */
				if (pObject->pFaces[j].vertIndex[0] == i || 
					pObject->pFaces[j].vertIndex[1] == i || 
					pObject->pFaces[j].vertIndex[2] == i)
				{	
					vSum = vSum + pTempNormals[j];
					shared++;								
				}
			}      
			pObject->pNormals[i] = vSum / float(-shared);
			
			/** 规一化顶点法向 */
			pObject->pNormals[i] = pObject->pNormals[i].normalize();	
			vSum = vZero;								
			shared = 0;										
		}
		/** 释放存储空间，开始下一个对象 */
		delete [] pTempNormals;
		delete [] pNormals;
	}
}








