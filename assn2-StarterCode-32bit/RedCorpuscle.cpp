#include "RedCorpuscle.h"


RedCorpuscle::RedCorpuscle(PV3D pos, PV3D rot)
{
	position = pos;
	rotation = rot;
}


RedCorpuscle::~RedCorpuscle()
{
}

void RedCorpuscle::draw(int modo){

	if (modo == 1){
		/**/
	}
	else if (modo == 2){ //wire mode
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(position.getX(), position.getY(), position.getZ());
		glRotatef(rotation.getX(), 1.0f, 0.0f, 0.0f);
		glRotatef(rotation.getY(), 0.0f, 1.0f, 0.0f);
		glRotatef(rotation.getZ(), 0.0f, 0.0f, 1.0f);
		glColor3f(0.9f, 0.1f, 0.2f);
		glutWireTorus(0.02f, 0.2f, 10, 10);
		glPopMatrix();
	}
	else{ // solid mode
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(position.getX(), position.getY(), position.getZ());
		glRotatef(rotation.getX(), 1.0f, 0.0f, 0.0f);
		glRotatef(rotation.getY(), 0.0f, 1.0f, 0.0f);
		glRotatef(rotation.getZ(), 0.0f, 0.0f, 1.0f);
		glColor3f(0.9f, 0.1f, 0.2f);
		glutSolidTorus(0.02f, 0.2f, 10, 10);
		glPopMatrix();
	}
	
