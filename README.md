# 零起步之opengl1.1
学习<<OpenGL游戏编程>>过程中的记录和心得。

## 编译工具
 vs2015 [clang8.0.0 32位](http://releases.llvm.org/download.html#8.0.0) [winsdk1809 10.0.17763.0](https://developer.microsoft.com/en-us/windows/downloads/sdk-archive)


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
    2 修改Log源码  并统一输出log文件格式为ansi
        bool bFromUtf8 = false;
        char* pszAnsi = NULL;

    #ifdef COMPILE_CL
        bFromUtf8 = false;
    #else
        if (utf82ansi(pszBuf, &pszAnsi))
        {
            bFromUtf8 = true;
        }
    #endif
```




















