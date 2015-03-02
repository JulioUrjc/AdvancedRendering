#include "Vein.h"


#include <vector>
#include <glm\gtx\transform.hpp>
#include <cstdlib>
#include <amp.h>



//---------------------------------------------------------------------------

Vein::Vein(int NP, GLfloat radius, BezierCurve* curve) : Mesh(NP*curve->nPoints(), NP*curve->nPoints(), NP*curve->nPoints()){
	this->NP = NP;
	this->radius = radius;
	this->curve = curve;

	build();
}

Vein::~Vein(){}

//-------------------------------------------------------------------------
void Vein::build(){

	Poligon *poli = new Poligon(new PV3D(), NP, radius);                     // Poligono en el origen para desplazarlo al sistema de referencia
	vector<PV3D*>* puntos = poli->getVertex();                               // local a cada punto de la curva.

	for (int i = 0; i< curve->nPoints(); i++){								 // Esto ocurre en cada "secci�n" de la vena

		PV3D* Tt = curve->getTangentList().at(i)->clone();           // Normalize Tangent in Point
		PV3D* Bt = curve->getBinormalList().at(i)->clone();          // Binormal
		PV3D* Nt = curve->getNormalList().at(i)->clone();
		PV3D* Ct = curve->getPointList().at(i)->clone();             //Center Point with n steap
		//Tt->toString();
		//Bt->toString();
		//Nt->toString();

		for (int j = 0; j<NP; j++){			// Esto ocurre con cada uno de los v�rtices del pol�gono
			// Se construyen los vertices
			int numV = NP*i + j;
			PV3D* clon = puntos->at(j)->clone();					// Un clon del punto del pol�gono para trabajar
			PV3D* punto = clon->matrixProduct(Nt, Bt, Tt, Ct);      // Transformacion del poligono al sistema de referencia local del punto
			vertex->at(numV) = punto;								// El punto recibe un identificador y siempre con sentido

			delete clon;

			// Se construyen las caras
			// Caras como cuadrados
			int numFace = NP*(i)+j;
			faces->at(numFace) = new Face(4);
			vector<VertexNormal*>* auxNormals = new vector<VertexNormal*>(4);

			int verticeBase = numFace;
			int a = (verticeBase) % (NP*curve->nPoints());
			int b = (nextVertex(verticeBase)) % (NP*curve->nPoints());
			int c = (nextVertex(verticeBase) + NP) % (NP*curve->nPoints());
			int d = (verticeBase + NP) % (NP*curve->nPoints());

			auxNormals->at(0) = new VertexNormal(a, numFace);
			auxNormals->at(1) = new VertexNormal(b, numFace);
			auxNormals->at(2) = new VertexNormal(c, numFace);
			auxNormals->at(3) = new VertexNormal(d, numFace);

			faces->at(numFace)->setIndicesVN(auxNormals);



		}

		//deletes de los objetos ya no necesarios
		delete Tt; 	delete Bt;	delete Nt;	delete Ct;
	}

	for (int i = 0; i<faces->size(); i++){
		normals->at(i) = doVectorNormalNewell(faces->at(i));
	}

	delete poli;
}

//-------------------------------------------------------------------------
int Vein::nextVertex(int v){
	int vAux = v + 1;
	if (vAux%NP == 0){
		return vAux - NP;
	}
	return vAux;
}
//-------------------------------------------------------------------------

void Vein::draw(int modo){

	Mesh::draw(modo);     // Draw Mesh whit point, alambric or face mode
}

void Vein::draw(bool relleno){

	Mesh::draw(relleno);  // Dibuja la Mesh	
}

void Vein::addPerlinNoise(float** perlinNoise){
	float deformation=0;
	float maxDef = 0.3f;
	float minDef = 0.01f;
	for (int i = 0; i < curve->nPoints(); i++){
		//I'm in each subdivision
		for (int j = 0; j < NP; j++){
			//I'm in each vertex at subdivision
			deformation = ((perlinNoise[i%256][j] + 1) / 2)*(maxDef - minDef) + minDef;
			int iN = faces->at(i)->getNormalIndex(j%4);
			GLfloat nX = normals->at(iN)->getX();
			GLfloat nY = normals->at(iN)->getY();
			GLfloat nZ = normals->at(iN)->getZ();
			int iV = faces->at(i)->getVertexIndex(j%4);
			vertex->at(iV)->setX(vertex->at(iV)->getX() + glm::clamp(nX*deformation, 0.0f, 0.3f));
			vertex->at(iV)->setY(vertex->at(iV)->getY() + glm::clamp(nY*deformation, 0.0f, 0.3f));
			vertex->at(iV)->setZ(vertex->at(iV)->getZ() + glm::clamp(nZ*deformation, 0.0f, 0.3f));
			
			vertex->at(iV)->setColor(new PV3D(glm::clamp(deformation, 0.0f, 0.8f), glm::clamp(deformation, 0.0f, 0.5f), 0.0f));

		}
	}
}
