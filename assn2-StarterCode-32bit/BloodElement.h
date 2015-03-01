#pragma once

#include <Windows.h>
#include <GL/gl.h>
#include <GL/GLU.h>
#include "GL/glut.h"

#include "PV3D.h"

enum Obj_types{ PRIMITIVE, EXTERN };
enum Primitives{ CUBE, PLANE, CONE, CYLINDER, DISK, SPHERE, TORUS, TEAPOT, SPLINE, FONT };

class BloodElement
{
private:

	PV3D position;
	PV3D rotation;

public:

	BloodElement();
	~BloodElement();

	void create(const char *name, int primitive, GLfloat size = 0.0, GLfloat height = 0.0, GLint sliceX = 0, GLint sliceY = 0);
};

