#pragma once

#include <windows.h>

class BitmapSaver
{
public:
	BitmapSaver(int _height,int _width);

	void saveBitmap(float **image,LPCWSTR name,int levels);
private:
	char **file;
	int height;
	int width;
	LPCWSTR name;
};

