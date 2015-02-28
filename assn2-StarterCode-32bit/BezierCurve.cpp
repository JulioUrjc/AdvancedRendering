#include "BezierCurve.h"
#include <vector>
#include <glm\gtx\transform.hpp>

#include <cstdlib>
#include <time.h>

BezierCurve::BezierCurve(){
	generateControlPoints();
	generateCurve();
	srand(time(NULL));
}

void BezierCurve::generateControlPoints(){
	////Create some initial control points
	//////1
	//glm::vec4 initialPoint = glm::vec4(-60, -80, 0.0, 0.0);
	//2
	//glm::vec4 initialPoint = glm::vec4(-2000 + getRandom(), 500 + getRandom(), 4000 + getRandom(), 0);
	//2
	//controlPointList.push_back(glm::vec4(-2000 + getRandom(), 500 + getRandom(), 2000 + getRandom(), 0));
	//3
	//controlPointList.push_back(glm::vec4(-3000 + getRandom(), 500 + getRandom(), -2000 + getRandom(), 0));
	////4
	//controlPointList.push_back(glm::vec4(1000 + getRandom(), 1500 + getRandom(), 1500 + getRandom(), 0));
	////5
	//controlPointList.push_back(glm::vec4(1500 + getRandom(), 1000 + getRandom(), 1500 + getRandom(), 0));
	////6
	//controlPointList.push_back(glm::vec4(1500 + getRandom(), 1000 + getRandom(), 1000 + getRandom(), 0));
	////7
	//controlPointList.push_back(glm::vec4(5000 + getRandom(), 1000 + getRandom(), 500 + getRandom(),0));
	////8
	//controlPointList.push_back(glm::vec4(6000 + getRandom(), 1500, 500, 0));
	////8
	//controlPointList.push_back(glm::vec4(6000 + getRandom(), 2000, 0, 0));
	//9
	//controlPointList.push_back(initialPoint);

	//Create some initial control points
	glm::vec4 initialPoint = glm::vec4(-4, 5, 0, 0);
	controlPointList.push_back(initialPoint);

	////2
	controlPointList.push_back(glm::vec4(-2,-5,0,0));

	////3
	controlPointList.push_back(glm::vec4(0,0,0,0));

	////4
	controlPointList.push_back(glm::vec4(2,-5,0,0));

	////5
	controlPointList.push_back(glm::vec4(4,5,0,0));

	//////6
	//controlPointList.push_back(glm::vec4(3000,2000,2000,0));

	//////7
	//controlPointList.push_back(glm::vec4(3000,2000,1000,0));

	//////8
	//controlPointList.push_back(glm::vec4(2000,3000,1000,0));

}

void BezierCurve::generateCurve(){
	// step variable
	int nSteps = 25;

	// Tension
	float t= 0.5;
	glm::mat4x4 mMat = glm::mat4x4(0,   1,     0,  0,
								  -t,   0,     t,  0,
							     2*t, t-3, 3-2*t, -t,
								  -t, 2-t,   t-2,  t);

	for (int i = 2; i< controlPointList.size()+2; ++i){
		for (int step = 0; step< nSteps; step++){
			float u = ((float)step) / ((float)nSteps);

			glm::vec4   uVec = glm::vec4(1, u, u*u, u*u*u);
			glm::mat4x4 pVec = glm::mat4x4(controlPointList[(i-2) % controlPointList.size()],
										   controlPointList[(i-1) % controlPointList.size()],
										   controlPointList[(i)   % controlPointList.size()],
										   controlPointList[(i+1) % controlPointList.size()]);

			//Interpolate
			glm::vec4 p = pVec*(mMat*uVec);

			//Adding vertex of interpolated point
			pointList.push_back(new PV3D(p.x, p.y, p.z));


		}
	}

	//Calculate tangents and binormals
	for (int i = 2; i < pointList.size() + 2; ++i)
	{
		//Tangent = t*([pi-1] - [pi+1])
		//glm::vec3 tangent = glm::normalize(t*(pointList[(i + 1) % pointList.size()] - pointList[(i - 1) % pointList.size()]));
		//tangentList.push_back(tangent);
		PV3D* tangent = new PV3D();
		tangent = (pointList[(i + 1) % pointList.size()]->subtraction(pointList[(i - 1) % pointList.size()]))->factor(t);
		tangent->normalize();
		tangentList.push_back(tangent);

		////Binormal = Normal x Tangent
		//glm::vec3 normal = glm::normalize(glm::cross(tangent, glm::vec3(0, 1, 0)));
		//glm::vec3 binormal = glm::normalize(glm::cross(normal, tangent));
		//binormalList.push_back(binormal);
		PV3D* normal = tangent->crossProduct(new PV3D(0, 1, 0));
		normal->normalize();
		normalList.push_back(normal);
		PV3D* binormal = normal->crossProduct(tangent);
		binormal->normalize();
		binormalList.push_back(binormal);
	}
}

float BezierCurve::getRandom(){
	return ((float)rand() / (RAND_MAX + 1)) * 1000;
}


std::vector<glm::vec4> BezierCurve::getControlPointList(){
	return controlPointList;
}

std::vector<PV3D*> BezierCurve::getPointList(){
	return pointList;
}

std::vector<PV3D*> BezierCurve::getTangentList(){
	return tangentList;
}

std::vector<PV3D*> BezierCurve::getNormalList(){
	return normalList;
}

std::vector<PV3D*> BezierCurve::getBinormalList(){
	return binormalList;
}

int BezierCurve::nPoints(){
	return pointList.size();
}