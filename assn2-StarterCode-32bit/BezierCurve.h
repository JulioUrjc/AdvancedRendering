#pragma once

#include <vector>
#include <glm\gtx\transform.hpp>
#include "PV3D.h"

class BezierCurve{

private:

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
	BezierCurve();

	std::vector<glm::vec4> getControlPointsList();
	std::vector<PV3D*> getPointList();
	std::vector<PV3D*> getTangentList();
	std::vector<PV3D*> getNormalList();
	std::vector<PV3D*> getBinormalList();
	int nPoints();
};

