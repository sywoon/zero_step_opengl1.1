#include "File.h"

#include <sys/stat.h>
#ifdef WIN32
#	include <direct.h>
#else
#	include <dirent.h>
#endif


namespace file
{

	bool MkDir(const char* pszPath)
	{
#ifdef WIN32
		return 0 == _mkdir(pszPath);
#else
		return 0 == mkdir(pszPath, 0775);
#endif
	}

	//末尾不能带'/'
	bool DirExist(const char* pszPath)
	{
		struct stat s;
		int ret = stat(pszPath, &s);
		if (0 != stat(pszPath, &s))
			return false;

		return (s.st_mode & S_IFDIR) != 0;
	}

}