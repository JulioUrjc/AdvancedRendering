#include "Ray.h"


Ray::Ray(Vector origin, Vector direction, int numRebounds){
	this->origin = origin;
	this->direction = direction;
	restRebounds = numRebounds;
}

//Check if ray is intersecting with a sphere
float Ray::collisionSphere(SceneSphere* sphere){

	//Translate and scale sphere. No rotations, it's a sphere!
	Vector center = sphere->getTransformedCenter();

	//It's implicit, so use a mean coord
	float radius = sphere->getTransformedRadius();

	//Vector from ray origin to sphere center (C - O)
	Vector toCenter = center - origin;

	float dist = toCenter.Dot(toCenter) - radius * radius;

	//Just if ray origin is outside the sphere
	if (dist > 0)
	{
		float projection = toCenter.Dot(direction);

		//Check if sphere is in front of the camera, using the angle
		if (projection >= 0)
		{
			float p2 = projection * projection;

			//Ray tangent to the sphere
			float t;
			if (dist == p2)
			{
				t = projection;
				return t;
			}

			//Else, just solve the ecuation
			else if (dist < p2)
			{
				float sq = sqrt(p2 - dist);

				//2 solutions, so return the min (nearest)
				t = min(projection - sq, projection + sq);
				return t;
			}
		}
	}

	return -1;
}

Vector Ray::testCollisions(Scene &scene){

	Vector intersecCoord(0, 0, 0);
	int modelObject;

	alpha = INFINITY;
	int object = -1;
	int modelTriangle = -1;

	for (int i = 0; i<scene.GetNumObjects(); ++i){
			float distance = -1;
			SceneObject* obj = scene.GetObject(i);
			int modelObject = -1;

			if (obj->IsSphere())	{
				distance = testCollisionSphere((SceneSphere*)obj);
			}
			else if (obj->IsTriangle()){
				distance = testCollisionTriangle((SceneTriangle*)obj, &intersecCoord);
			}
			else{
				//distance = testCollisionModel((SceneModel*)obj, modelObject, &intersecCoord, scene.showBoundingSpheres, scene.useBs);
			}

			//If closer
			if (distance>0 && distance<alpha){
				alpha = distance;
				barycCoord = intersecCoord;
				object = i;
				modelTriangle = modelObject;
			}
	} // End FOR

	collidedObject = object;

	//If there is a collision
	if (alpha<INFINITE){

		SceneObject *intersectObject = scene.GetObject(object);

		//Get my own phong collor
		Vector myColor;
		Vector reflective;
		Vector transparent;
		Vector refraction;
		Vector normal;
		//Get eye vector
		Vector eye = (scene.GetCamera().GetPosition() - getIntersectionPoint()).Normalize();
		if (intersectObject->IsSphere())
		{
			SceneSphere *sphere = (SceneSphere *)intersectObject;

			//Get sphere normal: vector from center to intersection point
			normal = (getIntersectionPoint() - sphere->getTransformedCenter()).Normalize();

			//Get Sphere material
			SceneMaterial *material = scene.GetMaterial(sphere->material);
			reflective = material->reflective;
			refraction = material->refraction_index;
			transparent = material->transparent;

			myColor = phongIllumination(material->diffuse, material->specular, material->shininess, getIntersectionPoint(), normal, eye, scene);
		}
		else if (intersectObject->IsTriangle() || intersectObject->IsModel())
		{
			SceneTriangle *triangle;
			//Get the triangle
			if (intersectObject->IsTriangle())
				triangle = (SceneTriangle *)intersectObject;
			else
			{
				triangle = ((SceneModel *)intersectObject)->GetTriangle(modelTriangle);
			}

			//Get triangle normal interpolating normal from vertex
			normal = barycentricInterpolation(u, v, w, triangle->normal[0], triangle->normal[1], triangle->normal[2]);

			//Interpolate material
			SceneMaterial *material0 = scene.GetMaterial(triangle->material[0]);
			SceneMaterial *material1 = scene.GetMaterial(triangle->material[1]);
			SceneMaterial *material2 = scene.GetMaterial(triangle->material[2]);

			//Interpolate tex coordinates
			float texU = lerp(u, v, w, triangle->u[0], triangle->u[1], triangle->u[2]);
			float texV = lerp(u, v, w, triangle->v[0], triangle->v[1], triangle->v[2]);

			if (texU<0)
				texU = 0;
			if (texV <0)
				texV = 0;

			//Get diffuse or texture
			Vector diffuse0 = material0->diffuse;
			if (material0->texture != "")
				diffuse0 = material0->GetTextureColor(texU, texV) / 255.0f;

			Vector diffuse1 = material1->diffuse;
			if (material1->texture != "")
				diffuse1 = material1->GetTextureColor(texU, texV) / 255.0f;

			Vector diffuse2 = material2->diffuse;
			if (material2->texture != "")
				diffuse2 = material2->GetTextureColor(texU, texV) / 255.0f;

			Vector diffuse = barycentricInterpolation(u, v, w, diffuse0, diffuse1, diffuse2);
			Vector specular = barycentricInterpolation(u, v, w, material0->specular, material1->specular, material2->specular);
			float shininess = lerp(u, v, w, material0->shininess, material1->shininess, material2->shininess);
			reflective = barycentricInterpolation(u, v, w, material0->reflective, material1->reflective, material2->reflective);
			refraction = barycentricInterpolation(u, v, w, material0->refraction_index, material1->refraction_index, material2->refraction_index);
			transparent = barycentricInterpolation(u, v, w, material0->transparent, material1->transparent, material2->transparent);

			myColor = phongIllumination(diffuse, specular, shininess, getIntersectionPoint(), normal, eye, scene);
		}

		//Recursive rays
		//If last recursion or not reflective, return object color
		if (energy == 0)
			return myColor;

		//If not, throw another ray
		if (reflective.Magnitude() > 0){
			//Reflect ray
			Vector reflected = direction - normal*direction.Dot(normal) * 2;

			Ray newRay(getIntersectionPoint(), reflected, energy - 1);
			Vector reflectedColor = newRay.testCollisions(scene, collidedObject);

			Vector inverseReflective = Vector(1 - reflective.x, 1 - reflective.y, 1 - reflective.z);
			return reflectedColor*reflective + myColor*inverseReflective;
		}

		//Refraction
		if (transparent.Magnitude() > 0){
			//Using glm to refract the ray
			glm::vec3 vDirection(direction.x, direction.y, direction.z);
			glm::vec3 vNormal(normal.x, normal.y, normal.z);

			//Red component of refraction
			glm::vec3 redRefraction = glm::refract(vDirection, vNormal, refraction.x);
			Vector refractedRed = Vector(redRefraction.x, redRefraction.y, redRefraction.z, 0);
			Ray redRay(getIntersectionPoint(), refractedRed, energy - 1);
			Vector redRefractedColor = redRay.testCollisions(scene, collidedObject)*Vector(1, 0, 0);

			//Green component of refraction
			glm::vec3 greenRefraction = glm::refract(vDirection, vNormal, refraction.y);
			Vector refractedGreen = Vector(greenRefraction.x, greenRefraction.y, greenRefraction.z, 0);
			Ray greenRay(getIntersectionPoint(), refractedGreen, energy - 1);
			Vector greenRefractedColor = greenRay.testCollisions(scene, collidedObject)*Vector(0, 1, 0);

			//Blue component of refraction
			glm::vec3 blueRefraction = glm::refract(vDirection, vNormal, refraction.y);
			Vector refractedBlue = Vector(blueRefraction.x, blueRefraction.y, blueRefraction.z, 0);
			Ray blueRay(getIntersectionPoint(), refractedBlue, energy - 1);
			Vector blueRefractedColor = blueRay.testCollisions(scene, collidedObject)*Vector(0, 0, 1);

			Vector refractedColor = redRefractedColor + greenRefractedColor + blueRefractedColor;

			Vector inverseTransparent = Vector(1 - transparent.x, 1 - transparent.y, 1 - transparent.z);
			return refractedColor*transparent + myColor*inverseTransparent;

		}

		//There's been a collision and it's not reflective or refractive, so return my color
		return myColor;
	}

	//If there's not a collision, return background
	return scene.GetBackground().color;
}

