#include "BezierCurve.h"
#include <vector>
#include <glm\gtx\transform.hpp>

#include <cstdlib>
#include <time.h>

BezierCurve::BezierCurve(int steps, float curv){
	nSteps = steps;
	t = curv;
	srand(time(NULL));
	createPoints();
	createCurve();
}

void BezierCurve::createPoints(){
	//Create some initial control points
	glm::vec4 initialPoint = glm::vec4(-4, 5, 0, 0);
	controlPointsList.push_back(initialPoint);

	////2
	controlPointsList.push_back(glm::vec4(-2,-5,0,0));

	////3
	controlPointsList.push_back(glm::vec4(0,0,0,0));

	////4
	controlPointsList.push_back(glm::vec4(2,-5,0,0));

	////5
	controlPointsList.push_back(glm::vec4(4,5,0,0));

}

void BezierCurve::createCurve(){
	
	glm::mat4x4 mMat = glm::mat4x4(0,   1,     0,  0,
								  -t,   0,     t,  0,
							     2*t, t-3, 3-2*t, -t,
								  -t, 2-t,   t-2,  t);

	for (int i = 2; i< controlPointsList.size()+2; ++i){
		for (int step = 0; step< nSteps; step++){
			float u = ((float)step) / ((float)nSteps);

			glm::vec4   uVec = glm::vec4(1, u, u*u, u*u*u);
			glm::mat4x4 pVec = glm::mat4x4(controlPointsList[(i-2) %controlPointsList.size()],
										   controlPointsList[(i-1) %controlPointsList.size()],
										   controlPointsList[(i)   %controlPointsList.size()],
										   controlPointsList[(i+1) %controlPointsList.size()]);

			glm::vec4 point = pVec*(mMat*uVec);

			//Adding vertex of interpolated point
			pointList.push_back(new PV3D(point.x, point.y, point.z));
			pointList_np.push_back(PV3D(point.x, point.y, point.z));
		}
	}

	//Calculate tangents, normals and binormals
	for (int i = 2; i < pointList.size() + 2; ++i){
		////Tangent = t*([pi-1] - [pi+1])
		PV3D* tangent = new PV3D();
		tangent = (pointList[(i+1) %pointList.size()]->subtraction(pointList[(i-1) %pointList.size()]))->factor(t); tangent->normalize();
		tangentList.push_back(tangent);
		PV3D tangent2 = PV3D();
		tangent2 = (pointList_np[(i + 1) % pointList_np.size()].subtraction(pointList_np[(i - 1) % pointList_np.size()])).factor_np(t); tangent2.normalize();
		tangentList_np.push_back(tangent2);

		//////Binormal = Normal x Tangent
		PV3D* normal = new PV3D();
		normal->setZ(1.0f); //Set normal vector as z-axis
		//normal = tangent->crossProduct(new PV3D(0, 1, 0)); normal->normalize();
		//normal = (pointList[(i) % pointList.size()]->subtraction(pointList[(i - 2) % pointList.size()]))->factor(t); normal->normalize();
		normalList.push_back(normal);
		PV3D* binormal = normal->crossProduct(tangent);  binormal->normalize();
		binormalList.push_back(binormal);
		PV3D normal2 = PV3D();
		normal2 = (pointList_np[(i) % pointList_np.size()].subtraction(pointList_np[(i - 2) % pointList_np.size()])).factor_np(t); normal2.normalize();
		normalList_np.push_back(normal2);
		PV3D binormal2 = normal2.crossProduct(tangent2);  binormal2.normalize();
		binormalList_np.push_back(binormal2);
	}
}

float BezierCurve::numRandom(){
	return ((float)rand() / (RAND_MAX + 1)) * 10;
}

std::vector<glm::vec4> BezierCurve::getControlPointsList(){
	return controlPointsList;
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

std::vector<PV3D> BezierCurve::getPointList_np(){
	return pointList_np;
}

std::vector<PV3D> BezierCurve::getTangentList_np(){
	return tangentList_np;
}

std::vector<PV3D> BezierCurve::getNormalList_np(){
	return normalList_np;
}

std::vector<PV3D> BezierCurve::getBinormalList_np(){
	return binormalList_np;
}

int BezierCurve::nPoints(){
	return pointList.size();
}