#pragma once


#define Log mylog::WriteLog

namespace mylog 
{
	void WriteLog 		(const char* pszFmt, ...);
	void ClearLog		();
	void InitLogFile 	(const char* pszPath = NULL, const char* pszFile = NULL);
	void SetAutoReturn 	(const bool value);
}

