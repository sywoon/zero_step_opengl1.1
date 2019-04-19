@echo off

set "path=..\opengl1.1\bin;%path%"
call clenv.bat


:: 1 vs cl   2 clang
set USE_CL=2  

::vs cl use utf8 : 文件 -> 高级保存选项 -> UTF-8无签名（代码页65001）

cd src

if %USE_CL% == 1 (
	cl *.cpp /source-charset:utf-8 /D _USING_V110_SDK71_ /I "..\..\opengl1.1\include" /link /LIBPATH:"..\..\opengl1.1\lib" /out:..\main.exe user32.lib gdi32.lib
	del *.obj
)

if %USE_CL% == 2 (
	clang -D _CRT_SECURE_NO_WARNINGS -I "..\..\opengl1.1\include" -L "..\..\opengl1.1\lib" -l user32.lib -l gdi32.lib -o ..\main_clang.exe *.cpp
)

pause
