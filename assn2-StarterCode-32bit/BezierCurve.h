#pragma once

#include <vector>
#include <glm\gtx\transform.hpp>
#include "PV3D.h"

class BezierCurve{

private:

	////Control points
	//std::vector<glm::vec4> controlPointList;
	////Interpolated curve
	//std::vector<PV3D*> pointList;
	////Tangent list
	//std::vector<PV3D*> tangentList;
	////Normal list
	//std::vector<PV3D*> normalList;
	////Binormal list
	//std::vector<PV3D*> binormalList;

	//Control points
	std::vector<glm::vec4> controlPointList;

	//Interpolated curve
	std::vector<glm::vec3> pointList;

	//Tangent list
	std::vector<glm::vec3> tangentList;

	//Binormal list
	std::vector<glm::vec3> binormalList;

	void generateControlPoints();
	void generateCurve();
	float getRandom();

public:
	BezierCurve();

	/*std::vector<glm::vec4> getControlPointList();
	std::vector<PV3D*> getPointList();
	std::vector<PV3D*> getTangentList();
	std::vector<PV3D*> getNormalList();
	std::vector<PV3D*> getBinormalList();*/

	std::vector<glm::vec4> getControlPointList();
	std::vector<glm::vec3> getPointList();
	std::vector<glm::vec3> getTangentList();
	std::vector<glm::vec3> getNormalList();
	std::vector<glm::vec3> getBinormalList();
	int size();
};

