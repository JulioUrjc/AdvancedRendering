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
	initValues();
	generateVectors();
	initShaders();
	generateBuffers();
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

		for (int j = 0; j<NP; j++){			// Esto ocurre con cada uno de los vértices del polígono
			// Se construyen los vertices
			int numV = NP*i + j;
			PV3D* clon = puntos->at(j)->clone();					// Un clon del punto del polígono para trabajar
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Vein::initValues(){
	//vShader= ShaderLoader("./shaders/veinVShader.glsl", GL_VERTEX_SHADER);
	//fShader= ShaderLoader("./shaders/veinFShader.glsl", GL_FRAGMENT_SHADER);

	GLenum a = glGetError();
	program = 0;

	inVertex = -1;
	inColor = -1;
	normalID = -1;
	//texCoordID = -1;

	mvpMatrixID = -1;
	modelViewID = -1;

	ambientLightID = -1;
	diffuseLightID = -1;
	lightDirectionID = -1;

	//textureID = -1;
}

//Prepare shaders
void Vein::initShaders(){
	//Compile shaders
	vShader.compileShader();
	fShader.compileShader();

	//Link
	program = glCreateProgram();
	glAttachShader(program, vShader.getShaderID());
	glAttachShader(program, fShader.getShaderID());
	glBindAttribLocation(program, 0, "inVertex");
	//glBindAttribLocation(program, 1, "inColor");
	glBindAttribLocation(program, 1, "normal");
	//glBindAttribLocation(program, 3, "inTexCoord");

	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked){
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

		char *logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete logString;

		glDeleteProgram(program);
		program = 0;
		return;
	}

	//Uniforms
	mvpMatrixID = glGetUniformLocation(program, "modelViewProjection");
	modelViewID = glGetUniformLocation(program, "modelView");

	//Lighting uniforms
	ambientLightID = glGetUniformLocation(program, "ambientLight");
	diffuseLightID = glGetUniformLocation(program, "diffuseLight");
	lightDirectionID = glGetUniformLocation(program, "lightDirection");

	//Attributes
	//inColor = glGetAttribLocation(program, "inColor");
	inVertex = glGetAttribLocation(program, "inVertex");
	cout << "inVertex: "<<inVertex << endl;
	normalID = glGetAttribLocation(program, "normal");
	cout << "normalID: " << normalID << endl;
	//texCoordID = glGetAttribLocation(program, "inTexCoord");
}

void Vein::generateVectors(){
	for (int i = 0; i < vertex->size(); i++){
		PV3D* punto = vertex->at(i)->clone();
		vertexVector.push_back(punto->getX());
		vertexVector.push_back(punto->getY());
		vertexVector.push_back(punto->getZ());

		PV3D* normal = normals->at(i)->clone();
		normalVector.push_back(normal->getX());
		normalVector.push_back(normal->getY());
		normalVector.push_back(normal->getZ());
	}
	for (int j = 0; j < faces->size(); j++){
		Face* face = faces->at(j);
		triangleVector.push_back(face->getVertexIndex(0));
		triangleVector.push_back(face->getVertexIndex(1));
		triangleVector.push_back(face->getVertexIndex(2));
		triangleVector.push_back(face->getVertexIndex(3));
	}
}


//Generate VBO && VAO
void Vein::generateBuffers(){
	glFrontFace(GL_CW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(3, buffer);

	//Vertex
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexVector.size(), &(vertexVector.front()), GL_STATIC_DRAW);
	glVertexAttribPointer(inVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);  //Shader input
	glEnableVertexAttribArray(inVertex);

	//Vertex
	//glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*colorVector.size(), &(colorVector.front()), GL_STATIC_DRAW);
	//glVertexAttribPointer(inColor, 1, GL_FLOAT, GL_FALSE, 0, 0);  //Shader input
	//glEnableVertexAttribArray(inColor);

	//Normals
	glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normalVector.size(), &(normalVector.front()), GL_STATIC_DRAW);
	glVertexAttribPointer(normalID, 3, GL_FLOAT, GL_FALSE, 0, 0);  //Shader input
	glEnableVertexAttribArray(normalID);

	//Quads
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*triangleVector.size(), &(triangleVector.front()), GL_STATIC_DRAW);

	glBindVertexArray(0);

	//generateTexture();
}

//void Lake::generateTexture(){
//	TextureLoader loader("./Resources/...");
//
//	glGenTextures(1, &textureID);
//	glBindTexture(GL_TEXTURE_2D, textureID);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, loader.getWidth(), loader.getHeight(), 0,
//		GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)loader.getTexture());
//
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//}

//Draw the Lake
void Vein::draw(Camara camara){
	glUseProgram(program);
	glm::mat4 modelMatrix = glm::translate(glm::vec3(0, 0, 0));

	//Set ModelViewProjection matrix uniform
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &camara.getModelViewProjection(modelMatrix)[0][0]);
	glUniformMatrix4fv(modelViewID, 1, GL_FALSE, &camara.getModelView(modelMatrix)[0][0]);
	//Lighting uniforms
	glUniform3f(ambientLightID, ambientLight.x, ambientLight.y, ambientLight.z);
	glUniform3f(diffuseLightID, diffuseLight.x, diffuseLight.y, diffuseLight.z);
	glUniform3f(lightDirectionID, lightDirection.x, lightDirection.y, lightDirection.z);

	//Drawing   
	glBindVertexArray(vao);

	//Textures
	/*glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE5);
	GLint loc = glGetUniformLocation(program, "texMap");
	glUniform1i(loc, 5);
	glBindTexture(GL_TEXTURE_2D, textureID);*/

	//glDrawElements(GL_TRIANGLES, triangleVector.size(), GL_UNSIGNED_INT, 0);
	glDrawElements(GL_QUADS, triangleVector.size(), GL_UNSIGNED_INT, 0);

	glUseProgram(NULL);
}

void Vein::setDiffuseLight(glm::vec3 newLight){
	diffuseLight = newLight;
}

//Free memory
void Vein::freeMemory(){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(3, buffer);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);

	glDetachShader(program, vShader.getShaderID());
	glDetachShader(program, fShader.getShaderID());

	vShader.deleteShader();
	fShader.deleteShader();

	glDeleteProgram(program);

	//Texture
	//glDeleteTextures(1, &textureID);
}