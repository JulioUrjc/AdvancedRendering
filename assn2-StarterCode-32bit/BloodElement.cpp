#include "BloodElement.h"


BloodElement::BloodElement(Elements element, PV3D pos, PV3D rot){
	position = pos;
	rotation = rot;
	createElement(element);
	type = element;

	initMatrix();

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
	rotationMatrix = glm::rotate(rotationMatrix, (float)rotation.getY(), glm::vec3(0, 1, 0));
	rotationMatrix = glm::rotate(rotationMatrix, (float)rotation.getZ(), glm::vec3(0, 0, 1));

	translateMatrix = glm::translate( glm::vec3(position.getX(), position.getY(), position.getZ()));
}

void BloodElement::createElement(Elements element){
	switch (element){
		case RED:
			createPrimitive(TORUS, 0.01f, 0.1f, 10, 10);
			break;
		case WHITE:
			createPrimitive(TORUS, 0.01f, 0.1f, 10, 10);
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

					//Apply rotation matrix
					point1 = rotationMatrix*point1;
					point2 = rotationMatrix*point2;

					//Apply translate matrix
					point1 = translateMatrix*point1;
					point2 = translateMatrix*point2;

					vertex.push_back(PV3D(point1.x, point1.y, point1.z));
					vertex.push_back(PV3D(point2.x, point2.y, point2.z));

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
