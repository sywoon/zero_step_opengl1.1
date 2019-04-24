@echo off

set "path=..\opengl1.1\bin;%path%"
call clenv.bat


:: 1 vs cl   2 clang
set USE_CL=1

set "INCLUDE_PATH=..\..\opengl1.1\include;include;"
set "LIB_PATH=..\..\opengl1.1\lib"

cd src
if %USE_CL% == 1 (
	cl *.cpp /source-charset:utf-8  /wd"4819" /wd"4996" /wd"4018" /D COMPILE_CL /D _USING_V110_SDK71_ /D WIN32 /I "..\..\opengl1.1\include" /I "include" /I "framework" /link /LIBPATH:"%LIB_PATH%" /out:..\main_cl.exe user32.lib gdi32.lib
	::del *.obj
)

if %USE_CL% == 2 (
	clang -Xclang -flto-visibility-public-std -D _CRT_SECURE_NO_WARNINGS -I "..\..\opengl1.1\include" -I "include" -I "framework" -L "%LIB_PATH%" -l user32.lib -l gdi32.lib -o ..\main.exe *.cpp
)

pause
