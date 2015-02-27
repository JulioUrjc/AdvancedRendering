#pragma once

#include "PV3D.h"

class Compass{
  private:
	PV3D *pos;
	GLdouble dir;

  public:
	Compass();
	~Compass();
	Compass(PV3D *p, GLdouble dir);
	void setPos(PV3D *p);
	void setDir(GLdouble d);
	GLdouble getDir();
	PV3D* getPos();
	void turn(GLdouble a);
	void forward(GLdouble d);
};

