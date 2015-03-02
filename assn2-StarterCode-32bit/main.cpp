/*
	Simulacion del Torrente Sanguineo
	RA - Raquel Peces & Julio Martin
*/

#include <stdio.h>
#include <stdlib.h>
#include "pic.h"

#ifdef WIN32

#include <windows.h>
#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"
#pragma comment(lib, "win32/libtiff_i.lib")
#pragma comment(lib, "win32/jpeg.lib")
#pragma comment(lib, "win32/libtiff.lib")
#pragma comment(lib, "win32/libpicio.lib")
#pragma comment(lib, "libs/glut32.lib")
#pragma comment(lib, "libs/glew32.lib")
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "BezierCurve.h"
#include "Perlin_noise/PerlinGenerator.h"
#include "Vein.h"
#include "Camara.h"
#include "Blood.h"
#include <vector>
#include <iostream>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

/* - Menu State Identifier - */
int g_iMenuId;

/* - Mouse State Variables - */
int g_vMousePos[2] = {0, 0};
int g_iLeftMouseButton = 0;    /* 1 if pressed, 0 if not */
int g_iMiddleMouseButton = 0;
int g_iRightMouseButton = 0;

/* - BezierCurve Variable - */
const int curveSteps = 100;
const float curveT = 0.7f;
BezierCurve* curve;

/* - Vein Variable - */
const int veinSides = 25;
const float veinRadius = 0.5f;
Vein* vein;

/* - Blood Variable - */
 const int numRedCorpuscles = 10;
 const int numWhiteCorpuscles = 10;
 Blood* blood;

/* - Perlin Noise - */
const int sideVertex = 256;
PerlinGenerator perlinNoise(6, sideVertex);

/* Cam Variables*/
PV3D *eye, *look, *up;
GLdouble xRight, xLeft, yTop, yBot, N, F;
float angleYaw=0, angleRoll=0, anglePitch=0;
Camara* camara;
int modo = 2;    // Mode lines
int point = 0;   // Curve's Point 

/*	saveScreenshot - Writes a screenshot to the specified filename in JPEG */
void saveScreenshot (char *filename){
	Pic *in = NULL;

	if (filename == NULL)
		return;

	/* Allocate a picture buffer */
	in = pic_alloc (WINDOW_WIDTH, WINDOW_HEIGHT, 3, NULL);

	printf("File to save to: %s\n", filename);
	/* Loop over each row of the image and copy into the image */
	for (int i = WINDOW_HEIGHT - 1; i >= 0; i--){
		glReadPixels(0, WINDOW_HEIGHT - 1 - i, WINDOW_WIDTH, 1, GL_RGB,
						GL_UNSIGNED_BYTE, &in->pix[i*in->nx*in->bpp]);
	}

	/* Output the file */
	if (jpeg_write(filename, in)){
		printf("File saved Successfully\n");
	}else{
		printf("Error in Saving\n");
	}
	
	pic_free(in);
}

/*	loadTexture - Loads a texture from a JPEG file to memory and returns the handle	*/
GLuint loadTexture (char *filename, int *pWidth = NULL, int *pHeight = NULL){
	GLuint texIndex;

	if (filename == NULL)
		return 0;

	Pic *texture = jpeg_read (filename, NULL);
	if (texture == NULL)
		return 0;

	glGenTextures (1, &texIndex);
	glBindTexture (GL_TEXTURE_2D, texIndex);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	if (pWidth != NULL)
		*pWidth = texture->nx;
	if (pHeight != NULL)
		*pHeight = texture->ny;

	switch (texture->bpp)
	{
	case 1:
	default:
		gluBuild2DMipmaps (GL_TEXTURE_2D, 1, texture->nx, texture->ny,
					GL_R3_G3_B2, GL_UNSIGNED_BYTE, texture->pix);
		break;
	case 3:
		gluBuild2DMipmaps (GL_TEXTURE_2D, 3, texture->nx, texture->ny,
					GL_RGB, GL_UNSIGNED_BYTE, texture->pix);
		break;
	case 4:
		gluBuild2DMipmaps (GL_TEXTURE_2D, 4, texture->nx, texture->ny,
					GL_RGBA8, GL_UNSIGNED_BYTE, texture->pix);
		break;
	}

	pic_free(texture);

	return texIndex;
}

/*	doIdle - The idle-function that can be used to update the screen */
void doIdle(){
	glutPostRedisplay();
}

/*	mousedrag - converts mouse drags into information about rotation/translation/scaling */
void mousedrag(int x, int y){
	int vMouseDelta[2] = {x-g_vMousePos[0], y-g_vMousePos[1]};
	g_vMousePos[0] = x;
	g_vMousePos[1] = y;
}

/*	mouseidle - Idle mouse movement callback function */
void mouseidle(int x, int y){
	g_vMousePos[0]= x;
	g_vMousePos[1]= y;
}

/* 	mousebutton - Sets the global mouse states according to the actions */
void mousebutton(int button, int state, int x, int y){
	switch (button){
		case GLUT_LEFT_BUTTON:
			g_iLeftMouseButton = (state==GLUT_DOWN);
			break;
		case GLUT_MIDDLE_BUTTON:
			g_iMiddleMouseButton = (state==GLUT_DOWN);
			break;
		case GLUT_RIGHT_BUTTON:
			g_iRightMouseButton = (state==GLUT_DOWN);
			break;
	}
	g_vMousePos[0] = x;
	g_vMousePos[1] = y;
}

void key(unsigned char key, int x, int y){
	bool need_redisplay = true;
	switch (key) {
	case 27:  /* Escape key */
		exit(0);
		break;

	case 'a':
		++point;
		if (point > curve->nPoints()-1) point = 0;
		eye = curve->getPointList().at(point);
		//look= curve->getTangentList().at(point);
		look = eye->addition(curve->getTangentList().at(point));
		up  = curve->getBinormalList().at(point);		
		/*look= curve->getPointList().at(point);
		up  = curve->getTangentList().at(point);
		eye = look->addition(curve->getBinormalList().at(point))->addition(new PV3D(2.0,2.0,2.0));*/
		//eye = new PV3D(2 + look->getX() + curve->getBinormalList().at(point)->getX(), 2 + look->getY() + curve->getBinormalList().at(point)->getY(), 2 + look->getZ() + curve->getBinormalList().at(point)->getZ());

		camara->moveCamara(eye, look, up);
		camara->fijarCam();
		break;
	case 'z':
		--point;
		if (point < 0) point = curve->nPoints()-1;
		eye = curve->getPointList().at(point);
		//look= curve->getTangentList().at(point);
		look = eye->addition(curve->getTangentList().at(point));
		up  = curve->getBinormalList().at(point);
		/*look = curve->getPointList().at(point);
		up = curve->getTangentList().at(point);
		eye = look->addition(curve->getBinormalList().at(point))->addition(new PV3D(2.0, 2.0, 2.0));*/

		camara->moveCamara(eye, look, up);
		camara->fijarCam();
		break;
	// Teclas para giros
	case 'f':
		angleYaw += 0.01;
		camara->yaw(angleYaw);
		break;
	case 'v':
		angleYaw -= 0.01;
		camara->yaw(angleYaw);
		break;
	case '-':
		camara->desplazar(0.0,0.0,0.01);

		break;
	// Cambio de Modo entre puntos, aristas o poligonos
	case '1':
			modo = 1;
		break;
	case '2':
			modo = 2;
		break;
	case '3':
			modo = 3;
		break;
	}
}

/*	display - Function to modify with your heightfield rendering code (Currently displays a simple cube) */
void display(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*if (modo == 1){
		glBegin(GL_POINTS);
		glPointSize(3);
		glColor3f(1.0, 0, 0.0);
	}
	else if (modo == 2){
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0, 0);
	}else*/
		glBegin(GL_LINE_STRIP);
	
	for (PV3D* punto: curve->getPointList()){
		//cout << punto->getX() << " " << punto->getY() << " " << punto->getZ() << endl;
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(punto->getX(), punto->getY(), punto->getZ());	
	}
	//glVertex3f(curve->getPointList().at(0)->getX(), curve->getPointList().at(0)->getY(), curve->getPointList().at(0)->getZ());
	glEnd();

	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glTranslatef(-4.0, 5.0f, 0.0f);
	//glRotatef(M_PI/4,0.0f,1.0f,0.0f);
	//glColor3f(0.5f, 0.5f, 0.2f);
	//glutSolidTorus(0.02f, 0.2f, 10, 10);
	//glPopMatrix();

	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glTranslatef(-2.0, -5.0f, 0.0f);
	//glRotatef(M_PI / 4, 0.0f, 1.0f, 0.0f);
	//glColor3f(0.3f, 0.3f, 0.5f);
	//glutSolidTorus(0.02f, 0.2f, 10, 10);
	//glPopMatrix();

	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glTranslatef(1.0, 0.0f, 0.0f);
	//glRotatef(M_PI / 4, 0.0f, 1.0f, 0.0f);
	//glColor3f(0.8f, 0.5f, 0.3f);
	//glutSolidTorus(0.02f, 0.2f, 10, 10);
	//glPopMatrix();

	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glTranslatef(2.0, -5.0f, 0.0f);
	//glRotatef(M_PI / 4, 0.0f, 1.0f, 0.0f);
	//glColor3f(0.1f, 0.3f, 0.8f);
	//glutSolidTorus(0.02f, 0.2f, 10, 10);
	//glPopMatrix();

	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glTranslatef(4.0, 5.0f, 0.0f);
	//glRotatef(M_PI / 4, 0.0f, 1.0f, 0.0f);
	//glColor3f(0.2f, 0.5f, 0.3f);
	//glutSolidTorus(0.02f, 0.2f, 10, 10);
	//glPopMatrix();

	
	if (modo == 1){
		glPolygonMode(GL_FRONT, GL_POINT);
	}else if(modo==2){
		glPolygonMode(GL_FRONT, GL_LINE);
	}else{
		glPolygonMode(GL_FRONT, GL_FILL);
	}
	
	//vein->draw(false, camara, point);
	//vein->draw(modo);
	vein->draw(camara);
	//vein->draw(modo);
	blood->draw(modo);

	glutSwapBuffers();
}

/*	menufunc - Menu Event-handler */
void menufunc(int value){
	switch (value){
	case 0:
		exit(0);
		break;
	case 1:
		modo= ((modo+1)%3)+1;
		break;
	}
}

void startGlew(){
	GLenum err = glewInit();
	if (GLEW_OK != err){
		printf("Error: %s\n", glewGetErrorString(err));
	}
	const GLubyte *oglVersion = glGetString(GL_VERSION);
	printf("This system supports OpenGL Version %s.\n", oglVersion);
}

/*	myinit - Function to add your initialization code */
void startGlut(){
	// Por defecto
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Torrente Sanguineo");
	
	glutDisplayFunc(display);			/* tells glut to use a particular display function to redraw */
	glutKeyboardFunc(key);

	/* allow the user to quit using the right mouse button menu */
	g_iMenuId = glutCreateMenu(menufunc);
	glutSetMenu(g_iMenuId);
	glutAddMenuEntry("Quit", 0);
	glutAddMenuEntry("ModoVisualiz", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutIdleFunc(doIdle);				/* replace with any animate code */
	glutMotionFunc(mousedrag);			/* callback for mouse drags */
	glutPassiveMotionFunc(mouseidle);	/* callback for idle mouse movement */
	glutMouseFunc(mousebutton);			/* callback for mouse button changes */
}

void startCam(){	

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialf(GL_FRONT, GL_SHININESS, 0.1f);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT, GL_LINE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	//// Camera set up
	camara->fijarCam();

	//// Frustum set up
	camara->ortogonal();

	//// Viewport set up
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//// Light0
	glEnable(GL_LIGHT0);
	GLfloat d[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, d);
	GLfloat a[] = { 0.3f, 0.3f, 0.3f, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, a);
	GLfloat p[] = { 25.0, 25.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, p);
}

//Destroy and free memory
void unlinkAndFree(){
	vein->freeMemory();
}

/*	main - The Main Function */
int main (int argc, char ** argv){

	glutInit(&argc,argv);

	/* do initialization */
	startGlut(); 
	startGlew();

	/* Creamos el ruido de Perlin*/
	std::cout << "Generating perlin noise..." << std::endl;
	perlinNoise.generate();

	/* Creamos la curva y la vena asociada*/
	std::cout << "Generating bezier curve..." << std::endl;
	curve = new BezierCurve(curveSteps, curveT);

	std::cout << "Generating vein..." << std::endl;
	vein = new Vein(veinSides, veinRadius, curve);
	//vein->addPerlinNoise(perlinNoise.getNoiseImage());

	/* Creamos los globulos dentro de la vena */
	std::cout << "Generating blood..." << std::endl;
	blood = new Blood(numRedCorpuscles, numWhiteCorpuscles, curve);
	
	//// Camera parameters
	eye = curve->getPointList().at(point);
	look= eye->addition(curve->getTangentList().at(point));
	up  = curve->getBinormalList().at(point);
	xRight = 0.5; xLeft = -xRight; yTop = 0.5; yBot = -yTop; N = 0.01; F = 1000;

	camara = new Camara(*eye, *look, *up, xRight, xLeft,yTop, yBot, N, F);
	startCam();
	
	glutMainLoop();
	unlinkAndFree();
	return 0;
}
