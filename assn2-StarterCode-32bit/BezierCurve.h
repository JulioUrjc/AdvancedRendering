#pragma once

// Global includes
#include <vector>
#include <glm\gtx\transform.hpp>

//Project includes
#include "PV3D.h"
//#include "ShaderLoader.h"
//#include "Camara.h"

class BezierCurve{

private:

	//Paint Curve
	//VAO
	GLuint vao;
	//VBO
	GLuint buffer[3];
	//Shaders
	//ShaderLoader vShader = ShaderLoader("./shaders/curveVShader.glsl", GL_VERTEX_SHADER);
	//ShaderLoader fShader = ShaderLoader("./shaders/curveFShader.glsl", GL_FRAGMENT_SHADER);

	GLuint program;

	//Shader atributes
	GLint inColor;
	GLint inVertex;

	//Uniform variables for shaders
	//ModelViewProjection matrix
	GLint mvpMatrixID;

	std::vector<float> vertexVector;
	std::vector<unsigned int> pointVector;

	/* Curve parameters */
	//Curve Partitions
	int nSteps;
	//Curve parameter
	float t;
	//Color curve
	glm::vec3 color;

	//Control points
	std::vector<glm::vec4> controlPointsList;
	//Interpolated curve
	std::vector<PV3D*> pointList;
	//Tangent list
	std::vector<PV3D*> tangentList;
	//Normal list
	std::vector<PV3D*> normalList;
	//Binormal list
	std::vector<PV3D*> binormalList;

	void createPoints();
	void createCurve();
	float numRandom();

public:

	BezierCurve(int nS, float t);

	std::vector<glm::vec4> getControlPointsList();
	std::vector<PV3D*> getPointList();
	std::vector<PV3D*> getTangentList();
	std::vector<PV3D*> getNormalList();
	std::vector<PV3D*> getBinormalList();
	int nPoints();
	void draw(int modo);
	//void draw(Camara* camera);

	///* Init Shaders */
	//void initValues();
	//void initShaders();
	//void generateVectors();
	//void generateBuffers();

};

