#include "PathManager.h"
#include <string>

void PathManager::setBasePath(const char path[]) {
	strcpy_s(basePath, BASE_PATH_LEN, path);
}

void PathManager::makeBaseDir() {
	CreateDirectory(basePath, NULL);
}


void PathManager::getLocDirPath() {
	sprintf_s(pathBuf, PATH_BUF_LEN, "%s\\%d", basePath, locIndex);
}

void PathManager::getMetadataPath()
{
	sprintf_s(pathBuf, PATH_BUF_LEN, "%s\\%d\\metadata.json", basePath, locIndex);
}

void PathManager::makeLocDir() {
	getLocDirPath();
	CreateDirectory(pathBuf, NULL);
}

void PathManager::getFilePath() {
	sprintf_s(pathBuf, PATH_BUF_LEN, "%s\\%d\\%d_%d.jpg", basePath, locIndex, hour, minute);
}
