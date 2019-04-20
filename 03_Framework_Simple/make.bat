@echo off

set "path=..\opengl1.1\bin;%path%"
call clenv.bat


:: 1 vs cl   2 clang
set USE_CL=2

::vs cl use utf8 : 文件 -> 高级保存选项 -> UTF-8无签名（代码页65001）

cd src


:: cl编译出的exe编码行为和clang不一致 
:: 源码用utf8格式 控制台输出时转为ansi 
:: 在clang正常  cl /source-charset:utf-8 相反 不需要额外转一次ansi 得到的字符串就不是utf8的  估计编译时自动把源码做了转换
:: 用COMPILE_CL来区别两种情况

if %USE_CL% == 1 (
	cl *.cpp /source-charset:utf-8 /D COMPILE_CL /D _USING_V110_SDK71_ /I "..\..\opengl1.1\include" /link /LIBPATH:"..\..\opengl1.1\lib" /out:..\main_cl.exe user32.lib gdi32.lib
	del *.obj
)

if %USE_CL% == 2 (
	clang -D _CRT_SECURE_NO_WARNINGS -I "..\..\opengl1.1\include" -L "..\..\opengl1.1\lib" -l user32.lib -l gdi32.lib -o ..\main.exe *.cpp
)

pause
