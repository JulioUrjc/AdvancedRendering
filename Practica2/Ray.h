#pragma once
/* Clase que define un rayo Raquel Peces y Julio Martín */

#include "Utils.h"
#include "Scene.h"
#include <math.h>

class Ray{

	public:
		Ray(Vector origin, Vector direction, int numRebounds);

		// Check if ray is intersecting with a sphere
		float collisionSphere(SceneSphere* sphere);

		// Check if ray is intersecting with a triangle
		float collisionTriangle(SceneTriangle* triangle, Vector* uvw);

		// Returns the transparency to multiply to the color (0,0,0) total shadow, (1,1,1) no shadow
		Vector collisionsShadow(Scene &scene);

		// Returns the color of the object intersected
		Vector collisions(Scene &scene);

		//Getters
		Vector getRayDirection(){ return direction; }
		//Vector getIntersectionPoint(){ return origin+direction*alpha; }
		float getAlpha(){ return alpha; }
	
	private:	
		Vector origin;
		Vector direction;
		float alpha;
		int collidedObject;

		//Ray rebounds
		int restRebounds;

		//Barycentric coordinates in triangle intersection
		Vector barycCoord;
};

