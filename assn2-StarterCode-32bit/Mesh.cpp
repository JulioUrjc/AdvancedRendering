#include "Mesh.h"

Mesh::Mesh(int nV, int nF, int nN){
	numVertex = nV;
	numFaces = nF;
	numNormals = nN;

	vertex = new vector<PV3D*>(numVertex);
	faces = new vector<Face*>(numFaces);
	normals = new vector<PV3D*>(numNormals);
}
Mesh::~Mesh(){
	delete vertex;	delete faces;	delete normals;
}

//--------------------------------------------------------------------------
PV3D* Mesh::doVectorNormalNewell(Face* c){
	GLdouble x = 0;
	GLdouble y = 0;
	GLdouble z = 0;
	PV3D* currentV;
	PV3D* nextV;
	for (int i = 0; i<c->getNumV(); i++){
		currentV = vertex->at(c->getVertexIndex(i));
		nextV = vertex->at(c->getVertexIndex((i + 1) % c->getNumV()));
		x += (currentV->getY() - nextV->getY())*(currentV->getZ() + nextV->getZ());
		y += (currentV->getZ() - nextV->getZ())*(currentV->getX() + nextV->getX());
		z += (currentV->getX() - nextV->getX())*(currentV->getY() + nextV->getY());
	}
	PV3D* aux = new PV3D(x, y, z);
	aux->normalize();
	return aux;
}
//--------------------------------------------------------------------------
void Mesh::draw(bool fill){

	for (int i = 0; i< numFaces; i++){
		glLineWidth(1.0);
		glColor3f(0, 0, 0);
		if (!fill){
			glBegin(GL_LINE_LOOP);
			glColor3f(1.0, 0, 0);
		}
		else{
			glBegin(GL_POLYGON);
			glColor3f(0, 1.0, 0);
		}
		for (int j = 0; j< faces->at(i)->getNumV(); j++){

			int iN = faces->at(i)->getNormalIndex(j);
			GLfloat nX = normals->at(iN)->getX();
			GLfloat nY = normals->at(iN)->getY();
			GLfloat nZ = normals->at(iN)->getZ();
			glNormal3f(nX, nY, nZ);
			///Si hubiera coordenadas de textura, aqui se suministrarian
			//las coordenadas de textura del vertice j con glTexCoor2f(c);
			int iV = faces->at(i)->getVertexIndex(j);
			GLfloat vX = vertex->at(iV)->getX();
			GLfloat vY = vertex->at(iV)->getY();
			GLfloat vZ = vertex->at(iV)->getZ();
			//glColor3f(vertex->at(iV)->getColor()->getX(), vertex->at(iV)->getColor()->getY(), vertex->at(iV)->getColor()->getZ());
			glVertex3f(vX, vY, vZ);
		}
		glEnd();
	}
}