#pragma once

#include <vector>
#include <glm\gtx\transform.hpp>
#include "PV3D.h"

class BezierCurve{

private:

	//Curve Partitions
	int nSteps;
	//Curve parameter
	float t;

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

	//Interpolated curve
	std::vector<PV3D> pointList_np;
	//Tangent list
	std::vector<PV3D> tangentList_np;
	//Normal list
	std::vector<PV3D> normalList_np;
	//Binormal list
	std::vector<PV3D> binormalList_np;

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

	std::vector<PV3D> getPointList_np();
	std::vector<PV3D> getTangentList_np();
	std::vector<PV3D> getNormalList_np();
	std::vector<PV3D> getBinormalList_np();
	int nPoints();
};

