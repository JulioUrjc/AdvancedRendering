#include "Ray.h"

// Constructor
Ray::Ray(Vector origin, Vector direction, int numRebounds){
	this->origin = origin;
	this->direction = direction;
	restRebounds = numRebounds;
}

//Check if ray is intersecting with a sphere
//float Ray::collisionSphere(SceneSphere* sphere){
//
//	//Translate and scale sphere.
//	Vector center = sphere->getGlobalCenter();
//	float radius = sphere->getGlobalRadius();
//
//	//Vector from sphere center to ray origin (C - P)
//	Vector toCenter = origin - center;
//
//	float a = direction.Dot(direction);
//	float b = 2 * direction.Dot(origin);
//	float c = origin.Dot(origin) - pow(radius, 2);
//	// -b +/- sqrt(b^2-4ac)/ 2a
//	float disc = pow(b, 2) - 4 * a*c;
//
//	if (disc >= 0){
//		if (disc == 0){			//Hay una solución, rayo tangente a la esfera
//			return -b / 2*a;
//		}else{					//Hay dos soluciones, rayo secante
//			float sqrt = glm::sqrt(disc);
//			return glm::min((-b-sqrt) / 2*a, (-b+sqrt) / 2*a);
//		}
//	}
//	return -1;
//}

float Ray::collisionSphere(SceneSphere* sphere){
	//Translate and scale sphere. No rotations, it's a sphere!
	Vector center = sphere->getGlobalCenter();

	//It's implicit, so use a mean coord
	float radius = sphere->getGlobalRadius();

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
				t = glm::min(projection - sq, projection + sq);
				return t;
			}
		}
	}

	return -1;
}

// Check if ray is intersecting with a triangle
// Moller & Trumbore Method 2003 - Tema Traza de rayos transparencia 26
float Ray::collisionTriangle(SceneTriangle* triangle, Vector* intersecCoord){
	Vector edge1, edge2, pvec, qvec, tvec;
	float det, inv_det, u, v;
	float epsilon = 0.00001;

	// Get global vertex (translation, scale and rotation)
	Vector pointA = triangle->getGlobalVertex(0);
	Vector pointB = triangle->getGlobalVertex(1);
	Vector pointC = triangle->getGlobalVertex(2);

	edge1 = pointB - pointA;
	edge2 = pointC - pointA;

	pvec = direction.Cross(edge2);

	det = edge1.Dot(pvec);

	//No intersection
	if ((det>-epsilon) && (det<epsilon))
		return -1;

	inv_det = 1/det;
	tvec = origin - pointA;

	//Calculate u
	u = (tvec.Dot(pvec))*inv_det;

	//No intersection
	if (u<0.0f || u>1.0f)
		return -1;

	qvec = tvec.Cross(edge1);

	//Calculate v
	v = (direction.Dot(qvec))*inv_det;

	//No intersection
	if ((v<0.0f) || ((u + v)>1.0f))
		return -1;

	//Calculate t
	float t = (edge2.Dot(qvec))*inv_det;

	//INTERSECTION!
	if (t>epsilon){
		*intersecCoord = Vector(u, v, 1 - u - v);
		return t;
	}
	 
	return -1;	//Line intersection?
}

// Return if ray is intersecting with a model
float Ray::testCollisionModel(SceneModel* model, int &modelTriangle, Vector* uvw, bool showBoundingSpheres, bool useBs)
{
	float alpha = INFINITY;
	//float t = -1;
	//Vector uvwTemp;
	//modelTriangle = -1;

	////First, check collision with bounding sphere
	//SceneSphere boundingSphere = model->boundingSphere;
	//float collides = (collisionSphere(&boundingSphere));

	////Show bounding spheres
	//if (showBoundingSpheres && useBs){
	//	modelTriangle = 0;
	//	return collides;
	//}

	//if (collides>0 && collides < INFINITY || !useBs)
	//{
	//	//For each triangle in model
	//	for (int i = 0; i<model->GetNumTriangles(); i++)
	//	{
	//		t = collisionTriangle(model->GetTriangle(i), &uvwTemp);

	//		if (t>0 && t<alpha)
	//		{
	//			alpha = t;
	//			uvw->x = uvwTemp.x;
	//			uvw->y = uvwTemp.y;
	//			uvw->z = uvwTemp.z;
	//			modelTriangle = i;
	//		}
	//	}
	//}
	return alpha;
}

Vector Ray::collisionShadow(Scene &scene, int ignoreObject){
	
	Vector transparency(1, 1, 1);
	int object = -1;
	int modelTriangle = -1;

	barycCoord= Vector(0, 0, 0);
	minDistance = INFINITY;


	for (int i = 0; i<scene.GetNumObjects(); ++i){
		if (i != ignoreObject){
			float distance = -1;
			int modObject = -1;
			SceneObject* obj = scene.GetObject(i);

			if (obj->IsSphere()){
				distance = collisionSphere((SceneSphere*)obj);
			}else if (obj->IsTriangle()){
				distance = collisionTriangle((SceneTriangle*)obj, &barycCoord);
			}else if (obj->IsModel()){
				//distance = testCollisionModel((SceneModel*)so, modelObject, &uvw, scene.showBoundingSpheres, scene.useBs);
			}

			//If closer
			if (distance > 0 && distance < minDistance){
				minDistance = distance;
				object = i;
				modelTriangle = modObject;
			}
		}
	}

	if (minDistance<INFINITY){
		SceneObject *intersObj = scene.GetObject(object);
		Vector transparent;

		if (intersObj->IsSphere()){
			SceneMaterial *material = scene.GetMaterial(((SceneSphere *)intersObj)->material);
			transparency = material->transparent;

		}else if (intersObj->IsTriangle() || intersObj->IsModel()){
			SceneTriangle *triangle;

			if (intersObj->IsTriangle())
				triangle = (SceneTriangle *)intersObj;
			else
				triangle = ((SceneModel *)intersObj)->GetTriangle(modelTriangle);

			// Interpolate material
			SceneMaterial *material0 = scene.GetMaterial(triangle->material[0]);
			SceneMaterial *material1 = scene.GetMaterial(triangle->material[1]);
			SceneMaterial *material2 = scene.GetMaterial(triangle->material[2]);

			transparency = material0->transparent*barycCoord.z + material1->transparent*barycCoord.x + material2->transparent*barycCoord.y;
		}
	}
	return transparency;
}

Vector Ray::phong(Vector diffuseMat, Vector specularMat, float shininess, Vector point, Vector normal, Vector eye, Scene &scene){
	
	//Ambient
	Vector ambient = scene.GetBackground().ambientLight;
	ambient.x = glm::clamp(ambient.x, 0.0f, 1.0f);
	ambient.y = glm::clamp(ambient.y, 0.0f, 1.0f);
	ambient.z = glm::clamp(ambient.z, 0.0f, 1.0f);

	Vector color = ambient*0.3*diffuseMat;

	//For each light
	for (int i = 0; i<scene.GetNumLights(); i++){
		SceneLight *light = scene.GetLight(i);

		//Light ray
		 Vector ray = (light->position - point).Normalize();
		//Throw shadow ray to the light
		 Ray shadowRay(point, ray, 0);
		//Collision detected -> Shadow
		 Vector shadowValue = shadowRay.collisionShadow(scene, collidedObject);

		if (shadowValue.Magnitude()>0){
			//Attenuation
			float distance = (light->position - point).Magnitude();
			float att = glm::min(1.0f / (light->attenuationConstant + light->attenuationLinear*distance + light->attenuationQuadratic*distance*distance), 1.0f);

			//Diffuse component
			Vector diffuse = diffuseMat * ray.Dot(normal);
			diffuse.x = glm::clamp(diffuse.x, 0.0f, 1.0f);
			diffuse.y = glm::clamp(diffuse.y, 0.0f, 1.0f);
			diffuse.z = glm::clamp(diffuse.z, 0.0f, 1.0f);

			//Specular component
			Vector reflected = normal*ray.Dot(normal) * 2 - ray;
			Vector specular = specularMat * pow(reflected.Dot(eye), shininess);
			specular.x = glm::clamp(specular.x, 0.0f, 1.0f);
			specular.y = glm::clamp(specular.y, 0.0f, 1.0f);
			specular.z = glm::clamp(specular.z, 0.0f, 1.0f);

			color = color + light->color*(diffuse + specular)*att;

			//Shadow
			color = color * shadowValue;
		}
	}
	return color;
}

Vector Ray::collisions(Scene &scene, int ignoreObject){

	//int modelObject;
	int object = -1;
	int modelTriangle = -1;

	barycCoord = Vector(0, 0, 0);
	minDistance = INFINITY;

	for (int i = 0; i<scene.GetNumObjects(); ++i){
		if (i != ignoreObject){
			float distance = -1;
			int modObject = -1;
			SceneObject* obj = scene.GetObject(i);

			if (obj->IsSphere())	{
				distance = collisionSphere((SceneSphere*)obj);
			}
			else if (obj->IsTriangle()){
				distance = collisionTriangle((SceneTriangle*)obj, &barycCoord);
			}
			else{
				//distance = testCollisionModel((SceneModel*)obj, modObject, &intersecCoord, scene.showBoundingSpheres, scene.useBs);
			}

			//If closer
			if (distance > 0 && distance < minDistance){
				minDistance = distance;
				object = i;
				modelTriangle = modObject;
			}
		}
	} 

	collidedObject = object;

	if (minDistance < INFINITY){	//If there is a collision

		SceneObject *intersectObject = scene.GetObject(object);

		// Phong collor
		Vector myColor, reflective, transparent, refraction, normal;
		
		// Get eye vector
		Vector eye = (scene.GetCamera().GetPosition() - getIntersectionPoint()).Normalize();

		if (intersectObject->IsSphere()){
			SceneSphere *sphere = (SceneSphere *)intersectObject;

			// Get sphere normal: vector from center to intersection point
			normal = (getIntersectionPoint() - sphere->getGlobalCenter()).Normalize();

			//Get Sphere material
			SceneMaterial *material = scene.GetMaterial(sphere->material);
			reflective = material->reflective;
			refraction = material->refraction_index;
			transparent = material->transparent;

			myColor = phong(material->diffuse, material->specular, material->shininess, getIntersectionPoint(), normal, eye, scene);
		
		}else if (intersectObject->IsTriangle() || intersectObject->IsModel()){
			
			SceneTriangle *triangle;
			//Get the triangle
			if (intersectObject->IsTriangle())
				triangle = (SceneTriangle *)intersectObject;
			else
			{
				triangle = ((SceneModel *)intersectObject)->GetTriangle(modelTriangle);
			}

			//Get triangle normal interpolating normal from vertex
			normal = triangle->normal[0] * barycCoord.z + triangle->normal[1] * barycCoord.x + triangle->normal[2] * barycCoord.y;

			//Interpolate material
			SceneMaterial *material0 = scene.GetMaterial(triangle->material[0]);
			SceneMaterial *material1 = scene.GetMaterial(triangle->material[1]);
			SceneMaterial *material2 = scene.GetMaterial(triangle->material[2]);

			// Interpolate texture coordinates
			float texU = triangle->u[0] * barycCoord.z + triangle->u[1] * barycCoord.x + triangle->u[2] * barycCoord.y;
			float texV = triangle->v[0] * barycCoord.z + triangle->v[1] * barycCoord.x + triangle->v[2] * barycCoord.y;

			if (texU<0)
				texU = 0;
			if (texV <0)
				texV = 0;

			// Get diffuse or texture
			Vector diffuse0 = material0->diffuse;
			if (material0->texture != "")
				diffuse0 = material0->GetTextureColor(texU, texV) / 255.0f;

			Vector diffuse1 = material1->diffuse;
			if (material1->texture != "")
				diffuse1 = material1->GetTextureColor(texU, texV) / 255.0f;

			Vector diffuse2 = material2->diffuse;
			if (material2->texture != "")
				diffuse2 = material2->GetTextureColor(texU, texV) / 255.0f;

			Vector diffuse = diffuse0 * barycCoord.z + diffuse1 * barycCoord.x + diffuse2 * barycCoord.y;
			Vector specular = material0->specular * barycCoord.z + material1->specular * barycCoord.x + material2->specular * barycCoord.y;
			float shininess = material0->shininess * barycCoord.z + material1->shininess * barycCoord.x + material2->shininess * barycCoord.y;
			reflective = material0->reflective * barycCoord.z + material1->reflective * barycCoord.x + material2->reflective * barycCoord.y;
			refraction = material0->refraction_index * barycCoord.z + material1->refraction_index * barycCoord.x + material2->refraction_index * barycCoord.y;
			transparent = material0->transparent * barycCoord.z + material1->transparent * barycCoord.x + material2->transparent * barycCoord.y;

			myColor = phong(diffuse, specular, shininess, getIntersectionPoint(), normal, eye, scene);
		}

		//Recursive rays
		//If last recursion or not reflective, return object color
		if (restRebounds == 0)
			return myColor;

		//If not, throw another ray
		if (reflective.Magnitude() > 0){
			//Reflect ray
			Vector reflected = direction - normal*direction.Dot(normal) * 2;

			Ray newRay(getIntersectionPoint(), reflected, restRebounds - 1);
			Vector reflectedColor = newRay.collisions(scene, collidedObject);

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
			Ray redRay(getIntersectionPoint(), refractedRed, restRebounds - 1);
			Vector redRefractedColor = redRay.collisions(scene, collidedObject)*Vector(1, 0, 0);

			//Green component of refraction
			glm::vec3 greenRefraction = glm::refract(vDirection, vNormal, refraction.y);
			Vector refractedGreen = Vector(greenRefraction.x, greenRefraction.y, greenRefraction.z, 0);
			Ray greenRay(getIntersectionPoint(), refractedGreen, restRebounds - 1);
			Vector greenRefractedColor = greenRay.collisions(scene, collidedObject)*Vector(0, 1, 0);

			//Blue component of refraction
			glm::vec3 blueRefraction = glm::refract(vDirection, vNormal, refraction.y);
			Vector refractedBlue = Vector(blueRefraction.x, blueRefraction.y, blueRefraction.z, 0);
			Ray blueRay(getIntersectionPoint(), refractedBlue, restRebounds - 1);
			Vector blueRefractedColor = blueRay.collisions(scene, collidedObject)*Vector(0, 0, 1);

			Vector refractedColor = redRefractedColor + greenRefractedColor + blueRefractedColor;

			Vector inverseTransparent = Vector(1-transparent.x, 1-transparent.y, 1-transparent.z);
			return refractedColor*transparent + myColor*inverseTransparent;
		}

		//There's been a collision and it's not reflective or refractive, so return my color
		return myColor;
	}

	//If there's not a collision, return background
	return scene.GetBackground().color;
}