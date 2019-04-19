#include <stdio.h>
#include <windows.h>
#include <malloc.h>
#include <sys/stat.h>


#ifdef WIN32
#	include <time.h>
#	include <direct.h>
#else
#	include <sys/time.h>
#	include <dirent.h>
#endif

#include "Log.h"

static char _pszFile[512] = "output.log";
static bool _autoRtn = true;  //�Զ�����


HANDLE _CreateMyFile(const char* pszFile)
{
	HANDLE hFile = ::CreateFile(pszFile, 
							GENERIC_WRITE | GENERIC_READ,
							0,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("create file:%s failed!\n", pszFile);
		return NULL;
	}
	return hFile;
}

void Log(char* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);

    //vc6 not support
    const int extLen = _autoRtn ? 2 : 1;  //�������� \n + \0
	int len = vsnprintf(0, 0, pszFmt, args) + extLen;
	char* pszBuf = (char*)_alloca(len * sizeof(char));//ջ�з���, ����Ҫ�ͷ�

	vsnprintf(pszBuf, len, pszFmt, args);
	va_end(args);

	pszBuf[len-1] = '\0';
	if (_autoRtn)
	{
		pszBuf[len-2] = '\n';
	}

	HANDLE hFile = _CreateMyFile(_pszFile);
	if (NULL == hFile)
		return;

	SetFilePointer(hFile, 0, 0, FILE_END);

	DWORD dwWriteSize = 0;
	BOOL bRet = ::WriteFile(hFile, pszBuf, strlen(pszBuf),
					&dwWriteSize, NULL);
	if (bRet)
	{
		FlushFileBuffers(hFile);
		printf(pszBuf);
		if (_autoRtn)
		{
			printf("\n");
		}
	}

	CloseHandle(hFile);
}

void InitLogFile(const char* pszPath, const char* pszFile)
{
	if (NULL == pszPath)  //û·�����ļ��� Ĭ�ϸ�Ŀ¼��
	{
		time_t ltime;
		time(&ltime);
		
		tm* curTime = localtime(&ltime);
		sprintf(_pszFile, "%u_%u_%u.log",
			curTime->tm_year + 1900, curTime->tm_mon + 1, curTime->tm_mday);
		return;
	}

	if (!DirExist(pszPath) && !MkDir(pszPath))
	{
		printf("mkdir failed:%s\n", pszPath);
		return;
	}

	ZeroMemory(_pszFile, 512);
	if (pszFile)
	{
		sprintf(_pszFile, "%s/%s", pszPath, pszFile);
		return;
	}

	time_t ltime;
	time(&ltime);
	
	tm* curTime = localtime(&ltime);
	sprintf(_pszFile, "%s/%u_%u_%u.log", pszPath,
		curTime->tm_year + 1900, curTime->tm_mon + 1, curTime->tm_mday);
}

void SetAutoReturn(const bool value)
{
	_autoRtn = value;
}

void ClearLog()
{
	HANDLE hFile = _CreateMyFile(_pszFile);
	if (NULL == hFile)
		return;

	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	SetEndOfFile(hFile);
	CloseHandle(hFile);
}


bool MkDir(const char* pszPath)
{
#ifdef WIN32
	return 0 == _mkdir(pszPath);
#else
	return 0 == mkdir(pszPath, 0775);
#endif
}

//ĩβ���ܴ�'/'
bool DirExist(const char* pszPath)
{
	struct stat s;
	int ret = stat(pszPath, &s);
	if( 0 != stat(pszPath, &s) ) 
		return false;

	return (s.st_mode & S_IFDIR) != 0;
}