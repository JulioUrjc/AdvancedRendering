#include "VertexNormal.h"

VertexNormal::VertexNormal(int v, int n){
	indexV = v;
	indexN = n;
}

int VertexNormal::getNormal(){
	return indexN;
}

int VertexNormal::getVertex(){
	return indexV;
}

VertexNormal::~VertexNormal(void){}
