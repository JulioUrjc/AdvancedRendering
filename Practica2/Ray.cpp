#include "Ray.h"
#include <iostream>
/* Clase que define un rayo Raquel Peces y Julio Martín */

/* Constructor */
Ray::Ray(Vector origin, Vector direction, int numRebounds){
	this->origin = origin;
	this->direction = direction;
	restRebounds = numRebounds;
}

/* Intersection with a sphere */
//float Ray::collisionSphere(SceneSphere* sphere){
//
//	//Translate and scale sphere
//	Vector center = sphere->getGlobalCenter();
//	float radius = sphere->getGlobalRadius();
//
//	// Vector from sphere center to ray origin (C - P)
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

/* Intersection with a sphere */
float Ray::collisionSphere(SceneSphere* sphere){
	
	// Translate and scale sphere
	Vector center = sphere->getGlobalCenter();
	float radius = sphere->getGlobalRadius();

	// Vector from ray origin to center (C - P)
	Vector vtoCenter = center - origin;
	float dist = vtoCenter.Dot(vtoCenter) - pow(radius, 2);

	
	if(dist>0){		    										         // Ray origin outside the sphere
		float projection = vtoCenter.Dot(direction);
		if (projection >= 0){									         // Angle between sphefere and camera
			float sqrProjec = pow(projection, 2);

			if (dist == sqrProjec){									     // Ray tangent to the sphere
				return projection;
			}else if (dist < sqrProjec){								 // Solve the ecuation
				float s = sqrt(sqrProjec - dist);
				return glm::min(projection - s, projection + s);		 // Return the min solution
			}
		}
	}
	return -1;
}

/* Intersection with a triangle */
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

	// No intersection
	if ((det>-epsilon) && (det<epsilon)) return -1;

	inv_det = 1/det;
	tvec = origin - pointA;

	// Calculate u
	u = (tvec.Dot(pvec))*inv_det;

	// No intersection
	if (u<0.0f || u>1.0f) return -1;

	qvec = tvec.Cross(edge1);

	// Calculate v
	v = (direction.Dot(qvec))*inv_det;

	// No intersection
	if ((v<0.0f) || ((u+v)>1.0f)) return -1;

	// Calculate t
	float t = (edge2.Dot(qvec))*inv_det;

	// INTERSECTION!
	if (t>epsilon){
		*intersecCoord = Vector(u, v, 1 - u - v);
		return t;
	}
	 
	return -1;	// Line
}

/* Intersection whit a model */
float Ray::colExtModel(SceneModel* m, int &mTriangle, Vector* intersecCoord){

	float minDistance = INFINITY;
	float d = -1.0f;
	Vector intersecCoordAux;
	mTriangle = -1;

	for(unsigned int i= 0; i < m->GetNumTriangles(); ++i){
		d = collisionTriangle(m->GetTriangle(i), &intersecCoordAux);

		if ((d>0) && (d<minDistance)){
			minDistance = d;
			intersecCoord = &intersecCoordAux;
			mTriangle = i;
		}
	}
	return minDistance;
}

/* Return shadows (0,0,0) shadow, (1,1,1) no shadow */
Vector Ray::collisionShadow(Scene &scene, int ignoredObject){
	
	Vector shadow(1,1,1);
	int object = -1;
	int modTriangle = -1;

	barycCoord = Vector(0,0,0);
	minDistance = INFINITY;


	for(unsigned int i= 0; i<scene.GetNumObjects(); ++i){
		if(i != ignoredObject){
			float distance = -1;
			int modObject = -1;
			SceneObject* obj = scene.GetObject(i);

			// Object Type
			if(obj->IsSphere()){
				distance = collisionSphere((SceneSphere*)obj);
			}else if (obj->IsTriangle()){
				distance = collisionTriangle((SceneTriangle*)obj, &barycCoord);
			}else if (obj->IsModel()){
				//distance = colExtModel((SceneModel*)obj, object, &barycCoord);
			}

			if((distance>0) && (distance<minDistance)){		// If is min distance
				minDistance = distance;
				object = i;
				modTriangle = modObject;
			}
		}
	}

	if(minDistance<INFINITY){
		SceneObject *intersObj = scene.GetObject(object);

		if (intersObj->IsSphere()){
			SceneMaterial *material = scene.GetMaterial(((SceneSphere *)intersObj)->material);
			shadow = material->transparent;
		}else if(intersObj->IsTriangle() || intersObj->IsModel()){
			
			SceneTriangle *tri;
			if (intersObj->IsTriangle())
				tri = (SceneTriangle*)intersObj;
			else
				tri = ((SceneModel*)intersObj)->GetTriangle(modTriangle);

			// Material
			SceneMaterial *mat0 = scene.GetMaterial(tri->material[0]);
			SceneMaterial *mat1 = scene.GetMaterial(tri->material[1]);
			SceneMaterial *mat2 = scene.GetMaterial(tri->material[2]);

			shadow = mat0->transparent*barycCoord.z + mat1->transparent*barycCoord.x + mat2->transparent*barycCoord.y;
		}
	}
	return shadow;
}

/* Phong equation */
Vector Ray::phong(Vector p, Vector normal, Vector look, Scene &scene, Vector diffuseMaterial, Vector specularMaterial, float shin){
	
	// Ambient light
	Vector ambient = scene.GetBackground().ambientLight;
	ambient.x = glm::clamp(ambient.x, 0.0f, 1.0f);
	ambient.y = glm::clamp(ambient.y, 0.0f, 1.0f);
	ambient.z = glm::clamp(ambient.z, 0.0f, 1.0f);

	Vector color = ambient * 0.2 * diffuseMaterial;

	// Lights
	for(unsigned int i= 0; i<scene.GetNumLights(); i++){
		
		SceneLight *light = scene.GetLight(i);
		Vector ray = (light->position - p).Normalize();
		
		Ray shadowRay(p, ray, 0);			// Throw ray to the light
		Vector shadow= shadowRay.collisionShadow(scene, collidedObject);

		if (shadow.Magnitude()>0){
			// Attenuation
			float distance = (light->position - p).Magnitude();
			float att = glm::min(1.0f / (light->attenuationConstant + light->attenuationLinear*distance + light->attenuationQuadratic*distance*distance), 1.0f);

			// Specular
			Vector reflectRay = normal*ray.Dot(normal) * 2 - ray;
			Vector specular = specularMaterial * pow(reflectRay.Dot(look), shin);
			specular.x = glm::clamp(specular.x, 0.0f, 1.0f);
			specular.y = glm::clamp(specular.y, 0.0f, 1.0f);
			specular.z = glm::clamp(specular.z, 0.0f, 1.0f);

			// Diffuse
			Vector diffuse = diffuseMaterial * ray.Dot(normal);
			diffuse.x = glm::clamp(diffuse.x, 0.0f, 1.0f);
			diffuse.y = glm::clamp(diffuse.y, 0.0f, 1.0f);
			diffuse.z = glm::clamp(diffuse.z, 0.0f, 1.0f);

			color = color + light->color*(diffuse + specular)*att;

			//Shadow
			color = color * shadow;
		}
	}
	return color;
}

/* Principal method */
Vector Ray::collisions(Scene &scene, int ignoredObject){

	int object = -1;
	int modelTriangle = -1;

	Vector barycCoordAux(0, 0, 0);
	minDistance = INFINITY;

	for(unsigned int i = 0; i<scene.GetNumObjects(); ++i){
		if (i != ignoredObject){
			float distance = -1;
			int modObject = -1;
			SceneObject* obj = scene.GetObject(i);

			// Object Type
			if (obj->IsSphere())	{
				distance = collisionSphere((SceneSphere*)obj);
			}else if (obj->IsTriangle()){
				distance = collisionTriangle((SceneTriangle*)obj, &barycCoordAux);
			}else if(obj->IsModel()){
				//distance = colExtModel((SceneModel*)obj, object, &barycCoord);
			}

			if ((distance>0) && (distance<minDistance)){	// If is min distance
				minDistance = distance;
				object = i;
				modelTriangle = modObject;
				barycCoord = barycCoordAux;
			}
		}
	} 

	collidedObject = object;
	if(minDistance<INFINITY){	// If there's a collision

		SceneObject *intersectObject = scene.GetObject(object);

		// Phong
		Vector color, reflective, refraction, transparency, normal;
		Vector look = (scene.GetCamera().GetPosition() - getIntersectionPoint()).Normalize();

		if(intersectObject->IsSphere()){

			SceneSphere *sphere = (SceneSphere *)intersectObject;
			normal = (getIntersectionPoint() - sphere->getGlobalCenter()).Normalize();

			// Material
			SceneMaterial *mat = scene.GetMaterial(sphere->material);
			reflective = mat->reflective;
			refraction = mat->refraction_index;
			transparency = mat->transparent;

			color = phong(getIntersectionPoint(), normal, look, scene, mat->diffuse, mat->specular, mat->shininess);
		
		}else if(intersectObject->IsTriangle() || intersectObject->IsModel()){	
			SceneTriangle *tri;
			if (intersectObject->IsTriangle()){
				tri = (SceneTriangle*)intersectObject;
			}else{
				tri = ((SceneModel*)intersectObject)->GetTriangle(modelTriangle);
			}

			// Triangle normal interpolated normal from vertex
			normal = tri->normal[0] * barycCoord.z + tri->normal[1] * barycCoord.x + tri->normal[2] * barycCoord.y;

			// Material
			SceneMaterial *mat0 = scene.GetMaterial(tri->material[0]);
			SceneMaterial *mat1 = scene.GetMaterial(tri->material[1]);
			SceneMaterial *mat2 = scene.GetMaterial(tri->material[2]);

			// Texture coordinates
			float U = tri->u[0] * barycCoord.z + tri->u[1] * barycCoord.x + tri->u[2] * barycCoord.y;  if(U<0) U= 0;
			float V = tri->v[0] * barycCoord.z + tri->v[1] * barycCoord.x + tri->v[2] * barycCoord.y;  if(V<0) V= 0;

			// Texture or diffuse
			Vector diffuse0, diffuse1, diffuse2; 

			if(mat0->texture != "") diffuse0= mat0->GetTextureColor(U,V)/255.0f; else diffuse0= mat0->diffuse;
			if(mat1->texture != "") diffuse1= mat1->GetTextureColor(U,V)/255.0f; else diffuse1= mat1->diffuse;
			if(mat2->texture != "") diffuse2= mat2->GetTextureColor(U,V)/255.0f; else diffuse2= mat2->diffuse;

			Vector diffuse = diffuse0              * barycCoord.z + diffuse1              * barycCoord.x + diffuse2              * barycCoord.y;
			Vector specular= mat0->specular        * barycCoord.z + mat1->specular        * barycCoord.x + mat2->specular        * barycCoord.y;
			float shininess= mat0->shininess       * barycCoord.z + mat1->shininess       * barycCoord.x + mat2->shininess       * barycCoord.y;
			     reflective= mat0->reflective      * barycCoord.z + mat1->reflective      * barycCoord.x + mat2->reflective      * barycCoord.y;
			     refraction= mat0->refraction_index* barycCoord.z + mat1->refraction_index* barycCoord.x + mat2->refraction_index* barycCoord.y;
			   transparency= mat0->transparent     * barycCoord.z + mat1->transparent     * barycCoord.x + mat2->transparent     * barycCoord.y;

			   color = phong(getIntersectionPoint(), normal, look, scene, diffuse, specular, shininess);
		}

		/* Reflective */
		if (restRebounds == 0) return color;		// If there´s not rebounds

		if(reflective.Magnitude()>0){				// Throw another ray
			Vector reflected= direction - normal * direction.Dot(normal) * 2;
			Ray newRay(getIntersectionPoint(), reflected, restRebounds-1);
			Vector refColor = newRay.collisions(scene, collidedObject);

			return refColor*reflective + color * Vector(1-reflective.x, 1-reflective.y, 1-reflective.z);
		}

		/* Refraction */
		if(transparency.Magnitude()>0){
			glm::vec3 dir(direction.x, direction.y, direction.z);
			glm::vec3 norm(normal.x, normal.y, normal.z);

			// Red refraction
			glm::vec3 r= glm::refract(dir, norm, refraction.x); Vector red= Vector(r.x,r.y,r.z,0);	
			Ray rRay(getIntersectionPoint(), red, restRebounds-1);
			Vector rCol= rRay.collisions(scene, collidedObject) * Vector(1, 0, 0);

			// Green refraction
			glm::vec3 g= glm::refract(dir, norm, refraction.y); Vector green= Vector(g.x,g.y,g.z,0);
			Ray gRay(getIntersectionPoint(), green, restRebounds-1);
			Vector gCol= gRay.collisions(scene, collidedObject) * Vector(0, 1, 0);

			// Blue refraction
			glm::vec3 b= glm::refract(dir, norm, refraction.y); Vector blue= Vector(b.x,b.y,b.z,0);
			Ray bRay(getIntersectionPoint(), blue, restRebounds-1);
			Vector bCol= bRay.collisions(scene, collidedObject) * Vector(0, 0, 1);

			return (rCol+gCol+bCol) * transparency + color * Vector(1-transparency.x, 1-transparency.y, 1-transparency.z);
		}
		return color;	                // No reflective no refractive
	}
	return scene.GetBackground().color;	// Return background
}