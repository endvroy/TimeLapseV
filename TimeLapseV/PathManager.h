#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define BASE_PATH_LEN 200
#define PATH_BUF_LEN 200

class PathManager {
private:
	char basePath[BASE_PATH_LEN];
public:
	char pathBuf[PATH_BUF_LEN];
	int locIndex;
	int hour;
	int minute;

	PathManager() :locIndex(0), hour(0), minute(0) {};
	void setBasePath(const char path[]);
	void makeBaseDir();
	void getLocDirPath();
	void getMetadataPath();
	void makeLocDir();
	void getFilePath();
};