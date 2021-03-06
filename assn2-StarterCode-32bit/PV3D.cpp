#include "PV3D.h"

PV3D::PV3D(){
	corX = 0;
	corY = 0;
	corZ = 0;
}

PV3D::PV3D(GLdouble x, GLdouble y, GLdouble z){
	corX = x;
	corY = y;
	corZ = z;
	pv = pv;
}
PV3D::~PV3D(){}

//--------------------------------------------------------------
GLdouble PV3D::getX(){
	return corX;
}
GLdouble PV3D::getY(){
	return corY;
}
GLdouble PV3D::getZ(){
	return corZ;
}

//--------------------------------------------------------------
void PV3D::setX(GLdouble x){
	this->corX = x;
}
void PV3D::setY(GLdouble y){
	this->corY = y;
}
void PV3D::setZ(GLdouble z){
	this->corZ = z;
}

//--------------------------------------------------------------
int PV3D::getPv(){
	return this->pv;
}
void PV3D::setPv(int val){
	this->pv = val;
}

//--------------------------------------------------------------
PV3D* PV3D::clone(){
	return new PV3D(this->corX, this->corY, this->corZ);
}
PV3D* PV3D::addition(PV3D* p){
	return new PV3D(this->corX + p->getX(), this->corY + p->getY(), this->corZ + p->getZ());
}
PV3D* PV3D::addition(float f){
	return new PV3D(this->corX + f, this->corY + f, this->corZ + f);
}
PV3D* PV3D::subtraction(PV3D* p){
	return new PV3D(this->corX - p->getX(), this->corY - p->getY(), this->corZ - p->getZ());
}
PV3D* PV3D::factor(float f){
	return new PV3D(f*this->corX, f*this->corY, f*this->corZ);
}

//--------------------------------------------------------------
void PV3D::normalize(){
	GLdouble magnitude = this->magnitude();
	this->corX = this->corX / magnitude;
	this->corY = this->corY / magnitude;
	this->corZ = this->corZ / magnitude;
}
GLdouble PV3D::magnitude(){
	GLdouble aux = pow(this->corX, 2) + pow(this->corY, 2) + pow(this->corZ, 2);
	return sqrt(aux);
}
PV3D* PV3D::crossProduct(PV3D* v){
	GLdouble factori = (corY*v->getZ())-(corZ*v->getY());
	GLdouble factorj = (corZ*v->getX())-(corX*v->getZ());
	GLdouble factork = (corX*v->getY())-(corY*v->getX());

	return new PV3D(factori, factorj, factork);
}

GLdouble PV3D::scalarProduct(PV3D* v){
	return this->corX*v->getX() + this->corY*v->getY() + this->corZ*v->getZ();
}

PV3D* PV3D::matrixProduct(PV3D* Nt, PV3D* Bt, PV3D* Tt, PV3D* Ct){

	GLdouble valx = Nt->getX()*corX + Bt->getX()*corY + Tt->getX()*corZ + Ct->getX();
	GLdouble valy = Nt->getY()*corX + Bt->getY()*corY + Tt->getY()*corZ + Ct->getY();
	GLdouble valz = Nt->getZ()*corX + Bt->getZ()*corY + Tt->getZ()*corZ + Ct->getZ();
	return new PV3D(valx, valy, valz);
}

//-------------------------------------------------------------

void PV3D::setColor(PV3D* color){
	colR = color->getX();
	colG = color->getY();
	colB = color->getZ();
}

PV3D* PV3D::getColor(){
	return new PV3D(colR, colG, colB);
}

//-------------------------------------------------------------

void PV3D::toString(){
	std::cout << corX << " " << corY << " " << corZ << std::endl;
}

glm::vec3 PV3D::convertVec3(){
	glm::vec3 vector3 = glm::vec3(corX, corY, corZ);
	return  vector3;
}