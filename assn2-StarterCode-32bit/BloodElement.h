#pragma once

#define _USE_MATH_DEFINES

//#include <Windows.h>
//#include <GL/gl.h>
//#include <GL/GLU.h>
//#include "GL/glut.h"

#include <glm\gtx\transform.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include <math.h>
#include <vector>

#include "ShaderLoader.h"
#include "PV3D.h"
#include "Camara.h"
#include "GL/glut.h"


enum Obj_types{ PRIMITIVE, EXTERN };
enum Primitives{ CUBE, PLANE, CONE, CYLINDER, DISK, SPHERE, TORUS, TEAPOT, SPLINE, FONT, CELL};
enum Elements{ RED, WHITE };

class BloodElement
{
private:

	PV3D position;
	PV3D rotation;

	Elements type;

	std::vector<PV3D> vertex;
	std::vector<PV3D> normals;

	glm::mat4x4 rotationMatrix;
	glm::mat4x4 translateMatrix;

	void initMatrix();

	// Paint Blood Element
	//VAO
	GLuint vao;
	//VBO
	GLuint buffer[3];
	//Shaders
	ShaderLoader vShader = ShaderLoader("./shaders/bloodVShader.glsl", GL_VERTEX_SHADER);
	ShaderLoader fShader = ShaderLoader("./shaders/bloodFShader.glsl", GL_FRAGMENT_SHADER);

	GLuint program;

	//Shader atributes
	GLint inVertex;
	GLint inNormal;
	GLint inColor;
	//GLint texCoordID;

	std::vector<float> vertexVector;
	std::vector<float> normalVector;
	//std::vector<float> colorVector;
	//std::vector<float> texCoords;
	std::vector<unsigned int> indexVector;

	//Uniform variables for shaders
	GLint mvpMatrixID;
	GLint modelViewID;

	//Lighting
	GLint ambientLightID;
	GLint diffuseLightID;
	GLint lightDirectionID;

	//Texture
	//GLuint textureID;
	//std::vector<float> texCoords;

	//Lighting
	glm::vec3 ambientLight;
	glm::vec3 diffuseLight;
	glm::vec3 lightDirection;

public:

	BloodElement(Elements element, PV3D pos, PV3D rot);
	~BloodElement();

	void draw(int modo);

	void createElement(Elements element);
	void createPrimitive(Primitives primitive, GLfloat size = 0.0, GLfloat height = 0.0, GLint sliceX = 0, GLint sliceY = 0, bool interior=false);

	/* Init Shaders */
	void initValues();
	void initShaders();
	void generateVectors();
	void generateBuffers();
	//void generateTexture();

	void draw(Camara* camara, int modo, bool mutation);
	void setDiffuseLight(glm::vec3 newLight);
	void freeMemory();
};

