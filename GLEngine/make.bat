@echo off

set "path=C:\Program Files (x86)\MSBuild\14.0\Bin;%path%"

cd build
msbuild GLEngine.sln
cd ..



pause
