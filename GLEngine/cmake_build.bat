@echo off


rd /S /Q build
mkdir build

cd build
cmake -G "Visual Studio 14" ..\src
cd ..


pause

