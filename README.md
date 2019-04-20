# 零起步之opengl1.1
学习<<OpenGL游戏编程>>过程中的记录和心得。

## 编译工具
 vs2015 clang


## utf-8编码问题
vs对utf-8编码支持不是很好，用cl编译时可以带上/source-charset:utf-8来支持
```
  现象：
    采用cl的exe 输出中文时发现默认就是ansi的而非源码格式utf-8
    估计编译时就自动转换了  导致解决方案和clang有冲突
    
  为了将源码采用通用的utf-8编码，不影响Log和控制台中文的显示。
  解决方案：
    1 cl中定义额外的宏来区分用的那个编译器
      cl *.cpp /source-charset:utf-8 /D COMPILE_CL
    2 修改Log源码
      #ifdef COMPILE_CL
		printf("%s", pszBuf);
      #else
		char* pszAnsi = NULL;
		if (utf82ansi(pszBuf, &pszAnsi))
		{
			printf("%s", pszAnsi);
			free(pszAnsi);
		} else {
			printf("%s", pszBuf);
		}
      #endif
      
   缺陷：两个exe不能混用 cl输出的中文log格式是ansi
       而clang输出的log格式是utf-8这才符合
```





