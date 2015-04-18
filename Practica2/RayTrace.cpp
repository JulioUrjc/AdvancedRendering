#ifdef _OS_X_
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>	

#elif defined(WIN32)
#include <windows.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"

#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "Scene.h"
#include "RayTrace.h"


void RayTrace::initialize(){
	Scene &la_escena = m_Scene;
	Camera &la_camara = la_escena.GetCamera();
	camPos = la_camara.GetPosition();

	fovX = la_camara.GetFOV()* (Scene::WINDOW_WIDTH/Scene::WINDOW_HEIGHT) * M_PI / 180;
	fovY = la_camara.GetFOV()* M_PI / 180; 

	look = (la_camara.GetTarget() - camPos).Normalize();
	up = la_camara.GetUp().Normalize();
	normal = up.Cross(look).Normalize();
}

// -- Main Functions --
// - CalculatePixel - Returns the Computed Pixel for that screen coordinate
Vector RayTrace::CalculatePixel (int screenX, int screenY){
   /* -- How to Implement a Ray Tracer --

   This computed pixel will take into account the camera and the scene
   and return a Vector of <Red, Green, Blue>, each component ranging from 0.0 to 1.0

   In order to start working on computing the color of this pixel,
   you will need to cast a ray from your current camera position
   to the image-plane at the given screenX and screenY
   coordinates and figure out how/where this ray intersects with 
   the objects in the scene descriptor.
   The Scene Class exposes all of the scene's variables for you 
   through its functions such as m_Scene.GetBackground (), m_Scene.GetNumLights (), 
   etc. so you will need to use those to learn about the World.

   To determine if your ray intersects with an object in the scene, 
   you must calculate where your objects are in 3D-space [using 
   the object's scale, rotation, and position is extra credit]
   and mathematically solving for an intersection point corresponding to that object.

   For example, for each of the spheres in the scene, you can use 
   the equation of a sphere/ellipsoid to determine whether or not 
   your ray from the camera's position to the screen-pixel intersects 
   with the object, then from the incident angle to the normal at 
   the contact, you can determine the reflected ray, and recursively 
   repeat this process capped by a number of iterations (e.g. 10).

   Using the lighting equation & texture to calculate the color at every 
   intersection point and adding its fractional amount (determined by the material)
   will get you a final color that returns to the eye at this point.*/

	initialize();
	
	float width2  = Scene::WINDOW_WIDTH  / 2;
	float height2 = Scene::WINDOW_HEIGHT / 2;

	float alpha = tanf(fovX/2) * ((width2-screenX)/width2);  // width2-screenX si no sale en forma de espejo
	float beta  = tanf(fovY/2) * ((screenY-height2)/height2);

	Vector rayDirection = (look + normal*alpha + up*beta).Normalize();

   if ((screenX <0 || screenX>Scene::WINDOW_WIDTH-1) || (screenY<0 || screenY>Scene::WINDOW_HEIGHT-1)){  
      return Vector (0.0f, 0.0f, 0.0f);	// Off the screen, return black
   }

   Ray ray(camPos, rayDirection, REBOUNDS);	// REBOUNDS defined in RayTrace.h
   return ray.collisions(m_Scene, -1);		// In the first rebounce not ignore any object

   //return Vector(0.0f, 0.0f, 0.0f);
}
