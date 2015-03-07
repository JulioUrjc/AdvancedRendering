#pragma once

#include "GL/glew.h"
#include "Mesh.h"
#include "Poligon.h"
#include "BezierCurve.h"
#include "Camara.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include <vector>
#include <glm\gtx\transform.hpp>

#include <windows.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"

class Vein : public Mesh{
	private:
		int NP;
		GLfloat radiusVein;
		BezierCurve* curve;

		// Paint Vein
		//VAO
		GLuint vao;
		//VBO
		GLuint buffer[4];
		//Shaders
		ShaderLoader vShader= ShaderLoader("./shaders/veinVShader.glsl", GL_VERTEX_SHADER);
		ShaderLoader fShader = ShaderLoader("./shaders/veinFShader.glsl", GL_FRAGMENT_SHADER);

		GLuint program;

		//Shader atributes
		GLint inVertex;
		GLint inNormal;
		//GLint inColor;
		GLint texCoordID;
		GLuint textureID;

		std::vector<float> vertexVector;
		std::vector<float> normalVector;
		//std::vector<float> colorVector;
		std::vector<float> texCoords;
		std::vector<unsigned int> indexVector;

		//Uniform variables for shaders
		GLint mvpMatrixID;
		GLint modelViewID;
		int texture;
		GLint showTextureID;
		GLint globalTimeID;
		float time;

		//Lighting
		GLint ambientLightID;
		GLint diffuseLightID;
		GLint lightDirectionID;

		//Lighting
		glm::vec3 ambientLight;
		glm::vec3 diffuseLight;
		glm::vec3 lightDirection;

	public:
		Vein(int NP, GLfloat tam, BezierCurve* curve, GLint textureID);
		~Vein();

		void build();

		int nextVertex(int val);
		void draw(int modo);
		void draw(bool relleno);
		void addPerlinNoise(float** perlinNoise);

		/* Init Shaders */
		void generateShader();

		void initValues();
		void initShaders();
		void generateVectors();
		void generateBuffers();
		void generateTexture();

		void draw(Camara* camara, int modo, bool mutation);
		void setDiffuseLight(glm::vec3 newLight);
		void setShowTexture(int text);
		void freeMemory();
};

