@echo off

set "path=..\opengl1.1\bin;%path%"
call clenv.bat


:: 1 vs cl   2 clang
set USE_CL=2

::vs cl use utf8 : �ļ� -> �߼�����ѡ�� -> UTF-8��ǩ��������ҳ65001��

cd src


:: cl�������exe������Ϊ��clang��һ�� 
:: Դ����utf8��ʽ ����̨���ʱתΪansi 
:: ��clang����  cl /source-charset:utf-8 �෴ ����Ҫ����תһ��ansi �õ����ַ����Ͳ���utf8��  ���Ʊ���ʱ�Զ���Դ������ת��
:: ��COMPILE_CL�������������

if %USE_CL% == 1 (
	cl *.cpp /source-charset:utf-8 /D COMPILE_CL /D _USING_V110_SDK71_ /I "..\..\opengl1.1\include" /link /LIBPATH:"..\..\opengl1.1\lib" /out:..\main_cl.exe user32.lib gdi32.lib
	del *.obj
)

if %USE_CL% == 2 (
	clang -D _CRT_SECURE_NO_WARNINGS -I "..\..\opengl1.1\include" -L "..\..\opengl1.1\lib" -l user32.lib -l gdi32.lib -o ..\main.exe *.cpp
)

pause
