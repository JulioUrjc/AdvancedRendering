#include "Point3D.h"

Point3D::Point3D(){
	corX = 0;
	corY = 0;
	corZ = 0;
}

Point3D::Point3D(GLdouble x, GLdouble y, GLdouble z){
	corX = x;
	corY = y;
	corZ = z;
}
Point3D::~Point3D(){}

//--------------------------------------------------------------
GLdouble Point3D::getX(){
	return corX;
}
GLdouble Point3D::getY(){
	return corY;
}
GLdouble Point3D::getZ(){
	return corZ;
}

//--------------------------------------------------------------
void Point3D::setX(GLdouble x){
	this->corX = x;
}
void Point3D::setY(GLdouble y){
	this->corY = y;
}
void Point3D::setZ(GLdouble z){
	this->corZ = z;
}

//--------------------------------------------------------------
int Point3D::getPv(){
	return this->pv;
}
void Point3D::setPv(int val){
	this->pv = val;
}

//--------------------------------------------------------------
Point3D* Point3D::clone(){
	return new Point3D(this->corX, this->corY, this->corZ);
}
Point3D* Point3D::addition(Point3D* p){
	return new Point3D(this->corX + p->getX(), this->corY + p->getY(), this->corZ + p->getZ());
}
Point3D* Point3D::subtraction(Point3D* p){
	return new Point3D(this->corX - p->getX(), this->corY - p->getY(), this->corZ - p->getZ());
}

//--------------------------------------------------------------
void Point3D::normalize(){
	GLdouble magnitude = this->magnitude();
	this->corX = this->corX / magnitude;
	this->corY = this->corY / magnitude;
	this->corZ = this->corZ / magnitude;
}
GLdouble Point3D::magnitude(){
	GLdouble aux = pow(this->corX, 2) + pow(this->corY, 2) + pow(this->corZ, 2);
	return sqrt(aux);
}
Point3D* Point3D::crossProduct(Point3D* v){
	GLdouble factori = (corY*v->getZ())-(corZ*v->getY());
	GLdouble factorj = (corZ*v->getX())-(corX*v->getZ());
	GLdouble factork = (corX*v->getY())-(corY*v->getX());

	return new Point3D(factori, factorj, factork);
}

Point3D* Point3D::matrixProduct(Point3D* Nt, Point3D* Bt, Point3D* Tt, Point3D* Ct){

	GLdouble valx = Nt->getX()*corX + Bt->getX()*corY + Tt->getX()*corZ + Ct->getX();
	GLdouble valy = Nt->getY()*corX + Bt->getY()*corY + Tt->getY()*corZ + Ct->getY();
	GLdouble valz = Nt->getZ()*corX + Bt->getZ()*corY + Tt->getZ()*corZ + Ct->getZ();
	return new Point3D(valx, valy, valz);
}
