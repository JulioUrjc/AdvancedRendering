#pragma once

#include <Windows.h>
#include <GL/gl.h>
#include <math.h>

class Point3D{
private:
	GLdouble corX;
	GLdouble corY;
	GLdouble corZ;
	int pv;

public:

	Point3D();
	~Point3D();
	Point3D(GLdouble x, GLdouble y, GLdouble z);
	GLdouble getX();
	void setX(GLdouble x);
	GLdouble getY();
	void setY(GLdouble y);
	GLdouble getZ();
	void setZ(GLdouble z);

	int getPv();
	void setPv(int val);

	Point3D* clone();
	Point3D* addition(Point3D* v);
	Point3D* subtraction(Point3D* v);
	void normalize();
	GLdouble magnitude();
	Point3D* crossProduct(Point3D* v);
	Point3D* matrixProduct(Point3D* Nt, Point3D* Bt, Point3D* Tt, Point3D* Ct);
};

