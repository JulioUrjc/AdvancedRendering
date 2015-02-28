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

	for (int i = 0; i< curve->nPoints(); i++){								 // Esto ocurre en cada "sección" de la vena

		PV3D* Tt = curve->getTangentList().at(i)->clone();           // Normalize Tangent in Point
		PV3D* Bt = curve->getBinormalList().at(i)->clone();          // Binormal
		PV3D* Nt = curve->getNormalList().at(i)->clone();
		PV3D* Ct = curve->getPointList().at(i)->clone();             //Center Point with n steap

		for (int j = 0; j<NP; j++){									// Esto ocurre con cada uno de los vértices del polígono
			int numV = NP*i + j;
			PV3D* clon = puntos->at(j)->clone();					// Un clon del punto del polígono para trabajar
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
		int b = (nextVertex(numFace)) % (NP*curve->nPoints());		// Teniendo cuidado de cerrar bien el círculo
		int c = (nextVertex(numFace) + NP) % (NP*curve->nPoints());
		int d = (numFace + NP) % (NP*curve->nPoints());

		auxNormals->at(0) = new VertexNormal(a, numFace);
		auxNormals->at(1) = new VertexNormal(b, numFace);
		auxNormals->at(2) = new VertexNormal(c, numFace);
		auxNormals->at(3) = new VertexNormal(d, numFace);

		faces->at(numFace)->setIndicesVN(auxNormals);
	}
	// Se hacen las normales

	for (int i = 0; i<numFaces; i++){
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
void Vein::draw(bool relleno,int point){

	Mesh::draw(relleno);  // Dibuja la Mesh
	
	float eyeX = curve->getPointList().at(point)->getX();
	float eyeY = curve->getPointList().at(point)->getY();
	float eyeZ = curve->getPointList().at(point)->getZ();

	float lookX = curve->getTangentList().at(point)->getX();
	float lookY = curve->getTangentList().at(point)->getY();
	float lookZ = curve->getTangentList().at(point)->getZ();

	float upX = curve->getBinormalList().at(point)->getX();
	float upY = curve->getBinormalList().at(point)->getY();
	float upZ = curve->getBinormalList().at(point)->getZ();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);

	//PV3D* fderivate = fDerivate(carPos);
	//PV3D* sderivate = sDerivate(carPos);

	//PV3D* Tt = fDerivate(carPos); Tt->normalize();					// Tt = C'
	//PV3D* Bt = fderivate->crossProduct(sderivate); Bt->normalize();	// Bt = C'.C''
	//PV3D* Nt = Bt->crossProduct(Tt);									// Nt = Bt.Tt
	//PV3D* Ct = function(carPos);										// Ct = C

	//GLfloat m[] = { Nt->getX(), Nt->getY(), Nt->getZ(), 0,		// Se niega Bt porque al ser 
	//	-Bt->getX(), -Bt->getY(), -Bt->getZ(), 0,		        // un producto escalar es perpendicular 
	//	Tt->getX(), Tt->getY(), Tt->getZ(), 0,		            // al plano definido por C'.C'' 
	//	Ct->getX(), Ct->getY(), Ct->getZ(), 1 };		        // pero en sentido contrario

	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();

	//glMultMatrixf(m);
	//glPopMatrix();

	////deletes de los objetos ya no necesarios
	//delete sderivate;	delete fderivate;	delete Tt;
	//delete Bt;			delete Nt;			delete Ct;
}

void Vein::addPerlinNoise(float** perlinNoise)
{
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
			vertex->at(iV)->setX(nX*deformation);
			vertex->at(iV)->setY(nY*deformation);
			vertex->at(iV)->setZ(nZ*deformation);
			vertex->at(iV)->setColor(new PV3D(glm::clamp(deformation, 0.0f, 0.8f), glm::clamp(deformation, 0.0f, 0.5f), 0.0f));

		}
	}
}
