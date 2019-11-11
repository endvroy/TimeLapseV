#include "ScreenCapturer.h"
#include <wchar.h>
#include <atlstr.h>
#include <atlimage.h>
#include <gdiplus.h>

ScreenCapturer::ScreenCapturer() {
	//GRAPHICS::_GET_SCREEN_ACTIVE_RESOLUTION(&windowWidth, &windowHeight);
	windowWidth = 1920;
	windowHeight = 1080;
	// get screen hdc
	hScreen = GetDC(NULL);
	// create hdc for bitmap
	hDC = CreateCompatibleDC(hScreen);
	// create bitmap
	hBitmap = CreateCompatibleBitmap(hScreen, windowWidth, windowHeight);
	// replace obj in hDC with the bitmap
	SelectObject(hDC, hBitmap);
}

void ScreenCapturer::screenshot(char path[])
{
	// copy screen to bitmap
	BOOL    bRet = BitBlt(hDC, 0, 0, windowWidth, windowHeight, hScreen, 0, 0, SRCCOPY);

	IStream* stream = NULL;
	HRESULT hr = CreateStreamOnHGlobal(0, TRUE, &stream);
	CImage image;
	ULARGE_INTEGER liSize;

	// save screenshot to png
	image.Attach(hBitmap);
	image.Save(path, Gdiplus::ImageFormatJPEG);
}

ScreenCapturer::~ScreenCapturer() {
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	DeleteObject(hBitmap);
}
