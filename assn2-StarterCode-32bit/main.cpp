/*
	Simulacion del Torrente Sanguineo
	RA - Raquel Peces & Julio Martin
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
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
const int curveSteps = 25;
const float curveT = 0.7f;
BezierCurve* curve;

/* - Vein Variable - */
const int veinSides = 10;
const float veinRadius = 0.7f;
Vein* vein;

/* - Blood Variable - */
 const int numRedCorpuscles = 0;
 const int numWhiteCorpuscles = 1;
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

/* Control del numero de captura */
int captura = 0;

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
//Destroy and free memory
void unlinkAndFree(){
	vein->freeMemory();
}

void key(unsigned char key, int x, int y){
	bool need_redisplay = true;
	switch (key) {
	//Escape key
	case 27:  
		unlinkAndFree();
		exit(0);
		break;
	// Teclas de Avanzar
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

	// Capturas de pantalla
	case '0':
		string folder = "./Capturas/screenShot" + to_string(captura) + ".jpg";
		char *fileName = new char[folder.length() + 1]; strcpy(fileName, folder.c_str());
		saveScreenshot(fileName);
		delete[] fileName;
		captura++;
		break;
	}
}

/*	display - Function to modify with your heightfield rendering code (Currently displays a simple cube) */
void display(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	curve->draw(modo);

	if (modo == 1){
		glPolygonMode(GL_FRONT, GL_POINT);
	}
	else if (modo == 2){
		glPolygonMode(GL_FRONT, GL_LINE);
	}
	else{
		glPolygonMode(GL_FRONT, GL_FILL);
	}

	//vein->draw(camara);
	vein->draw(modo);
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
		modo = (modo% 3) + 1;
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
void startGlut(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Torrente Sanguineo - Julio y Raquel");
	
	glutDisplayFunc(display);			/* tells glut to use a particular display function to redraw */
	glutKeyboardFunc(key);				/* callback for keyboard */
	glutIdleFunc(doIdle);				/* replace with any animate code */
	glutMotionFunc(mousedrag);			/* callback for mouse drags */
	glutPassiveMotionFunc(mouseidle);	/* callback for idle mouse movement */
	glutMouseFunc(mousebutton);			/* callback for mouse button changes */

	/* allow the user to quit using the right mouse button menu */
	g_iMenuId = glutCreateMenu(menufunc);
	glutSetMenu(g_iMenuId);
	glutAddMenuEntry("Quit", 0);
	glutAddMenuEntry("ModoVisualiz", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void startCam(){	
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT, GL_LINE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	camara->fijarCam();   //// Camera set up
	camara->ortogonal();  //// Frustum set up

	//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  //// Viewport set up
}

/*	main - The Main Function */
int main (int argc, char ** argv){

	/* do initialization */
	startGlut(argc, argv);
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
	blood = new Blood(numRedCorpuscles, numWhiteCorpuscles, curve, veinRadius);
	
	/* Camera parameters */
	std::cout << "Colocando la camara..." << std::endl;
	eye = curve->getPointList().at(point);
	look= eye->addition(curve->getTangentList().at(point));
	up  = curve->getBinormalList().at(point);
	xRight = 0.5; xLeft = -xRight; yTop = 0.5; yBot = -yTop; N = 0.01; F = 100;

	camara = new Camara(*eye, *look, *up, xRight, xLeft,yTop, yBot, N, F, curve);
	startCam();
	
	glutMainLoop();
	/* Liberando recursos */
	unlinkAndFree(); 
	return 0;
}
