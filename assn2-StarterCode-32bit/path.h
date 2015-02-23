#pragma once

#include <vector>
#include <glm\gtx\transform.hpp>

class path{

private:

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
	path();
	~path();

	std::vector<glm::vec4> getControlPointList();
	std::vector<glm::vec3> getPointList();
	std::vector<glm::vec3> getTangentList();
	std::vector<glm::vec3> getBinormalList();
	int size();
};

