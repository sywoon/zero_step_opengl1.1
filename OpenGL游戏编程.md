# OpenGL 游戏编程

α、β、γ、θ


[透明窗口](http://landcareweb.com/questions/3655/ru-he-shi-yong-tou-ming-bei-jing-zhi-zuo-openglxuan-ran-shang-xia-wen)



# 第一章 游戏开发快速入门

* 游戏运行原理
```
初始化 -> 主循环 -> 获取输入 -> 执行逻辑 -> 场景更新 -> 绘制屏幕
			↑        ↓                                    ↓
			-----------------------------------------------
			          ↓
					退出
```
* GLUT (opengl utility toolkit)
```
帮助创建不同平台的窗口
```

* DirectX9的组成
```
  1. DirectX Graphics:将早期的Diret3D和DirectDraw合并，
	用于处理2D和3D图形显示
  2. DirectX Audio：将DirectMusic和DirectSound整合，
    用于处理音频和音乐。
  3. DirectInput 输入处理：鼠标、键盘、游戏杆
  4. DirectPlay 网络通信
  5. DirectShow 播放视频流和音频流
```

## 坐标系
* 左手坐标系：拇指(x)、食指(y)、中指(z)相互成垂直，z朝屏幕里
* 右 手坐标系：拇指(x)、食指(y)、中指(z)相互成垂直，z朝屏幕外 


## 向量
* C = A + B   A的终点为B的起点  C的起点为A的起点 终点为B的终点
* C = A - B   AB共用一个起点  C的起点为B的终点  终点为A的终点

* 向量的长度： |C| = sqrt(x*x + y*y + z*z)
* 向量归一化：即其长度为1
	Norm(C) = C / |C|

* 与标量相乘：与原向量平行 长度不同或方向相反
   kC = {kx, ky}  标量k与每个分量相乘

* 点乘(内积)：对应分量乘积的和  表示两个向量的相似程度 越大越相近
```
  C·D = Cx*Dx + Cy*Dy = |C||D|cos(α)
  ==>
  = 0  夹角 等于90度 
  > 0   夹角小于 90度 
  < 0   夹角 大于90度

  A·B == B·A
```

* 叉乘
```
 得到一个新的向量 用于碰撞检测、光照和物理计算
 A x B = (AyBz-AzBy, AzBx-AxBz, AxBy-Aybx) = -(B x A)
 大小 = |A||B|sin(α)  
 方向垂直于AB面  根据右手法则确定方向(A对拇指 B对食指 结果就是中指的方向)

 A x B ~= B x A  大小相同 方向相反 == -(B x A)
```


## 坐标系
* 全局or世界
* 局部or物体
* 观察(视棱锥)
* 设备坐标(DC)和规格化设备坐标(NDC)
  设备坐标：与图形设备的物理参数有关
  规格化设备坐标：抽象坐标xy都是[0,1]
* 屏幕坐标or像素坐标
  左上(0,0) x向右 y向下


## 矩阵
* 方阵：行列数相同
* 对角矩阵：除了对角线外 其余值都为0的方阵
* 单位矩阵：对角线值为1的对角矩阵
      特性：矩阵x同维单位矩阵=矩阵自己

### 矩阵的运算
* 加减法：对应位置的元素做加减
    C(ij) = A(ij)+B(ij)
* 乘法：
```
  标量k乘矩阵：k乘每个元素
  向量乘矩阵：可看作一行或一列的特殊矩阵
  只有两种组合：
      1. 行向量 左乘 矩阵 => 行向量
	  [x,y,z] x A(3x3) = [x1,y1,z1]  相当于向量和A的每一列做点乘
	  2. 矩阵 右乘 列向量 => 列向量
  矩阵乘矩阵： 不支持交换律
      A(m,n) x B(n,r) = C(m,r)
	  C(i,j) = A(i,k)B(k,j) A的第i行和B的第j列做点乘
```


## 3D图形处理流程
坐标转换：
```
世界坐标系下的三维物体 => 三维几何变换(平移 缩放 旋转) =>
投影(三维视锥体) => 三维裁剪(保留仅在视锥体内部的部分)
=> 视口变换(屏幕内定一个矩形 viewport 投影后的图像显示在内)
=> 屏幕坐标系中的图形显示(转换为像素坐标)
```

## 二维几何变换
### 平移 translation
```
  x' = x + dx
  y' = y + dy
=>
  P' = P + T(m)
```

### 旋转
  P(x,y)绕原点 逆时针旋转α得到P'点
```
  其P到P'的角度为θ
可知：
  x = rcos(α)
  y = rsin(α)
  x' = r cos(α+ θ)
  y' = r sin(α+ θ)
=>
  x' = rcos(α)cos(θ) - rsin(α)sin(θ)
  y' = rsin(α)cos(θ) + cos(α)sin(θ)
=>
  x' = xcos(θ) - ysin(θ)
  y' = ycos(θ) + xsin(θ)

令
T(r) = |cosθ sinθ|
	   |-sinθcosθ|
则
[x' y'] = [x y]T(r)
=>
P' = P x T(r)
```


### 缩放 scale
```
  x' = x Sx
  y' = y Sy
令
T(s) = |Sx  0|
	   |0  Sy|
则
P' = P x T(s)
```





## 齐次坐标
由于平移变换不同于旋转和缩放 从而引入齐次坐标的概念
将n维的问题 转移到n+1维空间上
(x,y) => (x,y,h) 当h=1时 表示规格化齐次坐标

## 二维空间的三种变换：
* 平移
```
  [x',y',1] = [x,y,1] x |1  0  0|
						|0  1  0|
						|dx dy 1|
```

* 旋转
```
  [x',y',1] = [x,y,1] x |cosθsinθ 0|
						|-sinθcosθ0|
						|  0   0    1|
```

* 缩放
```
  [x',y',1] = [x,y,1] x |Sx  0  0|
						|0  Sy  0|
						|0  0   1|
```


## 三维空间的三种变换：
* 平移
```
  [x',y',z',1] = [x,y,z,1] x |1  0  0  0|
							 |0  1  0  0|
							 |0  0  1  0|
							 |dx dy dx 1|
```

* 旋转
* 绕x轴需旋转
```
  x' = x
  y' = ycosθ- zsinθ
  z' = ysinθ+ zcosθ
  [x',y',z',1] = [x,y,z,1] x|1   0    0   0|
							|0 cosθsinθ 0|
							|0 -sinθcosθ0|
							|0   0   0    1|
```

* 绕y轴旋转
```
  x' = xcosθ+ zsinθ
  y' = y
  z' = -xsinθ+ zcosθ
  [x',y',z',1] = [x,y,z,1] x|cosθ 0  -sinθ 0|
							|0     1    0    0|
							|sinθ 0   cosθ 0|
							|0     0    0    1|
```

* 绕z轴旋转
```
  x' = xcosθ- ysinθ
  y' = xsinθ+ ycosθ
  z' = z
  [x',y',z',1] = [x,y,z,1] x|cosθ sinθ0   0|
							|-sinθcosθ0   0|
							|0     1    0   0|
							|0     0    0   1|
```

* 缩放
```
  [x',y',z',1] = [x,y,z,1] x |Sx  0  0   0|
							 |0  Sy  0   0|
							 |0  0   Sz  0|
							 |0  0   0   1|
```


## 投影
* 正交投影(平行投影)  舍弃了深度信息
```
x' = x
y' = y
z' = 0
```

* 斜交投影(透视投影)
```
近大远小 模仿出实际视觉效果
远处的点通过投影面 聚焦到投影中心

  已知：投影中心 Zprp  投影面Zvp u:[0,1]
	x' = x - xu
	y' = y - yu
	z' = z - (z - Zprp)u

	当u=0,表示P点本身处
	x' = x
	y' = y
	z' = z
	当u=1,表示投影中心处
	x' = 0
	y' = 0
	z' = Zprp

	在观察面上：z' = Zvp
	==>
	u = (Zvp - z) / (Zprp - z)
	==>
	x' = x*(Zprp - Zvp)/(Zprp - z)
	y' = y*(Zprp - Zvp)/(Zprp - z)

	令 d = Zprp - Zvp 投影中心到投影面的距离
	==>
	x' = x * d / (Zprp - z)
	y' = y * d / (Zprp - z)

	扩展到矩阵：
	==>
	[x' y' z' 1] = [x y z 1] |d/(Zprp-z)  0    0  0|
							 |  0  d/(Zprp-z)  0  0|
							 |  0         0   Zvp 0|
							 |  0         0    0  1|
	令 h = (Zprp - z)/d   其中d = Zprp - Zvp
	==> 提取h值到外部
	[x' y' z' 1] = [x y z 1] * (1/h) |1   0   0     0|
									 |0   1   0     0|
									 |0   0   Zvp/h 0|
									 |0   0   0     h|
	==>
	x' = x / h
	y' = y / h
	z' = Zvp
	==>
	当Zvp = 0, 投影面在xy面，则
	x' = x * Zprp / (Zprp - z)
	y' = y * Zprp / (Zprp - z)
	Z' = 0

	当Zpvp = 0, 投影中心在(0,0,0)点，则
	x' = x * Zvp / z
	y' = y * Zvp / z
	Z' = Zvp
```



## 裁剪
6个面组成的观察体 z方向深度有限 根据投影方式不同分为：
* 立方体
* 菱台



# 第二章 OpenGL程序框架
OpenGL是纯图形api，和窗口无关，不同平台提供额外的扩展，来支持OpenGL。
windows平台上被称为wiggle函数，以wgl开头。
另glut函数库，做了相关的封装，免去平台底层创建窗口部分。
glew函数库，提供了不同平台的统一封装，无需判断某个gl函数是否存在。


## GDI设备环境(Graphics Device Interface)
* windows窗口默认的绘图方式
```
每个窗口有一个设备环境，每个GDI函数都需要传入设备环境。
记录当前画刷、画笔等
HDC:Handle of Device Context  设备环境句柄 
 case WM_PAINT:
   PAINTSTRUCT ps;
   BeginPaint(hWnd, &ps);
   Rectangle(ps.hdc, 100, 100, 150, 150);
```


## OpenGL渲染环境 rendering context
* 与GDI相似，负责记住当前颜色、状态等信息。
```
窗口被opengl渲染前，需要设置具体的参数:
	用硬件渲染还是软件渲染、单缓冲还是双缓冲、
	是否开启深度缓冲、模板缓冲、alpha缓冲
	一旦设定不能修改，只能销毁后重新创建窗口
```


### 像素格式
* 窗口3D特性的设置 
[PIXELFORMATDESCRIPTOR](https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/ns-wingdi-tagpixelformatdescriptor)
```
windows中 通过像素格式描述表：
  由硬件的opengl驱动或软件渲染器导出
  typedef struct tagPIXELFORMATDESCRIPTOR
  { // pfd   
   WORD   nSize;        //结构大小：   sizeof(PIXELFORMATDISCRIPTOR)
   WORD   nVersion;      //版本：    1    
   DWORD dwFlags;      //像素缓冲区属性
   BYTE   iPixelType;      //像素格式 
   BYTE   cColorBits;    //颜色缓冲区中位平面的数量
   BYTE   cRedBits;     //用多少位表示红色
   BYTE   cRedShift;     //红色位的移位计数
   BYTE   cGreenBits; 
   BYTE   cGreenShift; 
   BYTE   cBlueBits; 
   BYTE   cBlueShift; 
   BYTE   cAlphaBits; 
   BYTE   cAlphaShift; 
   BYTE   cAccumBits;      //积累缓冲区位数 
   BYTE   cAccumRedBits;   //积累缓冲区中红色的位数
   BYTE   cAccumGreenBits; 
   BYTE   cAccumBlueBits; 
   BYTE   cAccumAlphaBits; 
   BYTE   cDepthBits;    //深度缓冲区位数
   BYTE   cStencilBits;   //模板缓冲区位数
   BYTE   cAuxBuffers;    //多少个辅助缓冲区
   BYTE   iLayerType;    //过时或忽略
   BYTE   bReserved;     //上层或下层平面的数量
   DWORD dwLayerMask;    //过时或忽略
   DWORD dwVisibleMask;   //下平面的透明颜色
   DWORD dwDamageMask;   //过时或忽略
 } PIXELFORMATDESCRIPTOR; 

 其中dwFlags指定了缓冲区的属性：
   PFD_DRAW_TO_WINDOW  缓冲区可以绘制到一个窗口或者设备的表面
   PFD_DRAW_TO_BITMAP  缓冲区支持在内存中绘图
   PFD_SUPPORT_GDI     缓冲区支持windows的GDI绘制
   PFD_SUPPORT_OPENGL  缓冲区支持OpenGL的绘制
   PFD_GENERIC_ACCELERATED  像素格式被设备驱动程序所加速
   PFD_GENERIC_FORMAT  像素格式仅在软件中被支持(若程序慢 检查这个标志)
   PFD_NEED_PALETTE   指定缓冲区需要指定逻辑调色板
   PFD_NEED_SYSTEM_PALETTE  指向缓冲区需要将系统的调色板作为自身调色板的一部分，即 需要硬件调色板
   PFD_DOUBLEBUFFER  支持双缓冲 与PFD_SUPPORT_GDI互斥
   PFD_STEREO   指名左、右缓冲区是否安装立体图像来组织
   PFD_SWAP_LAYER_BUFFERS  标记设备是否能对各像素格式层面进行独立的交换，包括双缓存的覆盖层和底层面，否则所有层面将作为一组一起交换？
   PFD_DEPTH_DONTCARE  被侵权的像素格式可以有也可没有深度缓冲区，选择一个没有深度缓冲区的像素格式，必须指定该标志
   PFD_DOUBLEBUFFER_DONTCARE  像素格式可以是单一的也是双缓冲区
   PFD_STEREO_DONTCARE  请求的像素格式可以是单像素或立体像素？

   iPixelType:颜色缓冲区类型 PFD_TYPE_RGBA PFD_TYPE_COLORINDEX(已废弃)
   cColorBits:颜色宽度 8 16 24 32(可存alpha值)
   cDepthBits:深度宽度0 16 24 32 越大越精确 计算代价也越大
   cAuxBuffers:辅助缓冲区可用于渲染时从颜色缓冲区重定向到一个辅助缓冲区 再交换到屏幕
```
必须选择一个opengl导出的一种有效格式。可以使用DescribePixelFormat来获取
当前OpenGL支持的所有像素格式，且索引从1开始
```
  PIXELFORMATDESCRIPTOR pfd;
  int nFormatCount;
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  nFormatCount = DescribePixelFormat(hDc, 1, 0, NULL);
  for (int i = 1; i <= nFormatCount; i++)
  {
	DescribePixelFormat(hDc, i, pfd.nSize, &pfd);
	...
  }
  枚举所有像素格式，并检查符合需求的像素格式。

  windows提供了一个快捷函数：ChoosePixelFormat
  允许创建一种像素格式，包含绘制3D图形的窗口所需要的属性。
  若硬件或软件不完全支持所需要的像素格式，会寻找一个最为接近的匹配，
  并返回最适当的索引值。 再调用SetPixelFormat设置这种像素格式。
```


### OpenGL渲染环境句柄 
* 创建一个与目标窗口相兼容的渲染环境
```
	HGLRC hRC = wglCreateContext(hDC)
	执行任何OpenGL命令前，需要指定输出窗口。
	与主目标窗口兼容，可以创建多个，但同时只有一个处于活动状态。

	只要两个窗口像素格式相同，就可以将一个渲染环境移动到另一个渲染环境中。
	关联渲染环境和某个窗口：
	wglMakeCurrent(hDC, hRC)
```



---
# 第三章 OpenGL变换

## 数据类型和函数后缀
```
GLbyte           signed char     b  8
GLshort		     short           s  16
GLint GLsizei     int/long       i  32
GLfloat GLclampf   float         f  32
GLdouble GLclampd  double        d  64
GLubyte GLboolean  unsigned char  ub  8
GLushort          unsigned short   us  16
GLunit GLenum GLbitfield    unsigned int/unsigned long   ui 32
```

## 函数命名格式
<库前缀><命令><参数个数><参数类型>

带v的表示向量版本: glColor3f  glColor3fv

## OpenGL状态机
glEnable glDisable 打开和关闭某个状态
glPushAttrib glPushClientAttrib  glPopAttrib glPopClientAttrib
用于临时修改某个属性  做临时缓存



## OpenGL变换

### OpenGL的数据类型
为了跨平台，定义了自己的数据类型，针对不同的编辑器和环境，
映射为C变量类型。
```
	GL开头
	后缀  位数  c类型      opengl类型
	b		8	singed char		GLbyte
	s		16  short			GLshort
	i       32  int/long		GLint GLsizei
	f		32  float			GLfloat GLclampf
	d		64  double			GLdouble GLclampd
	ud      8   unsigned char	GLubyte GLboolean
	us		16  unsigned short  GLushort
	ui      32  unsigned int/long GLuint GLenum GLbitfield

	指针：GLvoid
```
常量以GL_开头 比如：GL_COLOR_BUFFER_BIT


### OpenGL函数的命名
库前缀+命令+参数个数+参数类型
```
	glColor3f(1.0, 1.0, 1.0)
	GLfloat color[] = {1.0, 1.0, 1.0}
	glColor3fv(color)
```

### OpenGL状态机
```
内部大多数函数都是用于修改某种状态的，
glEnable glDisable 控制某个状态
有默认值 可以在任何时候查询某个状态变量
若要临时修改 可用栈来简化
	glPushAttrib glPushclientAttrib
	glPopAttrib glPopClientAttrib
```



### 坐标变换
世界坐标系中的三维物体  --> 三维几何变换  --> 投影
--> 三维裁剪  --> 视口变换  --> 屏幕坐标系中的图形


某个点(x,y,z,w) --(模型视图矩阵)-> 视点坐标  --(投影矩阵)-> 裁剪坐标
--(投影除法)->归一化设备坐标  --(视口变换)-> 窗口坐标

### 投影除法
将坐标值除以w，得到归一化设备坐标

### 窗口坐标
可以操作视口，放大 缩小 拉伸图像

### 变换顺序
视点变换在模型变换前指定？
视口变换和投影变换可以在绘图前任意时刻指定


* 视点变换
```
  设置视点的位置和方向，默认在坐标系原点，初始方向指向Z负轴。
  同物体模型的默认位置一致。
  void gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez,
				GLdouble centerx, GLdouble centery, GLdouble centerz,
				GLdouble upx, GLdouble upy, GLdouble upz)

  物体的某个顶点，先通过模型变换，得到这个点在世界坐标的位置，
	再进行视点变换  得到在观察者眼中的坐标  可以组合为模型视图矩阵
```

* 模型变换
```
  默认模型在世界的原点 可通过gl命令 进行模型转换
  平移：
	glTranslate(x, y, z)
  旋转：
	glRotate(angle, x, y, z) 绕原点到(x,y,z)为轴 逆时针旋转
  缩放：
	glScale(x, y, z) 默认1.0
```

* 投影变换
```
  将三维物体降维，投影到二维平面。
  类型：
  * 透视投影(perspective projection)
    近大远小 符合人类眼睛的真实情况
	void glFrustum(GLdouble left, right, bottom, top, near, far)
	  指定了近平面的左下(left, bottom, -near)和右上(right, top, -near)两个点 
	  远平面指定了距离 自动计算远平面坐标
	  near far：指定距离视点的距离 都是正值

	or
	void gluPerspective(GLdouble fovy, aspect, zNear, zFar)
	  创建一个对称透视视景体
	  fovy:x-z平面的角度 [0.0, 180.0]
	  aspect:宽高比
	  zNear zFar:到视点的距离 总为正
	  
	都基于视点在原点 指向z负轴

  * 正视投影(orthographic projection) or 平行投影
    保持大小不变 和远近无关  常用于3d绘图软件 
	void glortho(GLdouble left, right, bottom, top, near, far)
	or
	void gluOrtho2D(GLdouble left, right, bottom, top)
	用于二维图像到二维屏幕的投影
	特殊的正视投影 near far默认为-1.0 1.0  所有物体z坐标都是0.0

  得到一个矩阵 右乘模型矩阵 得到投影位置
```

* 视口变换
```
  将投影后的物体显示在屏幕窗口(通常称为 视口)上
  经过了几何变换、投影变换、裁剪变换 要转为屏幕坐标
  glViewport(GLint x, y, GLsizei width, height)
    定义一个视口
    x y:窗口坐标系左下角坐标  
	单位：像素 都是整数
	通常值为 (0, 0, winWidth, winHeight)
```

* 裁剪变换
```
  除视景体外，可以自定义裁剪平面来去除场景中无关的目标
  void glClipPlane(GLenum plane, Const GLdouble *equation)
	plane:裁剪面号  GL_CLIP_PLANEi(i=0,1,...)
    equation:4个系数值的数值 对应Ax + By + cZ + D = 0
	        来确定一个平面 
  需要先开启 glEnable(GL_CLIP_FLANEi)
  怎么个切发？ 应该平面有方向 某个方向后的内容被裁剪掉
```


* 矩阵堆栈
```
  保存当前矩阵 执行新的变换后 恢复原来的矩阵
  4种类型:
    模型视图矩阵堆栈
	投影矩阵堆栈
	颜色矩阵堆栈：修改颜色
	纹理矩阵堆栈：存储纹理坐标信息

  glMatrixMode(GLenum mode)
    mode:GL_MODELVIEW GL_PROJECTION GL_TEXTURE
	指定当前修改哪个堆栈

  glPushMatrix()
    复制栈顶矩阵 下压一级 上面两个相同  不能压入过多 会报错

  glPopMatrix()
    栈内矩阵数要>1 否则报错
```



# 第四章 OpenGL光照、材质和纹理
* 为了模仿现实中的光照效果，opengl中采用光照模型和物体材质属性来实现。
* 为了提高真实感 大量使用纹理映射(texture mapping)技术 
  不但降低了多边形数 而且有更多的微观细节 比如：一面墙


## 真实感图形渲染步骤
1. 建模，用数学方法建立三维场景的几何描述,影响图形的复杂性和绘制的计算量 
2. 三维模型经过变换，转为二维平面透视投影图
3. 通过隐藏面消除算法，将视线外或被遮挡的不可见面消去
4. 计算可见面的颜色，基于光学模型计算可见面投射到观察者(位置)眼中的亮度大小和颜色分量，生成顶点颜色和每个像素的颜色


## OpenGL光照模型
* 假定光可以分为红、绿、蓝3种分量。
* 光源可以使用发射的红、绿、蓝光的量来描述
* 对于物体表面材质 可以使用对各个方向反射的红、绿、蓝光的比例来描述

```
  光照模型的组成：
  1. 环境光 ambient light
    光从光源发出后经过环境多次散射，无明确方向的光。
	室内成分占大头，室外很少？
  2. 漫射光 diffuse light 
    来自一个方向，垂直物体时弊倾斜时更明亮。
	在各个方向上均匀发散出去，不同角度看到的亮度一致。
  3. 镜面反射光 specular light
    特定方向的光被反射到特定方向。
	对光滑的表面反射大部分，对粗糙表面反射很少
  4. 发射光 emitted light
    材质的发射光 模拟物体自发光

  分别计算后叠加一起

  计算步骤：
  1. 为物体所有顶点定义法线向量
  2. 创建光源、设置其位置并启用光照和对应光源
  3. 创建和选择光照模型
  4. 设置物体的材质属性
```


### 法线向量
* 垂直于表面的向量。
* 对于一个平面，所有顶点的法线一样；但在曲面中，可能不同
* 可以为多边形指定法线，可以为每个顶点指定法线，但不再在顶点外其他地方指定法线
* 法线决定了物体相对于光源的朝向，从而确定有多少光源照射到该顶点上

```
  设置法线：
    void glNormal3{b s i d f}[v](TYPE nx, ny, nz)
	之后调用glVertex*() 则将法线赋值给指定的顶点
	法线之表明方向 无关大小 但计算光照前 需要转为1 即：归一化向量
	模型平移、旋转不会影响向量 但缩放会改变长度 
	opengl可自动将法线归一化 glEnable(GL_NORMALIZE)
	会影响性能 默认是关闭的
```


### 创建光源
* 光源的特性：颜色、位置、方向
```
  void glLight{i f}[v](GLenum light, GLenum pname, TYPE param)
    light:光源号 GL_LIGHT0 1 .. 7
	pname:特性
	param：特性值

  特性：
     pname         默认值            描述
    GL_AMBIENT  (0.0,0.0,0.0,1.0)  环境光的颜色
	GL_DIFFUSE  (1.0,1.0,1.0,1.0) or (0.0,0.0,0.0,1.0) 漫发射光颜色
	GL_SPECULAR    同上            镜面反射光颜色
	GL_POSITION (0,0,1,0)          光源位置齐次坐标(x,y,z,w)
	GL_SPOT_DIRECTION (0,0,-1)     聚光方向
	GL_SPOT_EXPONENT  0.0          聚光指数
	GL_SPOT_CUTOFF  180.0          聚光截止角
	GL_CONSTANT_ATTENUATION 1.0    常数衰减因子
	GL_LINER_ATTENUATION  0.0      线性衰减因子
	GL_QUADRATIC_ATTENUATION 0.0   平方衰减因子

	对于GL_LIGHT0,GL_DIFFUSE和GL_SPECULAR默认值(1,1,1,1) 其他光源默认(0,0,0,1)

	例子：
	GLfloat lightAmbient[] = {0.5, 0.5, 0.5, 1.0}
	glLightfv(GL_LIGHT0, AL_AMBIENT, lightAmbient)
```

* 特殊光源
```
  1. 定向光源和定位光源
    放无限远处：平行光  定向光源
	场景附近：决定光线如何照射到物体 定位光源
	使用GL_POSITON  (x,y,z,w)
	  w:0 定向光源   非0:定位光源

  2. 衰减光源
    衰减系数 = 1 / (kc + kl + kq * d*d)
	d:光源与顶点的距离
	kc:GL_CONSTANT_ATTENUATION
	kl:GL_LINER_ATTENUATION
	kq:GL_QUADRATIC_ATTENUATION
	默认都是0  可以修改：
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0)

  3. 聚光灯
    将定位光源限定在圆锥体内，成为一个聚光灯
	GL_SPOT_CUTOFF 设置轴线与母线的夹角  即：顶角的一半
	  默认180.0  可选：[0.0, 90.0]

	GL_SPOT_DIRECTION设置聚光灯方向
	GLfloat dir[] = {0.0, 0.0, -1.0}
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir)

	GL_SPOT_EXPONENT 聚光指数 控制光的集中程度  中心最强 外缘渐弱
	  默认0 即：均匀照射
	glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0)
```

* 启用光照
```
  默认不开启，不会进行法线、光源、材质等复杂计算，缺少真实感。
  glEnable(GL_LIGHTING)  总开关
  glEnable(GL_LIGHT0)  开启某个灯
```

### 选择光照模型
共有3种光照模型

* 全局环境光
```
  除了光源提供环境光外，有一个默认的环境光，不来自任何光源，
  称之全局环境光。这就是不加光源，也能绘制物体的原因。
  float ambient[] = {0.2, 0.2, 0.2, 1.0}
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient)
```

* 局部视点和无穷远点
```
  视点位置能影响镜面反射中高光的计算。
  顶点的高光强度取决于顶点法向量、顶点到光源的方向，
  以及顶点到视点的方向。
  无穷远视点：到所有顶点的方向一致
  局部视点：到每个顶点的方向都不同，性能降低很多，但质量高了

  默认：无穷远视点
  //设置为局部视点
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE)
```

* 双面光照
```
  一般只对物体正面进行光照计算，忽略背面。
  若要查看物体内侧情况，需要用到双面光照。
  //启用双面光照
  glLightModeli(GIGHT_MODEL_TWO_SIDE, GL_TRUE)
```



## 材质
现实中，不同颜色的物体在不同颜色的光照下，效果差异很大。
即使同颜色物体，也会有差异，取决于材质的反射和透射属性。

* 材质RGB与光源RGB的关系
```
  对于光源来说，RGB就是相对最大强度的百分比。
  白光(1.0,1.0,1.0) 黑(0,0,0)  其他颜色就是各分量的占比

  对于材质来说，表现为对光的反射率。
  比如(1.0, 0.5, 0.0) 反射全部红光 绿光一半 蓝光不反射

  最终到达眼睛的光的颜色 为两者颜色相乘的结果
  若有多束光 则将结果相加 超过1.0当做1.0处理
```

* 材质的定义
```
  void glMaterial{if}[v](GLenum face, GLenum pname, TYPE param)
    face: GL_FRONT 正面  GL_BACK 背面  GL_FRONT_AND_BACK 双面
	pname: 哪种材质属性
	param: 属性值

  材质属性：
    pname        默认值                 描述
  GL_AMBIENT    (0.2, 0.2, 1.0)       材质的环境颜色
  GL_DIFFUSE    (0.8, 0.8, 0.8, 1.0)  材质的散射颜色
  GL_AMBIENT_AND_DIFFUSE              材质的环境颜色和散射颜色
  GL_SPECULAR   (0.0, 0.0, 0.0, 1.0)  材质的镜面反射颜色
  GL_SHININESS   0.0                  镜面反射指数
  GL_EMISSION   (0.0, 0.0, 0.0, 1.0)  材质的发射光颜色
  GL_COLOR_INDEXES (0,1,1)            环境、散射和镜面颜色索引

  现实中物体的散射颜色和环境颜色通常相同，
  可以用GL_AMBIENT_AND_DIFFUSE 将两者设置相同

  镜面反射将导致物体出现高亮区域，反射强度取决于视点位置，
  反射角方向上最亮。
  GL_SHININESS控制高亮区域大小和亮度  [0.0, 128.0]  
  越大：范围越小 亮度越大
  GL_EMISSION 可以设置为灯的颜色 表示灯本身
```

* 颜色材质模式
```
  void glColorMaterial(GLenum face, GLenum mode)
    face:同上
	mode: GL_AMBIENT GL_DIFFUSE GL_SPECULAR GL_AMBIENT_AND_DIFFUSE GL_EMISSION
	将表面face的材质属性mode 设置为当前颜色 由glColor*()指定

	glColorMaterial(GL_FRONT, GL_DIFFUSE)  //使环境材质颜色追踪当前色
	glEnable(GL_COLOR_MATERIAL)  // 启用材质颜色追踪当前颜色

	使用场合：
	    需要改变场景中单个材质
		不需要时 记得关闭 避免开销

    若需要改变多个材质参数 还是通过glMaterial*更合适
```

* 不同材质球的表现
	![img](https://github.com/sywoon/zero_step_opengl1.1/blob/master/doc/07_Material.png)
```
  1 仅有漫反射光而无环境光和镜面光
      比较暗 边缘渐黑
  2 有漫反射光和镜面光，并有低高光，而无环境光
      中间大块高亮  边缘黑的更厉害
  3 有漫反射光和镜面光，并有很亮的高光，而无环境光
      小块高亮  明暗比1更突出
  4 有漫反射光和辐射光，而无环境和镜面反射光
      偏向自发光颜色  明暗比更像3
  5 有漫反射光和环境光，而无镜面反射光
      比1更亮些 
  6 有漫反射光、环境光和镜面光，且有低高光
      比2更亮
  7 有漫反射光、环境光和镜面光，且有很亮的高光
      比3更亮
  8 有漫反射光、环境光和辐射光，而无镜面光
      比4更亮
  9 有漫反射光和有颜色的环境光，而无镜面光
      同5 颜色略有不同
  10 有漫反射光和有颜色的环境光以及镜面光，且有低高光
      同6 颜色略有不同
  11 有漫反射光和有颜色的环境光以及镜面光，且有很亮的高光
      同7 颜色略有不同
  12 有漫反射光和有颜色的环境光以及辐射光，而无镜面光
      同8 颜色略有不同
```



















