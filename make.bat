@echo off

cls
call clenv.bat
call winkitenv.bat

cd src
cl *.cpp /D _USING_V110_SDK71_ /I "..\opengl1.1\include" /link /LIBPATH:"..\opengl1.1\lib" /out:%~dp0/main.exe user32.lib gdi32.lib
del *.obj


pause
