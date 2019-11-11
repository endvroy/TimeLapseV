#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>

class ScreenCapturer {
private:
	int windowWidth;
	int windowHeight;
	HDC hScreen;
	HDC hDC;
	HBITMAP hBitmap;
public:
	ScreenCapturer();
	~ScreenCapturer();
	void screenshot(char path[]);
};
