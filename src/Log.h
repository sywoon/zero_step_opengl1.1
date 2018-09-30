#pragma once 

void Log(char* pszFmt, ...);

void InitLogFile(const char* pszPath = NULL, const char* pszFile = NULL);
void SetAutoReturn(const bool value);
void ClearLog();

bool MkDir(const char* pszPath);
bool DirExist(const char* pszPath);
