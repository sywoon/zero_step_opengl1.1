@echo off

set "path=C:\Program Files (x86)\MSBuild\14.0\Bin;%path%"

cd proj.win32
msbuild GLEngine.sln
cd ..

move /y %~dp0\build\Debug\GLEngine.exe %~dp0

pause
