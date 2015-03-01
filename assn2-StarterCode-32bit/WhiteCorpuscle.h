#pragma once

#include "GL/glut.h"

#include "Mesh.h"
#include "PV3D.h"

class WhiteCorpuscle
{
private:
	PV3D position;
	PV3D rotation;

public:
	WhiteCorpuscle(PV3D pos, PV3D rot);
	~WhiteCorpuscle();
	
	void draw(int modo);

	PV3D getPosition();
	void setPosition(PV3D point);
	PV3D getRotation();
	void setRotation(PV3D point);
};

