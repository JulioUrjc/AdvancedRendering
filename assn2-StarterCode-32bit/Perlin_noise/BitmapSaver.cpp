#include "BitmapSaver.h"

#include <iostream>
#include <fstream>
#include <windows.h>


BitmapSaver::BitmapSaver(int _height,int _width)
{
	height = _height;
	width = _width;
}

void BitmapSaver::saveBitmap(float **image,LPCWSTR name,int levels)
{
    HANDLE file;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER fileInfo;
    RGBTRIPLE *newImage;
    DWORD write = 0;
    newImage = new RGBTRIPLE[height*width];
 
    file = CreateFile(name,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);  //Sets up the new bmp to be written to
 
    fileHeader.bfType = 19778;                                                                    //Sets our type to BM or bmp
    fileHeader.bfSize = sizeof(fileHeader.bfOffBits) + sizeof(RGBTRIPLE);                                                //Sets the size equal to the size of the header struct
    fileHeader.bfReserved1 = 0;                                                                    //sets the reserves to 0
    fileHeader.bfReserved2 = 0;
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);                    //Sets offbits equal to the size of file and info header
 
    fileInfo.biSize = sizeof(BITMAPINFOHEADER);
    fileInfo.biWidth = width;
    fileInfo.biHeight = height;
    fileInfo.biPlanes = 1;
    fileInfo.biBitCount = 24;
    fileInfo.biCompression = BI_RGB;
    fileInfo.biSizeImage = ((((fileInfo.biWidth *
								fileInfo.biBitCount ) + 31 ) & ~31 ) >> 3 ) *
								fileInfo.biHeight;
    fileInfo.biXPelsPerMeter = 2400;
    fileInfo.biYPelsPerMeter = 2400;
    fileInfo.biClrImportant = 0;
    fileInfo.biClrUsed = 0;
 
    WriteFile(file,&fileHeader,sizeof(fileHeader),&write,NULL);
    WriteFile(file,&fileInfo,sizeof(fileInfo),&write,NULL);
 
    int x=0;
	int rgbVal;
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
		{
			if(levels == 0)
				//From -1 - 1 to 0 - 255
				rgbVal=(((image[i][j]+1)/2)*255);
			else
				//From -6 - 6 to 0 - 255
				rgbVal=(((image[i][j]+levels)/(levels*2))*255);
			
			//std::cout<<image[i][j]<<" -> "<<rgbVal<<std::endl;
			newImage[x].rgbtBlue = rgbVal;
			newImage[x].rgbtGreen = 0;
			newImage[x].rgbtRed = rgbVal;

			x++;
		}
 
    WriteFile(file, newImage, fileInfo.biSizeImage, &write, NULL);
 
    CloseHandle(file);
}

