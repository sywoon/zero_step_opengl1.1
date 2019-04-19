@echo off

set "path=..\..\opengl1.1\bin;%path%"

call clenv.bat

::export .res
rc /d _USING_V110_SDK71_ GLFrame.rc

cl *.cpp GLFrame.res /D _USING_V110_SDK71_ /I "..\opengl1.1\include" /link /LIBPATH:"..\opengl1.1\lib" /out:main.exe user32.lib gdi32.lib

del *.obj
::clang -l user32.lib -l gdi32.lib -o main.exe *.c
pause
