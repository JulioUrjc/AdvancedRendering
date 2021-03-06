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

	int intRandom(int max);
};

