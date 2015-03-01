#include "BloodElement.h"


BloodElement::BloodElement()
{
	//verts = polys = 0;
	//type = PRIMITIVE;
	//pos.LoadIdentity();
	//rot.LoadIdentity();
	//scale = 1.0;
	//mat = NULL;
	//shadow_cast = true;
}


BloodElement::~BloodElement()
{
}

void BloodElement::create(const char* _name, int primitive, GLfloat size, GLfloat height, GLint sliceX, GLint sliceY){
	
	GLUquadricObj *q;

	GLfloat x = size / 2;
	GLfloat z = height / 2;

	switch (primitive)
	{
		//cube: 1 param: size
	case CUBE:
		glutSolidCube(size);
		break;
		//plane: 2 params, size, sizeY
	case PLANE:
		glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-x, 0.0, z);
		glTexCoord2f(5.0, 0.0); glVertex3f(x, 0.0, z);
		glTexCoord2f(5.0, 5.0); glVertex3f(x, 0.0, -z);
		glTexCoord2f(0.0, 5.0); glVertex3f(-x, 0.0, -z);
		glEnd();
		break;
		//cone: 4 params: baseRadius, height, sliceX, sliceY
	case CONE:
		gluCylinder(q, size, 0.0, height, sliceX, sliceY);
		break;
		//cylinder: 4 params: baseRadius, height, sliceX, sliceY
	case CYLINDER:
		gluCylinder(q, size, size, height, sliceX, sliceY);
		break;
		//disk: 4 params: innerRadius, outerRadius, sliceX, sliceY
	case DISK:
		gluDisk(q, size, height, sliceX, sliceY);
		break;
		//sphere: 3 params: radius, sliceX, sliceY
	case SPHERE:
		gluSphere(q, size, sliceX, sliceY);
		break;
		//torus: 4 params: innerRadius, outerRadius, nsides, rings
	case TORUS:
		glutSolidTorus(size, height, sliceX, sliceY);
		break;
	case TEAPOT:
		glutSolidTeapot(size);
		break;
	default:
		break;
	}

	gluDeleteQuadric(q);
}
