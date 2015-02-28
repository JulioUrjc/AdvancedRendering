#include "Vein.h"


#include <vector>
#include <glm\gtx\transform.hpp>
#include <cstdlib>


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

		for (int j = 0; j<NP; j++){									// Esto ocurre con cada uno de los v�rtices del pol�gono
			int numV = NP*i + j;
			PV3D* clon = puntos->at(j)->clone();					// Un clon del punto del pol�gono para trabajar
			PV3D* punto = clon->matrixProduct(Nt, Bt, Tt, Ct);      // Transformacion del poligono al sistema de referencia local del punto
			vertex->at(numV) = punto;								// El punto recibe un identificador y siempre con sentido
			delete clon;
		}

		//deletes de los objetos ya no necesarios
		delete Tt; 	delete Bt;	delete Nt;	delete Ct;
	}

	// Se construyen las Faces
	for (int numFace = 0; numFace< faces->size(); numFace++){      //  |>Recorremos todas las Faces en orden
		faces->at(numFace) = new Face(4);
		vector<VertexNormal*>* auxNormals = new vector<VertexNormal*>(4);

		int a = (numFace) % (NP*curve->nPoints());
		int b = (nextVertex(numFace)) % (NP*curve->nPoints());		// Teniendo cuidado de cerrar bien el c�rculo
		int c = (nextVertex(numFace) + NP) % (NP*curve->nPoints());
		int d = (numFace + NP) % (NP*curve->nPoints());

		auxNormals->at(0) = new VertexNormal(a, numFace);
		auxNormals->at(1) = new VertexNormal(b, numFace);
		auxNormals->at(2) = new VertexNormal(c, numFace);
		auxNormals->at(3) = new VertexNormal(d, numFace);

		faces->at(numFace)->setIndicesVN(auxNormals);
	}
	// Se hacen las normales

	/*for (int i = 0; i<numFaces; i++){
		normals->at(i) = doVectorNormalNewell(faces->at(i));
	}*/

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
void Vein::draw(bool relleno,Camara* camara, int point){

	Mesh::draw(relleno);  // Dibuja la Mesh
	

	PV3D* eye = new PV3D(curve->getPointList().at(point)->getX(), curve->getPointList().at(point)->getY(), curve->getPointList().at(point)->getZ());
	PV3D* look = new PV3D(curve->getTangentList().at(point)->getX(), curve->getTangentList().at(point)->getY(), curve->getTangentList().at(point)->getZ());
	PV3D* up = new PV3D(curve->getBinormalList().at(point)->getX(), curve->getBinormalList().at(point)->getY(), curve->getBinormalList().at(point)->getZ());
	/*PV3D* eye = new PV3D(-3.0,0.0,2.0);
	PV3D* look = new PV3D(-1.0, 0.0, 0.0);
	PV3D* up = new PV3D(0.0, 1.0, 0.0);*/
	camara->moveCamara(eye, look, up);
	camara->fijarCam();
}
