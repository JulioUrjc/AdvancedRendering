#pragma once

#include "PV3D.h"
#include "BezierCurve.h"
#include <glm\glm.hpp>

const float MAX_ZOOM = 1.0f;
const float MIN_ZOOM = 0.4f;

class Camara
{
private:
	PV3D eye, look, up;
	PV3D *n, *v, *u;
	float xRight, xLeft, yTop, yBot, N, F;
	float fovy;
	float aspect;
	float zoom;

	//View and projection matrix
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	//Curve Values
	BezierCurve *curve;
	int pointCurve;

public:
	Camara(PV3D eye, PV3D look, PV3D up, float N, float F, float fovy, float aspect, BezierCurve* c);
	~Camara();
	/* Camara en OpenGL*/
	glm::mat4 getModelView(glm::mat4 modelMatrix);
	glm::mat4 getModelViewProjection(glm::mat4 modelMatrix);
	void followCurve(bool alante);
	void followCurveOut(int alante, float displaced);
	void reDisplay();
	void addZoom(float val);
	void deductZoom(float val);
	void setCurve(BezierCurve* c){ this->curve = c; }
	int getCurrentPoint();

	/* Camara antigua por CPU*/
	//Camara(PV3D eye, PV3D look, PV3D up, float xRight, float xLeft, float yTop, float yBot, float N, float F, float fovy, float aspect, BezierCurve* c);
	//Camara(PV3D eye, PV3D look, PV3D up);
	/*void moveCamara(PV3D* eye, PV3D* look, PV3D* up);
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
	void fijarCam();*/
};

