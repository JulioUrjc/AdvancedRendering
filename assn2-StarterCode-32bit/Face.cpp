#include "Face.h"

Face::Face(int nV){
	numV = nV;
	indicesVN = new vector<VertexNormal*>(numV);
}

int Face::getNumV(){
	return numV;
}

int Face::getNormalIndex(int i){
	return indicesVN->at(i)->getNormal();
}

int Face::getVertexIndex(int i){
	return indicesVN->at(i)->getVertex();
}

void Face::setIndicesVN(vector<VertexNormal*> *array){
	this->indicesVN = array;
}

Face::~Face(){
	delete indicesVN;
}
