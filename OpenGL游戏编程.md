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

* 矩阵的应用
	![img](https://github.com/sywoon/zero_step_opengl1.1/blob/master/doc/05_Robot.png)



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

* 不同光的表现
	![img](https://github.com/sywoon/zero_step_opengl1.1/blob/master/doc/06_Light.png)


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


## 纹理映射


### 纹理资源的载入

#### BMP
* BMP位图简介
```
  Bitmap-File Windows采用的图像文件存储格式。
  Windows3.0之前 bmp格式与显示设备相关，
  称为设备相关位图(Device-Dependent Bitmap, DDB)
  之后为了与设备无关，改为设备无关位图(Device-Independent Bitmap, DIB)
  可以在任何类型的显示设备上显示。
```

* BMP结构
```
  4部分组成：
    文件头  BITMAPFILEHEADER bmfh
	信息头  BITMAPINFOHEADER bmih
	彩色表  RGBQUAD aColors[]
	图像数据字节阵列  BYTE aBitmapBits[]

  文件头: 文件类型、大小、存放位置
  typedef struct tagBITMAPFILEHEADER {
    UINT bfType;  文件类型
	DWORD bfSize;
	UINT bfReserverd1;  保留 0
	UINT bfReserverd2;  保留 0
	DOWRD bfOffBits;  说明从BITMAPFILEHEADER开始到实际的图像数据之间的字节偏移量
  } BITMAPFILEHEADER;

  typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;  结构大小
	LONG biWidth;  图宽  单位：像素
	LONG biHeight;  图高  单位：像素
    WORD biPlanes;  为目标设备说明位面数 默认1
    WORD biBitCount;  位数/像素  值：1 2 4 24 (?写错了 应该是16 24 32?)
	DWORD biCompression;  压缩类型  
							BI_RGB没压缩 
							BI_RLE8 每像素8位RLE压缩编码 
								压缩格式由2字节组成(重复像素计数和颜色索引)
							BI_RLE4 每像素4位RLE压缩编码 
								压缩格式由2字节组成
	DWORD biSizeImage;  图像大小 单位:字节  当用BI_RGB 可为0
	LONG biXPelsPerMeter;  水平分辨率  像素/米
	LONG biYPelsPerMeter;  垂直分辨率  像素/米
	DWORD biClrUsed;  位图实际使用的彩色表中的颜色索引数
	DWORD biClrImportant;  对图像显示有重要影响的颜色索引的数目 若为0 表示都重要
  } BITMAPINFOHEADER;

  彩色表包含的元素与位图所具有的颜色数相同，按重要性来排序。
  像素的颜色用RGBQUAD结构来定义。
  对于24位真彩色图像就不用彩色表，因为位图中rgb值已经代表了每个像素的颜色。
```

* 数据字节阵列
```
  按扫描行，从下到上存储。即：第一个像素是图片的左下角
  每一行的字节数取决于图像的颜色数目和用像素表示的图像宽度。
```


#### TGA

* TGA位图简介
```
  美国Truevision公司为显卡开发的一种图像文件格式 .tga
  有6种以上格式，用第三个字节来区别。

  值     文件类型
  0      没有图像数据
  1      未压缩 颜色表图像
  2      未压缩 rgb图像
  3      未压缩 黑白图像
  9      Runlenght编码的颜色表图像
  10     runlenght编码的RGB图像
  11     压缩的黑白图像
```

* TGA文件结构
```
  类似bitmap，文件头+信息头+图像数据

  类型为2的结构：
  偏移  长度   描述
   0     1     图像信息字段的字符数 [0,255]  0表示没图像信息字段
   1     1     颜色表类型  0：无  1：有
   2     1     图像类型
   3     5     颜色表规格  若颜色表类型字段为0 忽略， 否则如下：
     3     2     颜色表首地址
	 5     2     颜色表长度
	 7     1     颜色表表项的位数 16 24 32
   8    10     图像规格：
	 8     2      x坐标起始位置
	 10    2      y坐标起始位置
	 12    2      图像宽度 像素
	 14    2      图像高度
	 16    1      每像素存储占用位数  16 24 32
	 17    1      图像描述符字节
   18   可变   图像信息字段
   可变  可变  颜色表数据  若颜色变类型为0  则该项不存在
   可变  可变  图像数据域  存储w×h个像素  每个像素包含RGB颜色值 顺序为BGR(为啥这么排？ bitmap也是这种情况)

```


#### OpenGL纹理映射

* 纹理映射的步骤
```
  OpenGL的纹理映射只能在RGBA模式下使用
  1. 定义纹理
  2. 控制纹理
  3. 设置映射方式
  4. 绘制场景

   渲染前需要启动用纹理映射
	glEnable(GL_TEXTURE_2D);
```

* 纹理的定义
```
  通常定义在一个数组中，表示纹理空间中的一组网格点上的纹理值。
  网格点之间的值，可以通过插值计算来获取，
  再通过纹理空间到物体空间的坐标变换，将纹理映射到物体表面上。

  void glTexImage2D(GLenum target, GLint level, GLint components,
				GLsizei width, GLsizei height, GLsizei border, GLenum format,
				GLenum type, const GLvoid *pixels)
  定义一个二维纹理映射
  target: GL_TEXTURE_2D
  level: 具有多级分辨率的级数 若只有一种则为0
  components: 1-4的整数  指出选择了r g b a中的哪些分量用于调整和混合
		1：r分量  2：r和a  3：r g b  4：r g b a
  width height:纹理图像的长宽 必须是2m+2b m为整数 b为border的值
    若都为0 表示关闭纹理映射
  border:边界宽度 通常为0   啥作用？
  format:GL_COLOR_INDEX GL_RGB GL_RGBA GL_RED GL_GREW GL_BLUE GL_ALPHA
       GL_LUMINANCE GL_LUMINANCE_ALPHA
	   注意：不能用GL_STENCIL_INDEX GL_DEPTH_COMPONENT
  type: 数据类型  GL_BYTE GL_UNSIGNED_BYTE GL_SHORT GL_UNSIGNED_SHORT GL_INT
	  GL_UNSIGNED_INT GL_FLOAT GL_BITMAP
  pixels: 纹理图像数据 包含边界

  void glTexImage1D(...)  定义一维纹理映射 GL_TEXTURE_1D
	2m or 2m+2
```

* 纹理控制
```
  纹理如何对应到屏幕像素
  怎样通过纹理贴图实现纹理说法和纹理重复

  void glTexParameter{if}[v](GLenum target, GLenum pname, TYPE param)
  控制如何将纹理元素映射到片元上(fragment)
  target: GL_TEXTURE_1D GL_TEXTURE_2D

  参数类型            参数值
  GL_TEXTURE_WRAP_S  GL_CLAMP GL_REPEAT
  GL_TEXTURE_WRAP_T  GL_CLAMP GL_REPEAT
  GL_TEXTURE_MAG_FILTER GL_NEAREST GL_LINEAR   放大滤波
  GL_TEXTURE_MIN_FILTER GL_NEAREST GL_LINEAR   缩小滤波
						GL_NEAREST_MIPMAP_NEAREST
						GL_NEAREST_MIPMAP_LINEAR
						GL_LINEAR_MIPMAP_NEAREST
						GL_LINEAR_MIPMAP_LINEAR

  一般纹理为正方形或长方形 但映射到多边形或曲面上 再变换到屏幕坐标时
  不可能和屏幕像素一一对应
  屏幕上的一个像素 可能是纹理的一小部分(放大) or 一大批纹素(缩小)
  opengl采用滤波的方式 来解决这个问题：

  GL_NEAREST：使用坐标离像素中心最近的纹素 可能操作锯齿现象
  GL_LINEAR：使用离像素中心最近的2x2纹素阵列的加权线性平均值
  缩小时 若有多级纹理 可有不同的参数


  纹理的重复和截取：
	纹理坐标指定为[0,1] 之外的值可以截取或重复
	由GL_TEXTURE_WRAP_S _T来控制 横向和纵向处理方式
	GL_CLAMP：大于1用1  小于0用0
	GL_REPEAT：重复整个纹理
```

* 纹理的映射方式
```
  将纹理图像渲染到物体表面的映射方式
  void glTexEnv{if}[v](GLenum target, GLenum pname, TYPE param)
  target                       pname                 param
  GL_TEXTURE_FILTER_CONTROL  GL_TEXTURE_LOD_BIAS    浮点数
  GL_TEXTURE_ENV             GL_TEXTURE_ENV_MODE  GL_DECAL GL_MODULATE GL_BLEND
                             GL_TEXTURE_ENV_COLOR (r,g,b,a)浮点数组
							    只有采用GL_BLEND纹理函数才有用？？？
```

* 指定纹理坐标
```
  物体顶点需要定义几何坐标和纹理坐标
  前者决定了顶点在屏幕上的位置，后者决定了哪个纹素赋予给该顶点
  顶点间的坐标需要插值计算得到。

  纹理坐标有1 2 3 4维形式 用s t r q来表示 以区别于物体坐标(x,y,z,w)
  默认 t r为0  q为1用作齐次坐标

  void glTexCoord{1234}{aifd}[v](TYPE coords)
  设置当前纹理坐标  此后调用glVertex*()所产生的顶点都赋予当前的纹理坐标
  不同函数后缀对应不同的TYPE值 GLshort GLint GLfloat GLdouble
```

* 纹理对象
```
  用于存储纹理数据，可重复使用提供性能  避免每次都glTexImage*D方式载入纹理
  步骤：
  1. 生成纹理对象名称
    使用非0无符号整数作为对象名  为避免重复 需要用gl函数来得到其值
    void glGenTexture(GLsizei n, GLuint *textureNames)
	返回n个未使用的对象名

  2. 创建纹理对象、绑定纹理对象到纹理数据
    void glBindTexture(GLenum target, GLuint textureName)
	首次调用 将创建一个纹理对象 并设置其名称为textureName
	再将其纹理图像数据和纹理属性设置为默认值

  3. 再次绑定纹理对象，将纹理映射到物体表面
	再次调用2 设置该纹理对象为活动状态 然后调用修改纹理图像或属性函数时
	将修改当前的纹理对象内容和属性

  4. 删除纹理对象
    void glDeleteTextures(GLsizei n, const GLuint* textureNames)
	删除n个纹理对象 若对象不存在或名称为0 则被忽略 不会引发错误
```

* 实例 
![纹理贴图](https://github.com/sywoon/zero_step_opengl1.1/blob/master/doc/08_Texture.png)


#### OpenGL多重纹理
  将多幅纹理应用到一个多边形上，然后对这些纹理按一定的方式进行混合。
  在一个处理过程中。

* 多重纹理简介
```
  应用1：光照贴图 
      比如墙面，纹理1显示砖块 纹理2用灰度图制作出聚光灯效果 再融合一起
  
  早期显卡不支持多重纹理  在同一个地方用多个三角形叠加来模拟这种效果 性能很低
  opengl1.1没有实现多重纹理函数 在扩展库中
```

* OpenGL扩展
```
  新出的显卡，为了用新的功能，会通过扩展库来使用。
  不同厂商的扩展，导致命名混乱且同一个功能可能有多个扩展函数 

  扩展名：
  格式 GL_ARB_multitexture
  第一段：
	GL	OpenGL核心扩展
	WGL  针对Windows平台的扩展
	GLX  针对Unix/Linux平台的扩展
	GLU  针对OpenGL Utility Library的扩展

  第二段：谁开发的扩展
    ARB  OpenGL Architecture Review Board 体系评审委员会核准的
	EXT  多个硬件厂商支持的扩展
	NV   nVidia公司
	ATI  ATI公司开发的扩展
	ATIX  ATI公司开发的实验性的扩展
	SGI  Silicon Graphics(SGI)公司开发的扩展
	SGIX  Silicon Graphics(SGI)公司开发的实验性扩展

  第三段：真正的扩展名  功能本身


  如何使用扩展：
	1. 定义函数指针类型  因为windows平台上opengl扩展增加的函数
	   是通过运行时动态获取函数指针来使用的
	   SGI简化了这个步骤 封装到了glext.h中 包括产量和函数指针的定义

	2. 检查显卡是否支持该扩展
	 glGetString(GL_EXTENSIONS)获取支持的全部扩展信息
	 再检查扩展名能否在这里面找到

	3. 若支持可以通过 wglGetProcAddress获取该函数指针
```

* 多重纹理使用步骤
```
  1. 检查显卡是否支持 并获取其函数指针地址
  2. 创建纹理对象 载入纹理数据
  3. 为每一个纹理单元指定其纹理映射参数，并绑定纹理
  4. 对于每个顶点，指定多组纹理坐标

  void glActiveTexture(GLenum texUnit)
  激活一个纹理单元 后续的纹理函数将作用于这个纹理单元上
  texUnit: GL_TEXTUREi   i:[0,k-1]  k:opengl实现支持最大纹理单元数

  glGetIntegerv(GL_MAX_TEXTURE_UINTS) 获取最大支持数

  void glMultiTexCoord{1234}{sifd}[v](GLenum texUint, TYPE coords)
  需要给每个顶点知道多组纹理坐标----每个纹理单元一组
```

* 实例 
![单纹理](https://github.com/sywoon/zero_step_opengl1.1/blob/master/doc/09_MultiTexture01a.png)
![多重纹理](https://github.com/sywoon/zero_step_opengl1.1/blob/master/doc/09_MultiTexture01b.png)






