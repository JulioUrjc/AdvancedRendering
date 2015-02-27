#pragma once

#include "Point3D.h"

class Compass{
  private:
	Point3D *pos;
	GLdouble dir;

  public:
	Compass();
	~Compass();
	Compass(Point3D *p, GLdouble dir);
	void setPos(Point3D *p);
	void setDir(GLdouble d);
	GLdouble getDir();
	Point3D* getPos();
	void turn(GLdouble a);
	void forward(GLdouble d);
};

