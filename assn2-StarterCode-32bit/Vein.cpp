#include "Vein.h"

//---------------------------------------------------------------------------
Vein::Vein(int NP, int NQ, GLfloat radius) : Mesh(NP*NQ, NP*NQ, NP*NQ){
	this->NP = NP;
	this->NQ = NQ;
	this->radius = radius;
}

Vein::~Vein(){}

//--------------------------------------------------------------------------
GLfloat Vein::fDerivateX(GLfloat t){ return -3 * sin(t); }
GLfloat Vein::fDerivateY(GLfloat t){ return -3 * sin(1.5*t); }
GLfloat Vein::fDerivateZ(GLfloat t){ return 3 * cos(t); }
//-----------------------------------------
GLfloat Vein::sDerivateX(GLfloat t){ return -3 * cos(t); }
GLfloat Vein::sDerivateY(GLfloat t){ return -4.5*cos(1.5*t); }
GLfloat Vein::sDerivateZ(GLfloat t){ return -3 * sin(t); }
//-------------------------------------------------------------------------
GLfloat Vein::functionX(GLfloat t){ return 3 * cos(t); }
GLfloat Vein::functionY(GLfloat t){ return 2 * cos(t*1.5); }
GLfloat Vein::functionZ(GLfloat t){ return 3 * sin(t); }
//-------------------------------------------------------------------------
Point3D* Vein::function(GLfloat t){
	GLfloat x = functionX(t);
	GLfloat y = functionY(t);
	GLfloat z = functionZ(t);
	return new Point3D(x, y, z);
}
Point3D* Vein::fDerivate(GLfloat t){
	GLfloat x = fDerivateX(t);
	GLfloat y = fDerivateY(t);
	GLfloat z = fDerivateZ(t);
	return new Point3D(x, y, z);
}
Point3D* Vein::sDerivate(GLfloat t){
	GLfloat x = sDerivateX(t);
	GLfloat y = sDerivateY(t);
	GLfloat z = sDerivateZ(t);
	return new Point3D(x, y, z);
}
//-------------------------------------------------------------------------
void Vein::build(){

	GLfloat intervaloToma = 2*2*M_PI/NQ;					 // Hay que dar dos vueltas porque hay puntos con 2 valores

	Poligon *poli = new Poligon(new Point3D(), NP, radius);  // un polígono del tamaño y con los lados que queremos
	vector<Point3D*>* puntos = poli->getVertex();

	for (int i = 0; i<NQ; i++){								 // Esto ocurre en cada "sección" del gusano
		GLfloat toma = intervaloToma*i;						 // Este valor hace que a cada vuelta la "matriz" sea única

		Point3D* fderivate = fDerivate(toma);				 // Se calculan los valores que ayudan al cálculo de los puntos
		Point3D* sderivate = sDerivate(toma);

		Point3D* Tt = fDerivate(toma); Tt->normalize();
		Point3D* Bt = fderivate->crossProduct(sderivate); Bt->normalize();
		Point3D* Nt = Bt->crossProduct(Tt);
		Point3D* Ct = function(toma);

		for (int j = 0; j<NP; j++){									// Esto ocurre con cada uno de los vértices del polígono
			int numV = NP*i + j;
			Point3D* clon = puntos->at(j)->clone();					// Un clon del punto del polígono para trabajar
			Point3D* punto = clon->matrixProduct(Nt, Bt, Tt, Ct);
			vertex->at(numV) = punto;								// El punto recibe un identificador y siempre con sentido
			delete clon;
		}

		//deletes de los objetos ya no necesarios
		delete sderivate;	delete fderivate;	delete Tt;
		delete Bt;			delete Nt;			delete Ct;

	}
	// Se construyen las Faces

	for (int numFace = 0; numFace<faces->size(); numFace++){      //  |>Recorremos todas las Faces en orden
		faces->at(numFace) = new Face(4);
		vector<VertexNormal*>* auxNormals = new vector<VertexNormal*>(4);

		int a = (numFace) % (NP*NQ);
		int b = (nextVertex(numFace)) % (NP*NQ);		// Teniendo cuidado de cerrar bien el círculo
		int c = (nextVertex(numFace) + NP) % (NP*NQ);
		int d = (numFace + NP) % (NP*NQ);

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
void Vein::draw(bool relleno){

	Mesh::draw(relleno);  // Dibuja la Mesh

	Point3D* fderivate = fDerivate(carPos);
	Point3D* sderivate = sDerivate(carPos);

	Point3D* Tt = fDerivate(carPos); Tt->normalize();					// Tt = C'
	Point3D* Bt = fderivate->crossProduct(sderivate); Bt->normalize();	// Bt = C'.C''
	Point3D* Nt = Bt->crossProduct(Tt);									// Nt = Bt.Tt
	Point3D* Ct = function(carPos);										// Ct = C

	GLfloat m[] = { Nt->getX(), Nt->getY(), Nt->getZ(), 0,		// Se niega Bt porque al ser 
		-Bt->getX(), -Bt->getY(), -Bt->getZ(), 0,		        // un producto escalar es perpendicular 
		Tt->getX(), Tt->getY(), Tt->getZ(), 0,		            // al plano definido por C'.C'' 
		Ct->getX(), Ct->getY(), Ct->getZ(), 1 };		        // pero en sentido contrario

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glMultMatrixf(m);
	glPopMatrix();

	//deletes de los objetos ya no necesarios
	delete sderivate;	delete fderivate;	delete Tt;
	delete Bt;			delete Nt;			delete Ct;
}