﻿@echo off

cls
call clenv.bat
call winkitenv.bat

:: 1 vs cl   2 clang
set USE_CL=1


cd src

if %USE_CL% == 1 (
	cl *.cpp /source-charset:utf-8 /D COMPILE_CL /D _USING_V110_SDK71_ /I "..\..\opengl1.1\include" /link /LIBPATH:"..\..\opengl1.1\lib" /out:..\main_cl.exe user32.lib gdi32.lib
	del *.obj
)

if %USE_CL% == 2 (
	clang -D _CRT_SECURE_NO_WARNINGS -I "..\..\opengl1.1\include" -L "..\..\opengl1.1\lib" -l user32.lib -l gdi32.lib -o ..\main.exe *.cpp
)



pause
