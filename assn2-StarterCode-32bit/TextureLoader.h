#pragma once
class TextureLoader
{
private:
	char *fileName;
	int height;
	int width;
	char *texture;
	unsigned char *texture2;
public:
	TextureLoader(char *fileName);

	void loadTextureFromFile();
	void loadTextureFromFile2();

	//Getters
	int getWidth();
	int getHeight();
	char *getTexture();
	unsigned char *getTexture2();
	void freeTexture(unsigned char* image);
	//~TextureLoader(void);
};



