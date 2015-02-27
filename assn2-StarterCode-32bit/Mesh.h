#pragma once

#include "PV3D.h"
#include "Face.h"

#include <vector>
using namespace std;

class Mesh{
  protected:
	int numVertex;
	int numFaces;
	int numNormals;

	std::vector<PV3D*> *vertex;
	std::vector<PV3D*> *normals;
	std::vector<Face*> *faces;

  public:
	Mesh(int nV, int nF, int nN);
	~Mesh();
	virtual void draw(bool fill);
	virtual void build() = 0;
	//PV3D* doVectorNormalNewell(Face* c);
};

