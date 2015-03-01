#pragma once

#include "PV3D.h"

class Camara
{
private:
	PV3D eye, look, up;
	PV3D *n, *v, *u;
	float xRight, xLeft, yTop, yBot, N, F;

public:
	Camara(PV3D eye, PV3D look, PV3D up);
	Camara(PV3D eye, PV3D look, PV3D up, float xRight, float xLeft, float yTop, float yBot, float N, float F);
	~Camara();

	void moveCamara(PV3D* eye, PV3D* look, PV3D* up);
	void moveCamara(PV3D* eye, PV3D* look);
	void roll(GLdouble alfa);
	void yaw(GLdouble alfa);
	void pitch(GLdouble alfa);
	void desplazar(GLdouble x, GLdouble y, GLdouble z);
	void ortogonal();
	void ortogonal(GLdouble left, GLdouble right, GLdouble botton, GLdouble top, GLdouble near, GLdouble far);
	void perspectiva(GLdouble left, GLdouble right, GLdouble botton, GLdouble top, GLdouble near, GLdouble far);
	void oblicua(PV3D* vector, GLdouble left, GLdouble right, GLdouble botton, GLdouble top, GLdouble near, GLdouble far);
	void recorridoEje(GLdouble x, GLdouble y, GLdouble z);
	void giraX(GLdouble alfa);
	void giraY(GLdouble alfa);
	void giraZ(GLdouble alfa);
	void lateral();
	void cenital();
	void frontal();
	void esquina();
	
	void getCoordCam();
	void getMatriz();
	void fijarCam();
};
