#pragma once
/* Clase que define un rayo Raquel Peces y Julio Martín */

#include "Utils.h"
#include "Scene.h"
#include <Math.h>

class Ray{

	public:
		Ray(Vector origin, Vector direction, int numRebounds);

		// Check if ray is intersecting with a sphere
		float collisionSphere(SceneSphere* sphere);

		// Check if ray is intersecting with a triangle
		float collisionTriangle(SceneTriangle* triangle, Vector* intersecCoord);

		// Return if ray is intersecting with a model
		float collisionExternModel(SceneModel* model, int &modelTriangle, Vector* intersecCoord);

		// Returns the transparency to multiply to the color (0,0,0) total shadow, (1,1,1) no shadow
		Vector collisionShadow(Scene &scene, int ignoreObject);

		// Return phong color
		Vector phong(Vector diffuseMat, Vector specularMat, float shininess, Vector point, Vector normal, Vector eye, Scene &scene);

		// Returns the color of the object intersected
		Vector collisions(Scene &scene, int ignoreObject);

		//Getters
		inline Vector getRayDirection(){ return direction; }
		inline Vector getIntersectionPoint(){ return origin + direction*minDistance; }
		inline float getMinDistance(){ return minDistance; }
	
	private:	
		Vector origin;
		Vector direction;
		float minDistance;
		int collidedObject;

		//Ray rebounds
		int restRebounds;

		//Barycentric coordinates in triangle intersection
		Vector barycCoord;
};

