#pragma once

#include "BezierCurve.h"
#include <glm\glm.hpp>
using namespace glm;

//Const camera values
const float MAX_X_ROTATION = 90.0f;
const float MIN_X_ROTATION = -90.0f;

const float MAX_ZOOM = 2.0f;
const float MIN_ZOOM = 0.3f;

class Camera{
	private:
		//Perspective Definition
		float fov;
		float aspectRatio;
		float nearPlane;
		float farPlane;

		//Look At Definition
		vec3 position;
		vec3 look;
		vec3 up;

		//View and projection matrix
		mat4 viewMatrix;
		mat4 projectionMatrix;

		//Orientation
		vec3 orientation;
		vec3 inertia;

		//Vein track
		BezierCurve curve;
		int currentPoint;

		//Zoom
		float zoom;

	public:
		//Constructor with all parameters
		Camera(float fov, float aspectRatio, float nearPlane, float farPlane, vec3 position, vec3 look, vec3 up);

		//Set perspective parameters
		void setPerspective(float _fov, float _aspectRatio, float _nearPlane, float _farPlane);
		//Set aspect ratio
		void setAspectRatio(float _aspectRatio);
		//Set look at parameters
		void setLookAt(vec3 _position, vec3 _look, vec3 _up);

		//Set camera position
		void setPosition(vec3 _position);
		//Get camera position
		vec3 getPosition();
		//Get camera orientation
		vec3 getOrientation();
		//Get camera look
		vec3 getLook();
		//Get camera Up
		vec3 getUp();
		//Set look vector
		void setLookVector(vec3 _look);
		//Set up vector
		void setUpVector(vec3 _up);

		//Move camera. Adds a vector to the position
		void moveCamera(vec3 move);

		//Rotate camera
		void rotate(vec3 rotation);

		//Set camera inertia rotation
		void setInertiaRotation(vec3 _inertia);

		//Update camera in First Person mode
		void updateFP();

		//Update camera in First Person mode without inertia
		void updateFromRollerCoaster();

		//Set curve of roller coaster
		void setVeinCurve(BezierCurve c);

		//Matrix getters
		mat4 getViewMatrix();
		mat4 getProjectionMatrix();
		mat4 getModelView(mat4& modelMatrix);
		//mat4 getModelViewProjection(mat4 modelMatrix);

		//Increase zoom
		void increaseZoom(float step);
		void decreaseZoom(float step);
};

