#include "BloodElement.h"

BloodElement::BloodElement(Elements element, PV3D pos, PV3D rot){
	position = pos;
	//std::cout << "Position:" << position.getX() << " " << position.getY() << " " << position.getZ() << std::endl;
	rotation = rot;
	type = element;

	initMatrix();
	createElement(element);

	initValues();
	generateVectors();
	initShaders();
	generateBuffers();
}

BloodElement::~BloodElement(){}

void BloodElement::initMatrix(){

	//Calculate matrix
	/*
	glm::mat4x4 matRotX	(1, 0, 0, 0,
						0, glm::cos(rotation.getX()), -glm::sin(rotation.getX()), 0,
						0, glm::sin(rotation.getX()), glm::cos(rotation.getX()), 0,
						0, 0, 0, 1);

	glm::mat4x4 matRotY(glm::cos(rotation.getY()), 0, glm::sin(rotation.getY()), 0,
						0, 1, 0, 0,
						-glm::sin(rotation.getY()), 0, glm::cos(rotation.getY()), 0,
						0, 0, 0, 1);

	glm::mat4x4 matRotZ(glm::cos(rotation.getZ()), -glm::sin(rotation.getZ()), 0, 0,
						glm::sin(rotation.getZ()), glm::cos(rotation.getZ()), 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1);
	*/
	glm::mat4x4 identity = glm::mat4x4(1.0f);
	rotationMatrix = glm::rotate(identity,(float)rotation.getX(), glm::vec3(1, 0, 0));
	//std::cout << "rotacion x" << std::endl;
	//std::cout << rotationMatrix[0][0] << " " << rotationMatrix[0][1] << " " << rotationMatrix[0][2] << " " << rotationMatrix[0][3] << std::endl;
	//std::cout << rotationMatrix[1][0] << " " << rotationMatrix[1][1] << " " << rotationMatrix[1][2] << " " << rotationMatrix[1][3] << std::endl;
	//std::cout << rotationMatrix[2][0] << " " << rotationMatrix[2][1] << " " << rotationMatrix[2][2] << " " << rotationMatrix[2][3] << std::endl;
	//std::cout << rotationMatrix[3][0] << " " << rotationMatrix[3][1] << " " << rotationMatrix[3][2] << " " << rotationMatrix[3][3] << std::endl;
	rotationMatrix = glm::rotate(rotationMatrix, (float)rotation.getY(), glm::vec3(0, 1, 0));
	//std::cout << "rotacion x+y" << std::endl;
	//std::cout << rotationMatrix[0][0] << " " << rotationMatrix[0][1] << " " << rotationMatrix[0][2] << " " << rotationMatrix[0][3] << std::endl;
	//std::cout << rotationMatrix[1][0] << " " << rotationMatrix[1][1] << " " << rotationMatrix[1][2] << " " << rotationMatrix[1][3] << std::endl;
	//std::cout << rotationMatrix[2][0] << " " << rotationMatrix[2][1] << " " << rotationMatrix[2][2] << " " << rotationMatrix[2][3] << std::endl;
	//std::cout << rotationMatrix[3][0] << " " << rotationMatrix[3][1] << " " << rotationMatrix[3][2] << " " << rotationMatrix[3][3] << std::endl;
	rotationMatrix = glm::rotate(rotationMatrix, (float)rotation.getZ(), glm::vec3(0, 0, 1));
	//std::cout << "rotacion x+y+z" << std::endl;
	//std::cout << rotationMatrix[0][0] << " " << rotationMatrix[0][1] << " " << rotationMatrix[0][2] << " " << rotationMatrix[0][3] << std::endl;
	//std::cout << rotationMatrix[1][0] << " " << rotationMatrix[1][1] << " " << rotationMatrix[1][2] << " " << rotationMatrix[1][3] << std::endl;
	//std::cout << rotationMatrix[2][0] << " " << rotationMatrix[2][1] << " " << rotationMatrix[2][2] << " " << rotationMatrix[2][3] << std::endl;
	//std::cout << rotationMatrix[3][0] << " " << rotationMatrix[3][1] << " " << rotationMatrix[3][2] << " " << rotationMatrix[3][3] << std::endl;

	translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(position.getX(), position.getY(), position.getZ()));
	//std::cout << "translaccion" << std::endl;
	//std::cout << translateMatrix[0][0] << " " << translateMatrix[0][1] << " " << translateMatrix[0][2] << " " << translateMatrix[0][3] << std::endl;
	//std::cout << translateMatrix[1][0] << " " << translateMatrix[1][1] << " " << translateMatrix[1][2] << " " << translateMatrix[1][3] << std::endl;
	//std::cout << translateMatrix[2][0] << " " << translateMatrix[2][1] << " " << translateMatrix[2][2] << " " << translateMatrix[2][3] << std::endl;
	//std::cout << translateMatrix[3][0] << " " << translateMatrix[3][1] << " " << translateMatrix[3][2] << " " << translateMatrix[3][3] << std::endl;
	//translateMatrix = glm::transpose(translateMatrix);
	//std::cout << "translaccion transpose" << std::endl;
	//std::cout << translateMatrix[0][0] << " " << translateMatrix[0][1] << " " << translateMatrix[0][2] << " " << translateMatrix[0][3] << std::endl;
	//std::cout << translateMatrix[1][0] << " " << translateMatrix[1][1] << " " << translateMatrix[1][2] << " " << translateMatrix[1][3] << std::endl;
	//std::cout << translateMatrix[2][0] << " " << translateMatrix[2][1] << " " << translateMatrix[2][2] << " " << translateMatrix[2][3] << std::endl;
	//std::cout << translateMatrix[3][0] << " " << translateMatrix[3][1] << " " << translateMatrix[3][2] << " " << translateMatrix[3][3] << std::endl;


}

void BloodElement::createElement(Elements element){
	switch (element){
		case RED:
			createPrimitive(TORUS, 0.005f, 0.05f, 5, 8);
			break;
		case WHITE:
			createPrimitive(TORUS, 0.005f, 0.05f, 10, 10);
			createPrimitive(TORUS, 0.01f, 0.1f, 10, 10);
			break;
	}
}

void BloodElement::createPrimitive(Primitives primitive, GLfloat size, GLfloat height, GLint sliceX, GLint sliceY){
	
	GLUquadricObj *q;

	GLfloat x = size / 2;
	GLfloat z = height / 2;

	switch (primitive){
		case TORUS:
		{
			//glutSolidTorus(size, height, sliceX, sliceY);
			//torus creation - thanks to http://www.sulaco.co.za/
			float theta, phi, theta1,
				cosTheta, sinTheta,
				cosTheta1, sinTheta1,
				ringDelta, sideDelta,
				cosPhi, sinPhi, dist;

			sideDelta = 2.0 * M_PI / sliceX;
			ringDelta = 2.0 * M_PI / sliceY;

			theta = 0.0;
			cosTheta = 1.0;
			sinTheta = 0.0;

			for (int i = sliceY - 1; i >= 0; i--){
				theta1 = theta + ringDelta;
				cosTheta1 = cos(theta1);
				sinTheta1 = sin(theta1);

				phi = 0.0;
				for (int j = sliceX; j >= 0; j--){
					phi = phi + sideDelta;
					cosPhi = cos(phi);
					sinPhi = sin(phi);
					dist = height + (size * cosPhi);

					glm::vec4 point1 = glm::vec4(cosTheta1 * dist, -sinTheta1 * dist, size * sinPhi, 1);
					glm::vec4 point2 = glm::vec4(cosTheta * dist, -sinTheta * dist, size * sinPhi, 1);
					//std::cout << "point1: " << point1.x << " " << point1.y << " " << point1.z << std::endl;
					//Apply rotation matrix
					point1 = rotationMatrix*point1;
					point2 = rotationMatrix*point2;
					//std::cout << "rotacion matrix:" << std::endl;
					//std::cout << rotationMatrix[0][0] << " " << rotationMatrix[0][1] << " " << rotationMatrix[0][2] << " " << rotationMatrix[0][3] << std::endl;
					//std::cout << rotationMatrix[1][0] << " " << rotationMatrix[1][1] << " " << rotationMatrix[1][2] << " " << rotationMatrix[1][3] << std::endl;
					//std::cout << rotationMatrix[2][0] << " " << rotationMatrix[2][1] << " " << rotationMatrix[2][2] << " " << rotationMatrix[2][3] << std::endl;
					//std::cout << rotationMatrix[3][0] << " " << rotationMatrix[3][1] << " " << rotationMatrix[3][2] << " " << rotationMatrix[3][3] << std::endl;

					//std::cout << "point1 rotated: " << point1.x << " " << point1.y << " " << point1.z << std::endl;

					//Apply translate matrix
					point1 = translateMatrix*point1;
					point2 = translateMatrix*point2;
					//std::cout << "translate matrix:" << std::endl;
					//std::cout << translateMatrix[0][0] << " " << translateMatrix[0][1] << " " << translateMatrix[0][2] << " " << translateMatrix[0][3] << std::endl;
					//std::cout << translateMatrix[1][0] << " " << translateMatrix[1][1] << " " << translateMatrix[1][2] << " " << translateMatrix[1][3] << std::endl;
					//std::cout << translateMatrix[2][0] << " " << translateMatrix[2][1] << " " << translateMatrix[2][2] << " " << translateMatrix[2][3] << std::endl;
					//std::cout << translateMatrix[3][0] << " " << translateMatrix[3][1] << " " << translateMatrix[3][2] << " " << translateMatrix[3][3] << std::endl;

					//std::cout << "point1 translated: " << point1.x << " " << point1.y << " " << point1.z << std::endl;

					vertex.push_back(PV3D(point1.x, point1.y, point1.z));
					vertex.push_back(PV3D(point2.x, point2.y, point2.z));

					//Calculating the normals 
					glm::vec4 normal1 = glm::vec4(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi, 1);
					glm::vec4 normal2 = glm::vec4(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi, 1);

					normal1 = rotationMatrix*normal1;
					normal2 = rotationMatrix*normal2;

					normals.push_back(PV3D(normal1.x, normal1.y, normal1.z));
					normals.push_back(PV3D(normal2.x, normal2.y, normal2.z));

					//glTexCoord2f(cosTheta1 * cosPhi, -sinTheta1 * cosPhi);
					//glNormal3f(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
					//glVertex3f(cosTheta1 * dist, -sinTheta1 * dist, size * sinPhi);

					//glTexCoord2f(cosTheta * cosPhi, -sinTheta * cosPhi);
					//glNormal3f(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
					//glVertex3f(cosTheta * dist, -sinTheta * dist, size * sinPhi);
				}
				theta = theta1;
				cosTheta = cosTheta1;
				sinTheta = sinTheta1;
			}
		}
			break;
		case TEAPOT:
			glutSolidTeapot(size);
			break;
		default:
			break;
	}
}

void BloodElement::draw(int modo){
	/*glPushMatrix();

	glTranslatef(position.getX(), position.getY(), position.getZ());
	glRotatef(rotation.getX(), 1.0f, 0.0f, 0.0f);
	glRotatef(rotation.getY(), 0.0f, 1.0f, 0.0f);
	glRotatef(rotation.getZ(), 0.0f, 0.0f, 1.0f);*/

	switch (modo){
		case 0:
			glBegin(GL_POINTS);
			break;
		case 1:
			glBegin(GL_LINES);
			break;
		case 2:
			glBegin(GL_QUAD_STRIP);
			break;
		default:
			break;
	}
		for (int i = 0; i < vertex.size(); i++){
			(type==RED) ? glColor3f(1.0f, 0.1f, 0.2f) : glColor3f(1.0f, 1.0f, 0.8f);
			glVertex3f(vertex.at(i).getX(), vertex.at(i).getY(), vertex.at(i).getZ());
			//vertex.at(i).toString();
		}
	glEnd();
	/*glPopMatrix();*/
}

/* OpenGL 3.3 - gpu compute*/

void BloodElement::initValues(){

	//GLenum a = glGetError();
	program = 0;

	inVertex = -1;
	inNormal = -1;
	inColor = -1;
	//texCoordID = -1;

	mvpMatrixID = -1;
	modelViewID = -1;

	ambientLightID = -1;
	diffuseLightID = -1;
	lightDirectionID = -1;

	//textureID = -1;
}

//Prepare shaders
void BloodElement::initShaders(){
	//Compile shaders
	vShader.compileShader();
	fShader.compileShader();

	//Link
	program = glCreateProgram();
	glAttachShader(program, vShader.getShaderID());
	glAttachShader(program, fShader.getShaderID());
	glBindAttribLocation(program, 0, "inVertex");
	glBindAttribLocation(program, 1, "inNormal");
	//glBindAttribLocation(program, 2, "inColor");
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
	inColor = glGetUniformLocation(program, "inColor");

	//Lighting uniforms
	ambientLightID = glGetUniformLocation(program, "ambientLight");
	diffuseLightID = glGetUniformLocation(program, "diffuseLight");
	lightDirectionID = glGetUniformLocation(program, "lightDirection");

	//Attributes
	inVertex = glGetAttribLocation(program, "inVertex");
	inNormal = glGetAttribLocation(program, "inNormal");
	//inColor = glGetAttribLocation(program, "inColor");
	//texCoordID = glGetAttribLocation(program, "inTexCoord");
}

void BloodElement::generateVectors(){
	for (int i = 0; i < vertex.size(); i++){
		PV3D punto = vertex.at(i);
		vertexVector.push_back(punto.getX());
		vertexVector.push_back(punto.getY());
		vertexVector.push_back(punto.getZ());

		PV3D normal = normals.at(i);
		normalVector.push_back(normal.getX());
		normalVector.push_back(normal.getY());
		normalVector.push_back(normal.getZ());
	}
	for (unsigned int i = 0; i<vertexVector.size() / 3; ++i)
		indexVector.push_back(i);
}


//Generate VBO && VAO
void BloodElement::generateBuffers(){
	glFrontFace(GL_CW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(3, buffer);

	//Vertex
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexVector.size(), &(vertexVector.front()), GL_STATIC_DRAW);
	glVertexAttribPointer(inVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);  //Shader input
	glEnableVertexAttribArray(inVertex);

	//Normals
	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normalVector.size(), &(normalVector.front()), GL_STATIC_DRAW);
	glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);  //Shader input
	glEnableVertexAttribArray(inNormal);

	//Color
	//glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*colorVector.size(), &(colorVector.front()), GL_STATIC_DRAW);
	//glVertexAttribPointer(inColor, 1, GL_FLOAT, GL_FALSE, 0, 0);  //Shader input
	//glEnableVertexAttribArray(inColor);

	//Quads
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexVector.size(), &(indexVector.front()), GL_STATIC_DRAW);

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

/* Draw the Blood Element */
void BloodElement::draw(Camara* camara, int modo){
	glUseProgram(program);
	glm::mat4 modelMatrix = glm::translate(glm::vec3(0, 0, 0));

	//Set ModelViewProjection matrix uniform
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &camara->getModelViewProjection(modelMatrix)[0][0]);
	glUniformMatrix4fv(modelViewID, 1, GL_FALSE, &camara->getModelView(modelMatrix)[0][0]);
	//Lighting uniforms
	ambientLight = glm::vec3(0.3f, 0.3f, 0.3f);
	diffuseLight = glm::vec3(0.7f, 0.7f, 0.7f);
	lightDirection = normalize(glm::vec3(2.0f, 5.0f, 2.0f));
	glUniform3f(ambientLightID, ambientLight.x, ambientLight.y, ambientLight.z);
	glUniform3f(diffuseLightID, diffuseLight.x, diffuseLight.y, diffuseLight.z);
	glUniform3f(lightDirectionID, lightDirection.x, lightDirection.y, lightDirection.z);

	glm::vec3 color = (type == RED) ? glm::vec3(1.0, 0.0, 0.0) : glm::vec3(1.0, 1.0, 1.0);
	glUniform3f(inColor, color.x, color.y, color.z);

	//Drawing   
	glBindVertexArray(vao);

	//Textures
	/*glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE5);
	GLint loc = glGetUniformLocation(program, "texMap");
	glUniform1i(loc, 5);
	glBindTexture(GL_TEXTURE_2D, textureID);*/

	if (modo == 1){
		glDrawElements(GL_POINTS, indexVector.size(), GL_UNSIGNED_INT, 0);
	}else if (modo == 2){
		glPolygonMode(GL_FRONT, GL_LINE);
		glDrawElements(GL_QUAD_STRIP, indexVector.size(), GL_UNSIGNED_INT, 0);
	}else{
		glPolygonMode(GL_FRONT, GL_FILL);
		glDrawElements(GL_QUAD_STRIP, indexVector.size(), GL_UNSIGNED_INT, 0);
	}

	glUseProgram(NULL);
}

void BloodElement::setDiffuseLight(glm::vec3 newLight){
	diffuseLight = newLight;
}

void BloodElement::freeMemory(){
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
