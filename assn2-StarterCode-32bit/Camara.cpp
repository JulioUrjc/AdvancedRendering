#include "Camara.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include <iostream>
using namespace std;

Camara::Camara(PV3D eye, PV3D look, PV3D up, float N, float F, float fovy, float aspect, BezierCurve* c, float radius){
	this->eye = eye;
	this->look = look;
	this->up = up;
	this->N = N;
	this->F = F;
	this->fovy = fovy;
	this->aspect = aspect;
	this->curve = c;
	this->pointCurve = 0;
	this->zoom = 1.0f;

	veinRadius = radius;

	displaceNormal = glm::vec3(0.0f,0.0f,0.0f);
	displaceBinormal = glm::vec3(0.0f, 0.0f, 0.0f);

	viewMatrix = glm::lookAt(eye.convertVec3(), eye.convertVec3() + look.convertVec3(), up.convertVec3());
	projectionMatrix = glm::perspective(glm::radians(fovy), aspect, N, F);
}

Camara::~Camara(){}

/* Adquirir matrices de ModelView y de Proyeccion*/
glm::mat4 Camara::getModelView(glm::mat4 modelMatrix){
	return viewMatrix*modelMatrix;
}
glm::mat4 Camara::getModelViewProjection(glm::mat4 modelMatrix){
	return projectionMatrix*viewMatrix*modelMatrix;
}

/* Follow Curve*/
void Camara::followCurve(bool alante){
	if (alante)
		pointCurve++;
	else
		pointCurve--;

	if (pointCurve >= curve->nPoints())  pointCurve = 0;
	if (pointCurve < 0)  pointCurve = curve->nPoints() - 1;

	if (mutation)
		projectionMatrix = glm::perspective(120.0f*zoom, 0.5f, N, F);
	else
		projectionMatrix = glm::perspective(glm::radians(fovy*zoom), aspect, N, F);

	eye = *curve->getPointList().at(pointCurve);
	eye = *eye.addition(&PV3D(displaceNormal.x, displaceNormal.y, displaceNormal.z));
	eye = *eye.addition(&PV3D(displaceBinormal.x, displaceBinormal.y, displaceBinormal.z));

	viewMatrix = glm::lookAt(eye.convertVec3(), eye.convertVec3() + curve->getTangentList().at(pointCurve)->convertVec3(),
		curve->getBinormalList().at(pointCurve)->convertVec3());
}

/* Simulate Heart Beat*/
void Camara::simulateHeartBeat(int acceleration){
	
	(acceleration > 20) ? pointCurve += 4 : (acceleration>19)? pointCurve: ++pointCurve ;

	if (pointCurve >= curve->nPoints())  pointCurve = 0;
	//if (pointCurve < 0)  pointCurve = curve->nPoints() - 1;

	eye = *curve->getPointList().at(pointCurve);

	if (mutation){
		projectionMatrix = glm::perspective(90.0f*zoom, 0.1f, N, F);
		viewMatrix = glm::lookAt(eye.convertVec3(), curve->getTangentList().at(pointCurve)->convertVec3(),
			curve->getBinormalList().at(pointCurve)->convertVec3());
	}else{
		projectionMatrix = glm::perspective(glm::radians(fovy*zoom), aspect, N, F);
		viewMatrix = glm::lookAt(eye.convertVec3(), eye.convertVec3() + curve->getTangentList().at(pointCurve)->convertVec3(),
			curve->getBinormalList().at(pointCurve)->convertVec3());
	}


	viewMatrix = glm::lookAt(eye.convertVec3(), eye.convertVec3() + curve->getTangentList().at(pointCurve)->convertVec3(),
		curve->getBinormalList().at(pointCurve)->convertVec3());
}

/* Follow Curve Out*/
void Camara::followCurveOut(int alante, float displaced){

	if (alante==0)
		pointCurve++;
	else if (alante==1)
		pointCurve--;

	if (pointCurve >= curve->nPoints())  pointCurve = 0;
	if (pointCurve < 0)  pointCurve = curve->nPoints() - 1;

	projectionMatrix = glm::perspective(glm::radians(fovy*zoom), aspect, N, F);

	eye = *curve->getPointList().at(pointCurve);

	glm::vec3 displacement = glm::vec3(0.0, 0.0, displaced);
	viewMatrix = glm::lookAt(eye.convertVec3()+displacement, eye.convertVec3(),
		curve->getBinormalList().at(pointCurve)->convertVec3());
}

/* ReDisplay */
void Camara::reDisplay(){
	if (mutation)
		projectionMatrix = glm::perspective(fovy*zoom, aspect, N, F);
	else
		projectionMatrix = glm::perspective(glm::radians(fovy*zoom), aspect, N, F);

	viewMatrix = glm::lookAt(eye.convertVec3(), eye.convertVec3() + curve->getTangentList().at(pointCurve)->convertVec3(),
		curve->getBinormalList().at(pointCurve)->convertVec3());
}

void Camara::move(float x, float y){
	glm::vec3 displaceNormalAux = curve->getNormalList().at(pointCurve)->convertVec3()*x;
	displaceNormal += displaceNormalAux;
	glm::vec3 displaceBinormalAux = up.convertVec3()*y;
	displaceBinormal += displaceBinormalAux;
	glm::vec3 newEye = eye.convertVec3() + displaceNormalAux + displaceBinormalAux;
	if (glm::distance(newEye, curve->getPointList().at(pointCurve)->convertVec3()) < veinRadius/2){
		eye = PV3D(newEye.x, newEye.y, newEye.z);
	}
}

void Camara::rotate(float roll, float yaw, float pitch){
	glm::mat4x4 identity = glm::mat4x4(1.0f);
	//Roll, look dont change, change the up vector, we rotate with look vector
	glm::mat4x4 rotationMatrix = glm::rotate(identity, glm::radians(roll), look.convertVec3());
	//Yaw, up dont change, change the look vector, we rotate with up vector
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(yaw), up.convertVec3());
	//Pitch, change look y up vector, we rotate with normal vector
	glm::vec3 normal = glm::cross(look.convertVec3(), up.convertVec3());
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(pitch), normal);
	glm::vec4 auxEye = rotationMatrix*glm::vec4(eye.getX(),eye.getY(),eye.getZ(),1);
	glm::vec4 auxLook = rotationMatrix*glm::vec4(look.getX(), look.getY(), look.getZ(), 1);
	eye.setX(auxEye.x); eye.setY(auxEye.y); eye.setZ(auxEye.z);
	look.setX(auxLook.x); look.setY(auxLook.y); look.setZ(auxLook.z);
	/*glm::vec3 roller = glm::rotate(curve->getBinormalList().at(pointCurve)->convertVec3(),glm::radians(roll),look.convertVec3());
	projectionMatrix = glm::perspective(glm::radians(fovy*zoom), aspect, N, F);
	viewMatrix = glm::lookAt(eye.convertVec3(), eye.convertVec3() + curve->getTangentList().at(pointCurve)->convertVec3(),
		roller);*/
}

/* zoom */
void Camara::addZoom(float val){ zoom = ((zoom + val)>MAX_ZOOM) ? MAX_ZOOM : (zoom + val); }
void Camara::deductZoom(float val){ zoom = ((zoom - val)<MIN_ZOOM) ? MIN_ZOOM : (zoom - val); }

/* Return the current point where is the camera*/
int Camara::getCurrentPoint() { return pointCurve; }

void Camara::setMutation(bool b) { mutation = b; }

/* Camara Antigua */

//Camara::Camara(PV3D eye, PV3D look, PV3D up){
//	this->eye = eye;
//	this->look = look;
//	this->up = up;
//	this->pointCurve = 0;
//
//	getCoordCam();
//	fijarCam();
//	ortogonal(-10,10,-10,10,1,1000);
//}
//
//Camara::Camara(PV3D eye, PV3D look, PV3D up, float xRight, float xLeft, float yTop, float yBot, float N, float F, float fovy, float aspect, BezierCurve* c){
//	this->eye = eye;
//	this->look = look;
//	this->up = up;
//	this->xRight = xRight;
//	this->xLeft = xLeft;
//	this->yTop = yTop;
//	this->yBot = yBot;
//	this->N = N;
//	this->F = F;
//	this->fovy = fovy;
//	this->aspect = aspect;
//	this->curve = c;
//	this->pointCurve = 0;
//	this->zoom = 1.0f;
//
//	viewMatrix = glm::lookAt(eye.convertVec3(), eye.convertVec3() + look.convertVec3(), up.convertVec3());
//
//	projectionMatrix = glm::perspective(glm::radians(fovy), aspect, N, F);
//
//	//getCoordCam();
//	//fijarCam();
//	//ortogonal(-10, 10, -10, 10, 1, 1000);
//}
//
//void Camara::moveCamara(PV3D* eye, PV3D* look){
//	this->eye = *eye;
//	this->look = *look;
//}
//
//void Camara::moveCamara(PV3D* eye, PV3D* look, PV3D* up){
//	this->eye = *eye;
//	this->look = *look;
//	this->up = *up;
//}
//
//void Camara::getCoordCam(){
//	// n = (eye-look).normalizar()
//	PV3D* aux = new PV3D(eye.getX()-look.getX(),eye.getY()-look.getY(),eye.getZ()-look.getZ());
//	aux->normalize();
//	n = aux;
//	
//	// u = (up*n).normalizar() 
//	u = up.crossProduct(n);
//	u->normalize();
//	// v = n*u
//	v = n->crossProduct(u);
//}
//
//void Camara::getMatriz(){
//	/*
//	GLfloat* m= new GLfloat[16];
//	m[0] = this->u->getX();
//	m[1] = this->v->getX();
//	m[2] = this->n->getX();
//	m[3] = 0;
//
//	m[4] = this->u->getY();
//	m[5] = this->v->getY();
//	m[6] = this->n->getY();
//	m[7] = 0;
//
//	m[8] = this->u->getZ();
//	m[9] = this->v->getZ();
//	m[10] = this->n->getZ();
//	m[11] = 0;
//
//	m[12] = -(this->eye->scalarProduct(this->u));
//	m[13] = -(this->eye->scalarProduct(this->v));
//	m[14] = -(this->eye->scalarProduct(this->n));
//	m[15] = 0;
//	*/
//	
//	GLfloat m[16] = {
//		this->u->getX(), this->v->getX(), this->n->getX(), 0,
//		this->u->getY(), this->v->getY(), this->n->getY(), 0,
//		this->u->getZ(), this->v->getZ(), this->n->getZ(), 0,
//		-(this->eye.scalarProduct(this->u)), -(this->eye.scalarProduct(this->v)), -(this->eye.scalarProduct(this->n)), 1};
//	glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glLoadMatrixf(m);
//
//	//delete[] m;
//}
//
//void Camara::fijarCam(){
//  glMatrixMode(GL_MODELVIEW);
//  glLoadIdentity();
//  //getMatriz();
//  gluLookAt(eye.getX(), eye.getY(), eye.getZ(), look.getX(), look.getY(), look.getZ(), up.getX(), up.getY(), up.getZ());
//}
//
//void Camara::roll(GLdouble alfa){
//	getCoordCam();
//
//	PV3D* aux = u->clone();
//
//	u->setX((aux->getX()*cos(alfa)) - (v->getX()*sin(alfa)));
//	u->setY((aux->getY()*cos(alfa)) - (v->getY()*sin(alfa)));
//	u->setZ((aux->getZ()*cos(alfa)) - (v->getZ()*sin(alfa)));
//
//	v->setX((aux->getX())*sin(alfa) + (v->getX()*cos(alfa)));
//	v->setY((aux->getY())*sin(alfa) + (v->getY()*cos(alfa)));
//	v->setZ((aux->getZ())*sin(alfa) + (v->getZ()*cos(alfa)));
//
//	getMatriz();
//}
//
//void Camara::yaw(GLdouble alfa){
//	getCoordCam();
//	PV3D* aux = u->clone();
//
//	u->setX((aux->getX()*cos(alfa)) + (n->getX()*sin(alfa)));
//	u->setY((aux->getY()*cos(alfa)) + (n->getY()*sin(alfa)));
//	u->setZ((aux->getZ()*cos(alfa)) + (n->getZ()*sin(alfa)));
//
//	n->setX(-((aux->getX())*sin(alfa)) + (n->getX()*cos(alfa)));
//	n->setY(-((aux->getY())*sin(alfa)) + (n->getY()*cos(alfa)));
//	n->setZ(-((aux->getZ())*sin(alfa)) + (n->getZ()*cos(alfa)));
//
//	getMatriz();
//}
//
//void Camara::pitch(GLdouble alfa){
//	getCoordCam();
//	PV3D* aux = n->clone();
//
//	n->setX((aux->getX()*cos(alfa)) + (v->getX()*sin(alfa)));
//	n->setY((aux->getY()*cos(alfa)) + (v->getY()*sin(alfa)));
//	n->setZ((aux->getZ()*cos(alfa)) + (v->getZ()*sin(alfa)));
//
//	v->setX(-((aux->getX())*sin(alfa)) + (v->getX()*cos(alfa)));
//	v->setY(-((aux->getY())*sin(alfa)) + (v->getY()*cos(alfa)));
//	v->setZ(-((aux->getZ())*sin(alfa)) + (v->getZ()*cos(alfa)));
//
//	getMatriz();
//}
//
//void Camara::desplazar(GLdouble x, GLdouble y, GLdouble z){
//	this->eye.setX(this->eye.getX()+x);
//	this->eye.setY(this->eye.getY()+y);
//	this->eye.setZ(this->eye.getZ()+z);
//
//	this->look.setX(this->look.getX()+x);
//	this->look.setY(this->look.getY()+y);
//	this->look.setZ(this->look.getZ()+z);
//
//	this->fijarCam();
//	//this->ortogonal(-10,10,-10,10,1,1000);
//}
//
//void Camara::ortogonal(){
//	glMatrixMode(GL_PROJECTION);
//	//glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glOrtho(xLeft, xRight, yBot, yTop, N, F);
//}
//
//void Camara::ortogonal(GLdouble left, GLdouble right, GLdouble botton, GLdouble top, GLdouble n, GLdouble f){
//	glMatrixMode(GL_PROJECTION);
//	//glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glOrtho(left, right, botton, top, n, f);
//}
//
//void Camara::perspectiva(GLdouble left, GLdouble right, GLdouble botton, GLdouble top, GLdouble n, GLdouble f){
//	glMatrixMode(GL_PROJECTION);
//	//glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	//glFrustum(left,right,bottom,top,nearAux,farAux);
//	gluPerspective(5, 2, n, f);
//}
//
//void Camara::oblicua(PV3D* vector, GLdouble left, GLdouble right, GLdouble botton, GLdouble top, GLdouble n, GLdouble f){
//	glMatrixMode(GL_PROJECTION);
//	//glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	
//	vector->normalize();
//
//	if(vector->getZ() != 0.0 && (vector->getX() != 0 || vector->getY() != 0 || vector->getZ() != 1)){
//		GLfloat m[16] = {	
//			1, 0, 0, 0,
//			0, 1, 0, 0,
//			0, 0, 1, 0,
//			0, 0, 0, 1
//		};
//		m[8] = -(vector->getX()) / vector->getZ();
//		m[9] = -(vector->getY()) / vector->getZ();
//		m[12] = -n * (vector->getX() / vector->getZ());
//		m[13] = -n * (vector->getY() / vector->getZ());
//
//		glOrtho(left, right, botton, top, n, f);
//		glMultMatrixf(m);
//		
//	}
//}
//
//void Camara::recorridoEje(GLdouble x, GLdouble y, GLdouble z){
//	this->eye.setX(this->eye.getX()+x);
//	this->eye.setY(this->eye.getY()+y);
//	this->eye.setZ(this->eye.getZ()+z);
//	
//	this->fijarCam();
//	this->ortogonal(-10,10,-10,10,1,1000);
//}
//
//void Camara::giraX(GLdouble alfa){
//	GLdouble angle2 = atan2(eye.getZ(), eye.getY()) + alfa;
//	GLdouble dst = sqrt((eye.getY() * eye.getY()) + (eye.getZ() * eye.getZ()));
//
//	eye.setY(dst * cos(angle2));
//	eye.setZ(dst * sin(angle2));
//
//	this->fijarCam();
//	this->ortogonal(-10,10,-10,10,1,1000);
//}
//
//void Camara::giraY(GLdouble alfa){
//	GLdouble angle2 = atan2(eye.getZ(), eye.getX()) + alfa;
//	GLdouble dst = sqrt((eye.getX() * eye.getX()) + (eye.getZ() * eye.getZ()));
//
//	eye.setX(dst * cos(angle2));
//	eye.setZ(dst * sin(angle2));
//
//	this->fijarCam();
//	this->ortogonal(-10,10,-10,10,1,1000);
//}
//
//void Camara::giraZ(GLdouble alfa){
//	GLdouble angle2 = atan2(eye.getY(), eye.getX()) + alfa;
//	GLdouble dst = sqrt((eye.getY() * eye.getY()) + (eye.getX() * eye.getX()));
//
//	eye.setX(dst * cos(angle2));
//	eye.setY(dst * sin(angle2));
//
//	this->fijarCam();
//}
//
//void Camara::lateral(){
//	this->eye.setX(100);
//	this->eye.setY(0);
//	this->eye.setZ(0);
//
//	this->up.setX(0);
//	this->up.setY(1);
//	this->up.setZ(0);
//
//	this->fijarCam();
//	this->ortogonal(-10,10,-10,10,1,1000);
//}
//
//void Camara::frontal(){
//	this->eye.setX(0);
//	this->eye.setY(0);
//	this->eye.setZ(100);
//
//	this->up.setX(0);
//	this->up.setY(1);
//	this->up.setZ(0);
//
//	this->fijarCam();
//	this->ortogonal(-10,10,-10,10,1,1000);
//}
//
//void Camara::cenital(){
//	this->eye.setX(0);
//	this->eye.setY(100);
//	this->eye.setZ(0);
//
//	this->up.setX(1);
//	this->up.setY(0);
//	this->up.setZ(0);
//
//	this->fijarCam();
//	this->ortogonal(-10,10,-10,10,1,1000);
//}
//
//void Camara::esquina(){
//	this->eye.setX(100);
//	this->eye.setY(100);
//	this->eye.setZ(100);
//
//	this->up.setX(0);
//	this->up.setY(1);
//	this->up.setZ(0);
//
//	this->fijarCam();
//	this->ortogonal(-10,10,-10,10,1,1000);
//}