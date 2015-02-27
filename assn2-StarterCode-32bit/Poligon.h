#pragma once

#define _USE_MATH_DEFINES
#include "PV3D.h"
#include "Compass.h"
#include <math.h>
#include <vector>
using namespace std;

class Poligon{
  private:
	int n_sides;
	PV3D* center;
	GLdouble radius;
	std::vector<PV3D*> *vertex;

  public:
	Poligon();
	Poligon(PV3D *c, int n, GLdouble radius);
	~Poligon();

	vector<PV3D*> * getVertex(void);
	void setVertex(vector<PV3D*> *v);
	PV3D * getCenter(void);
	void setCenter(PV3D *p);
	GLdouble getRadius(void);
	void setRadius(GLdouble r);

	void calcula_vertices(PV3D* centroTemp);
};

