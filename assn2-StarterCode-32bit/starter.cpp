/*
  15-462 Computer Graphics I
  Assignment 2: Roller Coaster
  C++ starter code
  Author: rtark
  Aug 2007

  Name: [Name]
  AndrewID: [ID]
*/

#include <stdio.h>
#include <stdlib.h>
#include "pic.h"

#ifdef _OS_X_
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>	
//#include "/opt/local/include/gd.h"

#elif defined(WIN32)
/* !NOTE! libgd has problems with image functions that use the FILE data structure in Windows according to the FAQ.... */
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

#include "Splines.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

/* --- Global State Variables --- */

/* - Menu State Identifier - */
int g_iMenuId;

/* - Mouse State Variables - */
int g_vMousePos[2] = {0, 0};
int g_iLeftMouseButton = 0;    /* 1 if pressed, 0 if not */
int g_iMiddleMouseButton = 0;
int g_iRightMouseButton = 0;

/* - SplineList Variable for the Roller Coaster Track - */
SplineList g_Splines;

/*
	saveScreenshot - Writes a screenshot to the specified filename in JPEG
*/
void saveScreenshot (char *filename)
{
	Pic *in = NULL;

	if (filename == NULL)
		return;

	/* Allocate a picture buffer */
	in = pic_alloc (WINDOW_WIDTH, WINDOW_HEIGHT, 3, NULL);

	printf("File to save to: %s\n", filename);

	/* Loop over each row of the image and copy into the image */
	for (int i = WINDOW_HEIGHT - 1; i >= 0; i--)
	{
		glReadPixels(0, WINDOW_HEIGHT - 1 - i, WINDOW_WIDTH, 1, GL_RGB,
						GL_UNSIGNED_BYTE, &in->pix[i*in->nx*in->bpp]);
	}

	/* Output the file */
	if (jpeg_write(filename, in))
	{
		printf("File saved Successfully\n");
	}
	else
	{
		printf("Error in Saving\n");
	}

	/* Free memory used by image */
	pic_free(in);
}

/*
	loadTexture - Loads a texture from a JPEG file to memory and returns the handle
		Note: pWidth and pHeight are pointers to return imageWidth and imageHeight
*/
GLuint loadTexture (char *filename, int *pWidth = NULL, int *pHeight = NULL)
{
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

/*
	myinit - Function to add your initialization code
*/
void myinit()
{
	/* setup gl view here */
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

/*
	display - Function to modify with your heightfield rendering code (Currently displays a simple cube)
*/
void display()
{
	/* draw 1x1 cube about origin
		replace this code with your height field implementation
		you may also want to precede it with your rotation/translation/scaling */

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_POLYGON);

	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(-0.5, -0.5, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-0.5, 0.5, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(0.5, 0.5, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(0.5, -0.5, 0.0);

	glEnd();

	glutSwapBuffers ();
}

/*
	menufunc - Menu Event-handler
*/
void menufunc(int value)
{
	switch (value)
	{
	case 0:
		exit(0);
		break;
	}
}

/*
	doIdle - The idle-function that can be used to update the screen
*/
void doIdle()
{
	/* do some stuff... */

	/* make the screen update */
	glutPostRedisplay();
}

/*
	mousedrag - converts mouse drags into information about rotation/translation/scaling
*/
void mousedrag(int x, int y)
{
	int vMouseDelta[2] = {x - g_vMousePos[0], y - g_vMousePos[1]};

	g_vMousePos[0] = x;
	g_vMousePos[1] = y;
}

/*
	mouseidle - Idle mouse movement callback function
*/
void mouseidle(int x, int y)
{
	g_vMousePos[0] = x;
	g_vMousePos[1] = y;
}

/*
	mousebutton - Sets the global mouse states according to the actions
*/
void mousebutton(int button, int state, int x, int y)
{
	switch (button)
	{
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

/*
	main - The Main Function
*/
int main (int argc, char ** argv)
{
	if (argc < 2)
	{
		printf ("usage: %s trackfile\n", argv[0]);
		exit(1);
	}

	if (!g_Splines.LoadSplines (argv[1]))
	{
		printf ("failed to load splines\n");
		exit(1);
	}

	glutInit(&argc,argv);

	/*
	create a window here..should be double buffered and use depth testing
	window should be 640x480

	the code past here will segfault if you don't have a window set up....
	replace the exit once you add those calls.
	*/
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutCreateWindow("15-462 Assignment 2 - Roller Coaster");

	/* tells glut to use a particular display function to redraw */
	glutDisplayFunc(display);

	/* allow the user to quit using the right mouse button menu */
	g_iMenuId = glutCreateMenu(menufunc);
	glutSetMenu(g_iMenuId);
	glutAddMenuEntry("Quit",0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* replace with any animate code */
	glutIdleFunc(doIdle);

	/* callback for mouse drags */
	glutMotionFunc(mousedrag);
	/* callback for idle mouse movement */
	glutPassiveMotionFunc(mouseidle);
	/* callback for mouse button changes */
	glutMouseFunc(mousebutton);

	/* do initialization */
	myinit();

	glutMainLoop();
	return 0;
}
