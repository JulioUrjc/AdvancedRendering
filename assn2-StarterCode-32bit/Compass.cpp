#include "Compass.h"

Compass::Compass(){
	pos = new PV3D();
	dir = 0;
}
Compass::Compass(PV3D *p, GLdouble d){
	pos = p;
	dir = d;
}
Compass::~Compass(){
	delete pos;
}
//-------------------------------------------------------
void Compass::setPos(PV3D *p){
	pos = p;
}
void Compass::setDir(GLdouble d){
	dir = d;
}
//-------------------------------------------------------
GLdouble Compass::getDir(){
	return dir;
}
PV3D* Compass::getPos(){
	return pos;
}
//-------------------------------------------------------
void Compass::turn(GLdouble d){
	GLdouble aux = dir + d;
	//aux = aux % 360;
	dir = aux;
}

void Compass::forward(GLdouble d){
	//Seno(a)=opuesto/hipotenusa => opuesto = seno(a)*hipotenusa
	GLdouble y = pos->getY() + (sin(dir)*d);
	//Coseno(a)=contiguo/hipotenusa => contiguo = coseno(a)*hipotenusa
	GLdouble x = pos->getX() + (cos(dir)*d);

	pos->setX(x);
	pos->setY(y);
}