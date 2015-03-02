#pragma once


#define _USE_MATH_DEFINES

#include <Windows.h>
#include <GL/gl.h>
#include <GL/GLU.h>
#include "GL/glut.h"

#include <math.h>
#include <vector>

#include "PV3D.h"

enum Obj_types{ PRIMITIVE, EXTERN };
enum Primitives{ CUBE, PLANE, CONE, CYLINDER, DISK, SPHERE, TORUS, TEAPOT, SPLINE, FONT };
enum Elements{ RED, WHITE };

class BloodElement
{
private:

	PV3D position;
	PV3D rotation;

	Elements type;

	std::vector<PV3D> vertex;

public:

	BloodElement(Elements element, PV3D pos, PV3D rot);
	~BloodElement();

	void draw(int modo);

	void createElement(Elements element);
	void createPrimitive(Primitives primitive, GLfloat size = 0.0, GLfloat height = 0.0, GLint sliceX = 0, GLint sliceY = 0);
};

