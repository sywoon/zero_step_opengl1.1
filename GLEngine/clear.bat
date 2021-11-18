@echo off

pushd proj.win32
rd /s/q Debug
del /f *.sdf
popd

rd /s/q build

del /f *.ilk
del /f *.log



pause













