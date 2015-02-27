#pragma once

#define _USE_MATH_DEFINES
#include "Point3D.h"
#include "Compass.h"
#include <math.h>
#include <vector>
using namespace std;

class Poligon{
  private:
	int n_sides;
	Point3D* center;
	GLdouble radius;
	std::vector<Point3D*> *vertex;

  public:
	Poligon();
	Poligon(Point3D *c, int n, GLdouble radius);
	~Poligon();

	vector<Point3D*> * getVertex(void);
	void setVertex(vector<Point3D*> *v);
	Point3D * getCenter(void);
	void setCenter(Point3D *p);
	GLdouble getRadius(void);
	void setRadius(GLdouble r);

	void calcula_vertices(Point3D* centroTemp);
};

