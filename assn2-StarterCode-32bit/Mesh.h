#pragma once

#include "Point3D.h"
#include "Face.h"

#include <vector>
using namespace std;

class Mesh{
  protected:
	int numVertex;
	int numFaces;
	int numNormals;

	std::vector<Point3D*> *vertex;
	std::vector<Point3D*> *normals;
	std::vector<Face*> *faces;

  public:
	Mesh(int nV, int nF, int nN);
	~Mesh();
	virtual void draw(bool fill);
	virtual void build() = 0;
	//Point3D* doVectorNormalNewell(Face* c);
};

