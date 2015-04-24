#pragma once
/* Clase que define un rayo Raquel Peces y Julio Martín */

#include "Utils.h"
#include "Scene.h"
#include <Math.h>

class Ray{

	public:
		Ray(Vector origin, Vector direction, int numRebounds);

		// Intersection with a sphere
		float collisionSphere(SceneSphere* sphere);

		// Intersection with a triangle
		float collisionTriangle(SceneTriangle* triangle, Vector* intersecCoord);

		// Intersection with a model
		float colExtModel(SceneModel* model, int &modelTriangle, Vector* intersecCoord);

		// Shadows
		Vector collisionShadow(Scene &scene, int ignoreObject);

		// Phong
		Vector phong(Vector diffuseMat, Vector specularMat, float shininess, Vector point, Vector normal, Vector look, Scene &scene);

		// Color of the object intersected
		Vector collisions(Scene &scene, int ignoreObject);

		// Getters
		inline Vector getRayDirection(){ return direction; }
		inline Vector getIntersectionPoint(){ return origin + direction*minDistance; }
		inline float getMinDistance(){ return minDistance; }
	
	private:	
		Vector origin;
		Vector direction;
		float minDistance;
		int collidedObject;

		int restRebounds;	// Rest of ray rebounds
		Vector barycCoord;	// Barycentric coordinates in triangle intersection
};

