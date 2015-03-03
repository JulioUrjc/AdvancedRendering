#include "ShaderLoader.h"

#include <iostream>
#include <fstream>


//Constructor. Loads shader source from file
ShaderLoader::ShaderLoader(char *fileName,GLenum shaderType):fileName(fileName),shaderType(shaderType){
	loadShaderFile();
}

ShaderLoader::~ShaderLoader(void){
	delete sourceCode;
}

//Read shader source code
void ShaderLoader::loadShaderFile(){
	//Get file
     std::ifstream file;
     file.open(fileName, std::ios::in); 
     if(!file){
		 //Error opening file
		 std::cout<<"Error opening shader file: "<<fileName<<std::endl;
		 getchar();
		 exit(-1);
	 }
     
     //File length
     file.seekg(0,std::ios::end);
     fileLength = file.tellg();
     file.seekg(std::ios::beg);
             
     //Read source file
	 sourceCode=new char[fileLength+1];
	 int i=0;
	 while(file.good()) {
		 sourceCode[i]=file.get();
		 if (!file.eof()) 
			 i++;
		 else 
			 fileLength=i;
	 }
	 sourceCode[fileLength]= '\0';
	 file.close();
}

//Compile shader source code
void ShaderLoader::compileShader(){
	//Create shader from source code
	shaderID = glCreateShader(shaderType);
	glShaderSource (shaderID, 1,(const GLchar **) &sourceCode, (const GLint *)&fileLength);
	glCompileShader(shaderID);
	//Check compilation status
	GLint compiled;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
	if(!compiled){
		//If error get, show and exit
		GLint logLen;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLen);
     
		char *error= new char[logLen];
		glGetShaderInfoLog(shaderID, logLen, NULL,error);

		std::cout << "Error compiling shader " << fileName << ": " << error <<std::endl;
		getchar();
		exit(-1);

		delete error;
         
		//Delete shader
		glDeleteShader(shaderID);
	}
}

GLuint ShaderLoader::getShaderID(){
	return shaderID;
}

//Delete and free memory
void ShaderLoader::deleteShader(){
	glDeleteShader(shaderID);
}


