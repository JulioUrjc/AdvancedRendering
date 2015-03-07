#include "TextureLoader.h"
#include <FreeImage.h>
#include <iostream>


TextureLoader::TextureLoader(char *fileName):fileName(fileName)
{
	loadTextureFromFile();
}

void TextureLoader::loadTextureFromFile()
{
	FreeImage_Initialise(TRUE);

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName,0);
	if(format == FIF_UNKNOWN)
		format = FreeImage_GetFIFFromFilename(fileName);
	if((format == FIF_UNKNOWN)||!FreeImage_FIFSupportsReading(format))
	{
		std::cout<<"Image format unknown in "<<fileName<<std::endl;
		return;
	}
	
	FIBITMAP* img= FreeImage_Load(format, fileName);
	if (img==NULL)
	{
		std::cout<<"Imposible to load "<<fileName<<std::endl;
		return;
	}
	
	FIBITMAP* tempImg = img;
	img = FreeImage_ConvertTo32Bits(img);
	FreeImage_Unload(tempImg);

	width = FreeImage_GetWidth(img);
	height = FreeImage_GetHeight(img);

	//BGRA a RGBA
	texture = new char[4*width*height];
	char *buff = (char*)FreeImage_GetBits(img);

	for(unsigned int j= 0; j<width*height; j++){
		texture[j*4+0]= buff[j*4+2];
		texture[j*4+1]= buff[j*4+1];
		texture[j*4+2]= buff[j*4+0];
		texture[j*4+3]= buff[j*4+3];
	}

	FreeImage_Unload(img);

	FreeImage_DeInitialise();
}

//Getters
int TextureLoader::getWidth()
{
	return width;
}
int TextureLoader::getHeight()
{
	return height;
}
char *TextureLoader::getTexture()
{
	return texture;
}

TextureLoader::~TextureLoader(void)
{
	delete texture;
}
