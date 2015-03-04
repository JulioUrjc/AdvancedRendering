#pragma once

#include <windows.h>
#include "GL\glew.h"
#include "GL\gl.h"

class ShaderLoader{
  private:
	//File atributes
	char *fileName;
	char *sourceCode;
	unsigned int fileLength;

	//Shader type
	GLenum shaderType;
	//Shader ID
	GLint shaderID;


  public:
	ShaderLoader(char *fileName,GLenum shaderType);
	~ShaderLoader(void);

	//Read shader source code
	void loadShaderFile();

	//Compile shader source code
	void compileShader();

	//Get identificator
	GLuint getShaderID();

	//Delete and free memory
	void deleteShader();

	char * getSource(){ return sourceCode; }
};

