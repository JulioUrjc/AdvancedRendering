#include "Camara.h"

#include <glm\gtc\matrix_transform.hpp>

Camara::Camara(PV3D eye, PV3D look, PV3D up){
	this->eye = eye;
	this->look = look;
	this->up = up;
	getCoordCam();
	fijarCam();
	ortogonal(-10,10,-10,10,1,1000);
}

Camara::Camara(PV3D eye, PV3D look, PV3D up, float xRight, float xLeft, float yTop, float yBot, float N, float F){
	this->eye = eye;
	this->look = look;
	this->up = up;
	this->xRight = xRight;
	this->xLeft = xLeft;
	this->yTop = yTop;
	this->yBot = yBot;
	this->N = N;
	this->F = F;

	glm::vec3 eye2 = glm::vec3(eye.getX(), eye.getY(), eye.getZ());
	glm::vec3 look2 = glm::vec3(look.getX(), look.getY(), look.getZ());
	glm::vec3 up2 = glm::vec3(up.getX(), up.getY(), up.getZ());
	viewMatrix = glm::lookAt(eye2, look2, up2);
	projectionMatrix = glm::perspective(60.0f, 1.0f, N, F);

	getCoordCam();
	fijarCam();
	ortogonal(-10, 10, -10, 10, 1, 1000);
}

Camara::~Camara(){}

void Camara::moveCamara(PV3D* eye, PV3D* look){
	this->eye = *eye;
	this->look = *look;
}

void Camara::moveCamara(PV3D* eye, PV3D* look, PV3D* up){
	this->eye = *eye;
	this->look = *look;
	this->up = *up;
}

void Camara::getCoordCam(){
	// n = (eye-look).normalizar()
	PV3D* aux = new PV3D(eye.getX()-look.getX(),eye.getY()-look.getY(),eye.getZ()-look.getZ());
	aux->normalize();
	n = aux;
	
	// u = (up*n).normalizar() 
	u = up.crossProduct(n);
	u->normalize();
	// v = n*u
	v = n->crossProduct(u);
}

void Camara::getMatriz(){
	/*
	GLfloat* m= new GLfloat[16];
	m[0] = this->u->getX();
	m[1] = this->v->getX();
	m[2] = this->n->getX();
	m[3] = 0;

	m[4] = this->u->getY();
	m[5] = this->v->getY();
	m[6] = this->n->getY();
	m[7] = 0;

	m[8] = this->u->getZ();
	m[9] = this->v->getZ();
	m[10] = this->n->getZ();
	m[11] = 0;

	m[12] = -(this->eye->scalarProduct(this->u));
	m[13] = -(this->eye->scalarProduct(this->v));
	m[14] = -(this->eye->scalarProduct(this->n));
	m[15] = 0;
	*/
	
	GLfloat m[16] = {
		this->u->getX(), this->v->getX(), this->n->getX(), 0,
		this->u->getY(), this->v->getY(), this->n->getY(), 0,
		this->u->getZ(), this->v->getZ(), this->n->getZ(), 0,
		-(this->eye.scalarProduct(this->u)), -(this->eye.scalarProduct(this->v)), -(this->eye.scalarProduct(this->n)), 1};
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(m);

	//delete[] m;
}

void Camara::fijarCam(){
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //getMatriz();
  gluLookAt(eye.getX(), eye.getY(), eye.getZ(), look.getX(), look.getY(), look.getZ(), up.getX(), up.getY(), up.getZ());
}

void Camara::roll(GLdouble alfa){
	getCoordCam();

	PV3D* aux = u->clone();

	u->setX((aux->getX()*cos(alfa)) - (v->getX()*sin(alfa)));
	u->setY((aux->getY()*cos(alfa)) - (v->getY()*sin(alfa)));
	u->setZ((aux->getZ()*cos(alfa)) - (v->getZ()*sin(alfa)));

	v->setX((aux->getX())*sin(alfa) + (v->getX()*cos(alfa)));
	v->setY((aux->getY())*sin(alfa) + (v->getY()*cos(alfa)));
	v->setZ((aux->getZ())*sin(alfa) + (v->getZ()*cos(alfa)));

	getMatriz();
}

void Camara::yaw(GLdouble alfa){
	getCoordCam();
	PV3D* aux = u->clone();

	u->setX((aux->getX()*cos(alfa)) + (n->getX()*sin(alfa)));
	u->setY((aux->getY()*cos(alfa)) + (n->getY()*sin(alfa)));
	u->setZ((aux->getZ()*cos(alfa)) + (n->getZ()*sin(alfa)));

	n->setX(-((aux->getX())*sin(alfa)) + (n->getX()*cos(alfa)));
	n->setY(-((aux->getY())*sin(alfa)) + (n->getY()*cos(alfa)));
	n->setZ(-((aux->getZ())*sin(alfa)) + (n->getZ()*cos(alfa)));

	getMatriz();
}

void Camara::pitch(GLdouble alfa){
	getCoordCam();
	PV3D* aux = n->clone();

	n->setX((aux->getX()*cos(alfa)) + (v->getX()*sin(alfa)));
	n->setY((aux->getY()*cos(alfa)) + (v->getY()*sin(alfa)));
	n->setZ((aux->getZ()*cos(alfa)) + (v->getZ()*sin(alfa)));

	v->setX(-((aux->getX())*sin(alfa)) + (v->getX()*cos(alfa)));
	v->setY(-((aux->getY())*sin(alfa)) + (v->getY()*cos(alfa)));
	v->setZ(-((aux->getZ())*sin(alfa)) + (v->getZ()*cos(alfa)));

	getMatriz();
}

void Camara::desplazar(GLdouble x, GLdouble y, GLdouble z){
	this->eye.setX(this->eye.getX()+x);
	this->eye.setY(this->eye.getY()+y);
	this->eye.setZ(this->eye.getZ()+z);

	this->look.setX(this->look.getX()+x);
	this->look.setY(this->look.getY()+y);
	this->look.setZ(this->look.getZ()+z);

	this->fijarCam();
	//this->ortogonal(-10,10,-10,10,1,1000);
}

void Camara::ortogonal(){
	glMatrixMode(GL_PROJECTION);
	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(xLeft, xRight, yBot, yTop, N, F);
}

void Camara::ortogonal(GLdouble left, GLdouble right, GLdouble botton, GLdouble top, GLdouble n, GLdouble f){
	glMatrixMode(GL_PROJECTION);
	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(left, right, botton, top, n, f);
}

void Camara::perspectiva(GLdouble left, GLdouble right, GLdouble botton, GLdouble top, GLdouble n, GLdouble f){
	glMatrixMode(GL_PROJECTION);
	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glFrustum(left,right,bottom,top,nearAux,farAux);
	gluPerspective(5, 2, n, f);
}

void Camara::oblicua(PV3D* vector, GLdouble left, GLdouble right, GLdouble botton, GLdouble top, GLdouble n, GLdouble f){
	glMatrixMode(GL_PROJECTION);
	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	vector->normalize();

	if(vector->getZ() != 0.0 && (vector->getX() != 0 || vector->getY() != 0 || vector->getZ() != 1)){
		GLfloat m[16] = {	
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		m[8] = -(vector->getX()) / vector->getZ();
		m[9] = -(vector->getY()) / vector->getZ();
		m[12] = -n * (vector->getX() / vector->getZ());
		m[13] = -n * (vector->getY() / vector->getZ());

		glOrtho(left, right, botton, top, n, f);
		glMultMatrixf(m);
		
	}
}

void Camara::recorridoEje(GLdouble x, GLdouble y, GLdouble z){
	this->eye.setX(this->eye.getX()+x);
	this->eye.setY(this->eye.getY()+y);
	this->eye.setZ(this->eye.getZ()+z);
	
	this->fijarCam();
	this->ortogonal(-10,10,-10,10,1,1000);
}

void Camara::giraX(GLdouble alfa){
	GLdouble angle2 = atan2(eye.getZ(), eye.getY()) + alfa;
	GLdouble dst = sqrt((eye.getY() * eye.getY()) + (eye.getZ() * eye.getZ()));

	eye.setY(dst * cos(angle2));
	eye.setZ(dst * sin(angle2));

	this->fijarCam();
	this->ortogonal(-10,10,-10,10,1,1000);
}

void Camara::giraY(GLdouble alfa){
	GLdouble angle2 = atan2(eye.getZ(), eye.getX()) + alfa;
	GLdouble dst = sqrt((eye.getX() * eye.getX()) + (eye.getZ() * eye.getZ()));

	eye.setX(dst * cos(angle2));
	eye.setZ(dst * sin(angle2));

	this->fijarCam();
	this->ortogonal(-10,10,-10,10,1,1000);
}

void Camara::giraZ(GLdouble alfa){
	GLdouble angle2 = atan2(eye.getY(), eye.getX()) + alfa;
	GLdouble dst = sqrt((eye.getY() * eye.getY()) + (eye.getX() * eye.getX()));

	eye.setX(dst * cos(angle2));
	eye.setY(dst * sin(angle2));

	this->fijarCam();
}

void Camara::lateral(){
	this->eye.setX(100);
	this->eye.setY(0);
	this->eye.setZ(0);

	this->up.setX(0);
	this->up.setY(1);
	this->up.setZ(0);

	this->fijarCam();
	this->ortogonal(-10,10,-10,10,1,1000);
}

void Camara::frontal(){
	this->eye.setX(0);
	this->eye.setY(0);
	this->eye.setZ(100);

	this->up.setX(0);
	this->up.setY(1);
	this->up.setZ(0);

	this->fijarCam();
	this->ortogonal(-10,10,-10,10,1,1000);
}

void Camara::cenital(){
	this->eye.setX(0);
	this->eye.setY(100);
	this->eye.setZ(0);

	this->up.setX(1);
	this->up.setY(0);
	this->up.setZ(0);

	this->fijarCam();
	this->ortogonal(-10,10,-10,10,1,1000);
}

void Camara::esquina(){
	this->eye.setX(100);
	this->eye.setY(100);
	this->eye.setZ(100);

	this->up.setX(0);
	this->up.setY(1);
	this->up.setZ(0);

	this->fijarCam();
	this->ortogonal(-10,10,-10,10,1,1000);
}

glm::mat4 Camara::getModelView(glm::mat4 modelMatrix){
	return viewMatrix*modelMatrix;
}
glm::mat4 Camara::getModelViewProjection(glm::mat4 modelMatrix){
	return projectionMatrix*viewMatrix*modelMatrix;
}