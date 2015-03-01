#pragma once

#include <Windows.h>
#include <GL/gl.h>
#include <GL/GLU.h>
#include "GL/glut.h"

#include "PV3D.h"

class RedCorpuscle
{
private:
	PV3D position;
	PV3D rotation;

public:
	RedCorpuscle(PV3D pos, PV3D rot);
	~RedCorpuscle();

	void draw(int modo);

	PV3D getPosition();
	void setPosition(PV3D point);
	PV3D getRotation();
	void setRotation(PV3D point);
};

