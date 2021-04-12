@echo off


rd /s /q build
mkdir build

cd build
cmake -G "Visual Studio 14" ..\src
cd ..


pause

