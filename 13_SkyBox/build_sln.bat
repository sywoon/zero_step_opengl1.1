@echo off
set "path=C:\Program Files (x86)\MSBuild\14.0\Bin;%path%"

::call clenv.bat
::devenv "%~dp0\proj.win32\GLEngine1.1.sln" /build "Debug|Win32"
::vcbuild "%~dp0\proj.win32\GLEngine1.1.sln" "Debug|Win32"



MSBuild "%~dp0/proj.win32/GLEngine1.1.vcxproj" -property:Configuration=Debug


rd /s /q ".\proj.win32\.vs"
rd /s /q ".\proj.win32\Debug"
del ".\proj.win32\GLEngine1.1.VC.db"
del GLEngine1.1.ilk

pause