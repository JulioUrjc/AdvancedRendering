#pragma once

#include "ShaderLoader.h"
#include "Camara.h"
#include "BezierCurve.h"

class DrawCurve{
	private:

		BezierCurve* curve;
		glm::vec3 color;
		// Paint Curve
		//VAO
		GLuint vao;
		//VBO
		GLuint buffer[3];
		//Shaders
		ShaderLoader vShader = ShaderLoader("./shaders/curveVShader.glsl", GL_VERTEX_SHADER);
		ShaderLoader fShader = ShaderLoader("./shaders/curveFShader.glsl", GL_FRAGMENT_SHADER);

		GLuint program;

		//Shader atributes
		GLint inColor;
		GLint inVertex;

		//Uniform variables for shaders
		//ModelViewProjection matrix
		GLint mvpMatrixID;

		std::vector<float> vertexVector;
		std::vector<unsigned int> pointVector;
	public:
		/* Init Shaders */
		DrawCurve(BezierCurve* c);
		void draw(Camara* camara, int modo);
		void initValues();
		void initShaders();
		void generateVectors();
		void generateBuffers();
};

