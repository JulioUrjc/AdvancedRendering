#include "Vein.h"

#include <vector>
#include <glm\gtx\transform.hpp>

#include <cstdlib>


//---------------------------------------------------------------------------
//Vein::Vein(int NP, GLfloat radius, BezierCurve* curve) : Mesh(NP*curve->nPoints(), NP*curve->nPoints(), NP*curve->nPoints()){
//	this->NP = NP;
//	this->radius = radius;
//	this->curve = curve;
//
//	build();
//}

Vein::Vein(int NP, GLfloat radius, BezierCurve* curve) : Mesh(NP*curve->size(), NP*curve->size(), NP*curve->size()){
	this->NP = NP;
	this->radius = radius;
	this->curve = curve;

	build();
}

Vein::~Vein(){}

void Vein::build(){}
void Vein::draw(bool t){}
//-------------------------------------------------------------------------
//void Vein::build(){
//
//	Poligon *poli = new Poligon(new PV3D(), NP, radius);                     // Poligono en el origen para desplazarlo al sistema de referencia
//	vector<PV3D*>* puntos = poli->getVertex();                               // local a cada punto de la curva.
//
//	for (int i = 0; i< curve->nPoints(); i++){								 // Esto ocurre en cada "sección" de la vena
//
//		PV3D* Tt = curve->getTangentList().at(i);           // Normalize Tangent in Point
//		PV3D* Bt = curve->getBinormalList().at(i);          // Binormal
//		PV3D* Nt = curve->getNormalList().at(i);
//		PV3D* Ct = curve->getPointList().at(i);             //Center Point with n steap
//
//		for (int j = 0; j<NP; j++){									// Esto ocurre con cada uno de los vértices del polígono
//			int numV = NP*i + j;
//			PV3D* clon = puntos->at(j)->clone();					// Un clon del punto del polígono para trabajar
//			PV3D* punto = clon->matrixProduct(Nt, Bt, Tt, Ct);      // Transformacion del poligono al sistema de referencia local del punto
//			vertex->at(numV) = punto;								// El punto recibe un identificador y siempre con sentido
//			delete clon;
//		}
//
//		//deletes de los objetos ya no necesarios
//		delete Tt; 	delete Bt;	delete Nt;	delete Ct;
//	}
//
//	// Se construyen las Faces
//	for (int numFace = 0; numFace< faces->size(); numFace++){      //  |>Recorremos todas las Faces en orden
//		faces->at(numFace) = new Face(4);
//		vector<VertexNormal*>* auxNormals = new vector<VertexNormal*>(4);
//
//		int a = (numFace) % (NP*curve->nPoints());
//		int b = (nextVertex(numFace)) % (NP*curve->nPoints());		// Teniendo cuidado de cerrar bien el círculo
//		int c = (nextVertex(numFace) + NP) % (NP*curve->nPoints());
//		int d = (numFace + NP) % (NP*curve->nPoints());
//
//		auxNormals->at(0) = new VertexNormal(a, numFace);
//		auxNormals->at(1) = new VertexNormal(b, numFace);
//		auxNormals->at(2) = new VertexNormal(c, numFace);
//		auxNormals->at(3) = new VertexNormal(d, numFace);
//
//		faces->at(numFace)->setIndicesVN(auxNormals);
//	}
//	// Se hacen las normales
//
//	/*for (int i = 0; i<numFaces; i++){
//		normals->at(i) = doVectorNormalNewell(faces->at(i));
//	}*/
//
//	delete poli;
//}
//void Vein::build(){
//
//	Poligon *poli = new Poligon(new PV3D(), NP, radius);                     // Poligono en el origen para desplazarlo al sistema de referencia
//	vector<PV3D*>* puntos = poli->getVertex();                               // local a cada punto de la curva.
//
//	for (int i = 0; i< curve->size(); i++){								 // Esto ocurre en cada "sección" de la vena
//
//		glm::vec3 Tt = curve->getTangentList().at(i);           // Normalize Tangent in Point
//		glm::vec3 Bt = curve->getBinormalList().at(i);          // Binormal
//		glm::vec3 Nt = curve->getNormalList().at(i);
//		glm::vec3 Ct = curve->getPointList().at(i);             //Center Point with n steap
//
//		for (int j = 0; j<NP; j++){									// Esto ocurre con cada uno de los vértices del polígono
//			int numV = NP*i + j;
//			PV3D* clon = puntos->at(j)->clone();					// Un clon del punto del polígono para trabajar
//			PV3D* punto = clon->matrixProduct(Nt, Bt, Tt, Ct);      // Transformacion del poligono al sistema de referencia local del punto
//			vertex->at(numV) = punto;								// El punto recibe un identificador y siempre con sentido
//			delete clon;
//		}
//
//		//deletes de los objetos ya no necesarios
//		delete Tt; 	delete Bt;	delete Nt;	delete Ct;
//	}
//
//	// Se construyen las Faces
//	for (int numFace = 0; numFace< faces->size(); numFace++){      //  |>Recorremos todas las Faces en orden
//		faces->at(numFace) = new Face(4);
//		vector<VertexNormal*>* auxNormals = new vector<VertexNormal*>(4);
//
//		int a = (numFace) % (NP*curve->nPoints());
//		int b = (nextVertex(numFace)) % (NP*curve->nPoints());		// Teniendo cuidado de cerrar bien el círculo
//		int c = (nextVertex(numFace) + NP) % (NP*curve->nPoints());
//		int d = (numFace + NP) % (NP*curve->nPoints());
//
//		auxNormals->at(0) = new VertexNormal(a, numFace);
//		auxNormals->at(1) = new VertexNormal(b, numFace);
//		auxNormals->at(2) = new VertexNormal(c, numFace);
//		auxNormals->at(3) = new VertexNormal(d, numFace);
//
//		faces->at(numFace)->setIndicesVN(auxNormals);
//	}
//	// Se hacen las normales
//
//	/*for (int i = 0; i<numFaces; i++){
//	normals->at(i) = doVectorNormalNewell(faces->at(i));
//	}*/
//
//	delete poli;
//}
//-------------------------------------------------------------------------
int Vein::nextVertex(int v){
	int vAux = v + 1;
	if (vAux%NP == 0){
		return vAux - NP;
	}
	return vAux;
}
//-------------------------------------------------------------------------
//void Vein::draw(bool relleno){
//
//	Mesh::draw(relleno);  // Dibuja la Mesh
//
//	//PV3D* fderivate = fDerivate(carPos);
//	//PV3D* sderivate = sDerivate(carPos);
//
//	//PV3D* Tt = fDerivate(carPos); Tt->normalize();					// Tt = C'
//	//PV3D* Bt = fderivate->crossProduct(sderivate); Bt->normalize();	// Bt = C'.C''
//	//PV3D* Nt = Bt->crossProduct(Tt);									// Nt = Bt.Tt
//	//PV3D* Ct = function(carPos);										// Ct = C
//
//	//GLfloat m[] = { Nt->getX(), Nt->getY(), Nt->getZ(), 0,		// Se niega Bt porque al ser 
//	//	-Bt->getX(), -Bt->getY(), -Bt->getZ(), 0,		        // un producto escalar es perpendicular 
//	//	Tt->getX(), Tt->getY(), Tt->getZ(), 0,		            // al plano definido por C'.C'' 
//	//	Ct->getX(), Ct->getY(), Ct->getZ(), 1 };		        // pero en sentido contrario
//
//	//glMatrixMode(GL_MODELVIEW);
//	//glPushMatrix();
//
//	//glMultMatrixf(m);
//	//glPopMatrix();
//
//	////deletes de los objetos ya no necesarios
//	//delete sderivate;	delete fderivate;	delete Tt;
//	//delete Bt;			delete Nt;			delete Ct;
//}