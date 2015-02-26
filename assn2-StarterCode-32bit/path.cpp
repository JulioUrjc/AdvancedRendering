#include "path.h"
#include <vector>
#include <glm\gtx\transform.hpp>

#include <cstdlib>
#include <time.h>

Path::Path(){
	generateControlPoints();
	generateCurve();

	srand(time(NULL));
}

void Path::generateControlPoints()
{
	////Create some initial control points
	//////1
	controlPointList.push_back(glm::vec4(-2000 + getRandom(), 500 + getRandom(), 4000 + getRandom(), 0));

	//2
	controlPointList.push_back(glm::vec4(-2000 + getRandom(), 500 + getRandom(), 2000 + getRandom(), 0));

	//3
	controlPointList.push_back(glm::vec4(-3000 + getRandom(), 500 + getRandom(), -2000 + getRandom(), 0));

	//4
	controlPointList.push_back(glm::vec4(1000 + getRandom(), 1500 + getRandom(), 1500 + getRandom(), 0));

	//5
	controlPointList.push_back(glm::vec4(1500 + getRandom(), 1000 + getRandom(), 1500 + getRandom(), 0));

	//6
	controlPointList.push_back(glm::vec4(1500 + getRandom(), 1000 + getRandom(), 1000 + getRandom(), 0));

	//7
	controlPointList.push_back(glm::vec4(5000 + getRandom(), 1000 + getRandom(), 500, getRandom()));

	//8
	controlPointList.push_back(glm::vec4(6000 + getRandom(), 1500, 500, 0));

	//8
	controlPointList.push_back(glm::vec4(6000 + getRandom(), 2000, 0, 0));

	//Create some initial control points
	////1
	//controlPointList.push_back(glm::vec4(1000,1000,1000,0));

	////2
	//controlPointList.push_back(glm::vec4(1000,1000,2000,0));

	////3
	//controlPointList.push_back(glm::vec4(1000,1000,3000,0));

	////4
	//controlPointList.push_back(glm::vec4(2000,3000,3000,0));

	////5
	//controlPointList.push_back(glm::vec4(3000,1000,3000,0));

	////6
	//controlPointList.push_back(glm::vec4(3000,2000,2000,0));

	////7
	//controlPointList.push_back(glm::vec4(3000,2000,1000,0));

	////8
	//controlPointList.push_back(glm::vec4(2000,3000,1000,0));
}


void Path::generateCurve(){
	//step variables
	int nSteps = 300;
	float u = 0;

	//Tension
	float t = 0.5;
	glm::mat4x4 mMat = glm::mat4x4(0, 1, 0, 0,
		-t, 0, t, 0,
		2 * t, t - 3, 3 - 2 * t, -t,
		-t, 2 - t, t - 2, t);
	//glm::mat4x4 mMat = glm::mat4x4(	0,		2,		0,		0,
	//									-1,		0,		1,		0,
	//									2,		-5,		4,		-1,
	//									-1,		3,		-3,		1);

	glm::vec4 p;
	for (int i = 2; i<controlPointList.size() + 2; ++i)
	{
		for (int step = 0; step<nSteps; step++)
		{
			u = ((float)step) / ((float)nSteps);

			glm::vec4 uVec = glm::vec4(1, u, u*u, u*u*u);
			glm::mat4x4 pVec = glm::mat4x4(controlPointList[(i - 2) % controlPointList.size()],
				controlPointList[(i - 1) % controlPointList.size()],
				controlPointList[(i) % controlPointList.size()],
				controlPointList[(i + 1) % controlPointList.size()]);

			//Interpolate
			p = pVec*(mMat*uVec);

			//Adding vertex of interpolated point
			pointList.push_back(glm::vec3(p.x, p.y, p.z));


		}
	}

	//Calculate tangents and binormals
	for (int i = 2; i<pointList.size() + 2; ++i)
	{
		//Tangent = t*([pi-1] - [pi+1])
		glm::vec3 tangent = glm::normalize(t*(pointList[(i + 1) % pointList.size()] - pointList[(i - 1) % pointList.size()]));
		tangentList.push_back(tangent);

		//Binormal = Normal x Tangent
		glm::vec3 normal = glm::normalize(glm::cross(tangent, glm::vec3(0, 1, 0)));
		glm::vec3 binormal = glm::normalize(glm::cross(normal, tangent));
		binormalList.push_back(binormal);
	}

}

float Path::getRandom(){
	return ((float)rand() / (RAND_MAX + 1)) * 1000;
}



std::vector<glm::vec4> Path::getControlPointList(){
	return controlPointList;
}


std::vector<glm::vec3> Path::getPointList(){
	return pointList;
}

std::vector<glm::vec3> Path::getTangentList(){
	return tangentList;
}

std::vector<glm::vec3> Path::getBinormalList(){
	return binormalList;
}

int Path::size(){
	return pointList.size();
}