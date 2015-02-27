/*
	Simulacion del Torrente Sanguineo
	RA - Raquel Peces & Julio Martin
*/

#include <stdio.h>
#include <stdlib.h>
#include "pic.h"

#ifdef WIN32

#include <windows.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"
#pragma comment(lib, "win32/libtiff_i.lib")
#pragma comment(lib, "win32/jpeg.lib")
#pragma comment(lib, "win32/libtiff.lib")
#pragma comment(lib, "win32/libpicio.lib")
#pragma comment(lib, "glut32.lib")

#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "BezierCurve.h"
#include "Vein.h"
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
BezierCurve *curve;
Vein *vein;

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

/*	display - Function to modify with your heightfield rendering code (Currently displays a simple cube) */
void display(){
	/* draw 1x1 cube about origin you may also want to precede it with your rotation/translation/scaling */

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*glBegin(GL_POLYGON);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(-0.5, -0.5, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-0.5, 0.5, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(0.5, 0.5, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(0.5, -0.5, 0.0);

	glEnd();*/
	
	vein->draw(false);
	glutSwapBuffers();
}

/*	menufunc - Menu Event-handler */
void menufunc(int value){
	switch (value){
	case 0:
		exit(0);
		break;
	}
}

/*	myinit - Function to add your initialization code */
void glInit(){
	// Por defecto
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Torrente Sanguineo");

	/* tells glut to use a particular display function to redraw */
	glutDisplayFunc(display);

	/* allow the user to quit using the right mouse button menu */
	g_iMenuId = glutCreateMenu(menufunc);
	glutSetMenu(g_iMenuId);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* replace with any animate code */
	glutIdleFunc(doIdle);

	/* callback for mouse drags */
	glutMotionFunc(mousedrag);
	/* callback for idle mouse movement */
	glutPassiveMotionFunc(mouseidle);
	/* callback for mouse button changes */
	glutMouseFunc(mousebutton);
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Viewing frustum parameters
	GLdouble xRight = 10, xLeft = -xRight, yTop = 10, yBot = -yTop, N = 1, F = 1000;

	//// Camera parameters
	GLdouble eyeX = 100.0, eyeY = 100.0, eyeZ = 100.0;
	GLdouble lookX = 0.0, lookY = 0.0, lookZ = 0.0;
	GLdouble upX = 0, upY = 1, upZ = 0;

	//// Axis angles
	GLfloat angleX = 0, angleY = 0, angleZ = 0;

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);
	glMaterialf(GL_FRONT, GL_SHININESS, 0.1f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	//// buildSceneObjects();

	//// Camera set up
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);

	//// Frustum set up
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(xLeft, xRight, yBot, yTop, N, F);

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

/*	main - The Main Function */
int main (int argc, char ** argv){

	glutInit(&argc,argv);
	
	glInit(); /* do initialization */
	curve = new BezierCurve();
	vein = new Vein(10,20,curve);
	glutMainLoop();
	return 0;
}
