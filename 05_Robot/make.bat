@echo off

set "path=..\opengl1.1\bin;%path%"

call clenv.bat

::vs cl use utf8 : 文件 -> 高级保存选项 -> UTF-8无签名（代码页65001）

cd src
cl *.cpp /source-charset:utf-8 /D _USING_V110_SDK71_ /I "..\..\opengl1.1\include" /link /LIBPATH:"..\..\opengl1.1\lib" /out:..\main.exe user32.lib gdi32.lib

del *.obj
::clang -l user32.lib -l gdi32.lib -o ..\main.exe *.c

pause
