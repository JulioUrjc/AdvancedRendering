#include "Poligon.h"

Poligon::Poligon(){
	center = NULL;
	vertex = NULL;
}
Poligon::Poligon(PV3D *c, int n, GLdouble radius){
	this->center = c;
	this->n_sides = n;
	this->radius = radius;
	this->calcula_vertices(this->center);
}
Poligon::~Poligon(){
	delete vertex;
}
//------------------------------------------------------------------
void Poligon::calcula_vertices(PV3D* centroTemp){
	Compass *pencil = new Compass(centroTemp->clone(), 0);

	//CaLculo del angulo en el que se divide la circunferencia
	GLdouble alfa = 2*M_PI/n_sides;
	//Calculate edge size
	GLdouble tam = (radius*sin(alfa / 2)) * 2;
	//Calculate angle rotation of the pencil
	GLdouble beta = (M_PI-alfa) / 2;
	//Distance pencil to r
	pencil->forward(radius);
	pencil->turn(M_PI-beta);

	PV3D *aux_p;

	//Poligon's Vertex;
	this->vertex = new vector<PV3D*>();
	for (int i = 0; i<n_sides; i++){
		aux_p = pencil->getPos()->clone();
		vertex->push_back(aux_p);
		pencil->forward(tam);
		pencil->turn(M_PI - 2 * beta);
	}
	aux_p = NULL;
}

//------------------------------------------------------------------
void Poligon::setVertex(vector<PV3D*> *v){
	delete this->vertex;
	this->vertex = v;
}
void Poligon::setCenter(PV3D *p){
	delete this->center;
	this->center = p;
}
void Poligon::setRadius(GLdouble r){
	this->radius = r;
}

//------------------------------------------------------------------
vector<PV3D*>* Poligon::getVertex(void){
	return this->vertex;
}
PV3D* Poligon::getCenter(void){
	return this->center;
}
GLdouble Poligon::getRadius(void){
	return this->radius;
}
