#pragma once
class TextureLoader
{
private:
	char *fileName;
	int height;
	int width;
	char *texture;
public:
	TextureLoader(char *fileName);

	void loadTextureFromFile();

	//Getters
	int getWidth();
	int getHeight();
	char *getTexture();

	~TextureLoader(void);
};

