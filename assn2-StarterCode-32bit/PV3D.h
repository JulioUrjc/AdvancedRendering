#pragma once

#include <Windows.h>
#include <GL/gl.h>
#include <GL/GLU.h>
#include <math.h>
#include <iostream>

class PV3D{
private:
	GLdouble corX;
	GLdouble corY;
	GLdouble corZ;

	GLdouble colR;
	GLdouble colG;
	GLdouble colB;
	int pv;

public:

	PV3D();
	~PV3D();
	PV3D(GLdouble x, GLdouble y, GLdouble z);
	GLdouble getX();
	void setX(GLdouble x);
	GLdouble getY();
	void setY(GLdouble y);
	GLdouble getZ();
	void setZ(GLdouble z);

	int getPv();
	void setPv(int val);

	PV3D* clone();
	PV3D* addition(PV3D* v);
	PV3D* subtraction(PV3D* v);
	PV3D* factor(float f);
	void normalize();
	GLdouble magnitude();
	PV3D* crossProduct(PV3D* v);
	GLdouble scalarProduct(PV3D* v);
	PV3D* matrixProduct(PV3D* Nt, PV3D* Bt, PV3D* Tt, PV3D* Ct);

	void setColor(PV3D* col);
	PV3D* getColor();
	
	void toString();
};

