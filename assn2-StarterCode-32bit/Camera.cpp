#include "Camera.h"

#include <glm\gtc\matrix_transform.hpp>

//Constructor with all parameters
Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane, vec3 position, vec3 look, vec3 up) :
fov(fov), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane), position(position), look(look), up(up)
{
	//Create both
	viewMatrix = lookAt(position, look, up);
	projectionMatrix = perspective(fov, aspectRatio, nearPlane, farPlane);

	orientation = vec3(0.0f, 0.0f, 0.0f);
	inertia = vec3(0.0f, 0.0f, 0.0f);

	zoom = 1.0f;
	currentPoint = 0;
}
//Set perspective parameters
void Camera::setPerspective(float _fov, float _aspectRatio, float _nearPlane, float _farPlane)
{
	fov = _fov;
	aspectRatio = _aspectRatio;
	nearPlane = _nearPlane;
	farPlane = _farPlane;

	projectionMatrix = perspective(fov, aspectRatio, nearPlane, farPlane);
}

void Camera::setAspectRatio(float _aspectRatio)
{
	aspectRatio = _aspectRatio;

	projectionMatrix = perspective(fov, aspectRatio, nearPlane, farPlane);
}

//Set look at parameters
void Camera::setLookAt(vec3 _position, vec3 _look, vec3 _up)
{
	position = _position;
	look = _look;
	up = _up;

	viewMatrix = lookAt(position, look, up);
}

//Set camera position
void Camera::setPosition(vec3 _position)
{
	position = _position;
	viewMatrix = lookAt(position, look, up);
}

//Set look vector
void Camera::setLookVector(vec3 _look)
{
	look = _look;
	viewMatrix = lookAt(position, look, up);
}

//Set up vector
void Camera::setUpVector(vec3 _up)
{
	up = _up;
	viewMatrix = lookAt(position, look, up);
}

//Move camera. Adds a vector to the position
void Camera::moveCamera(vec3 move)
{
	look += move;
	position += move;
}

//Rotate camera
void Camera::rotate(vec3 rotation)
{
	orientation += rotation;
}

//Set camera inertia rotation
void Camera::setInertiaRotation(vec3 _inertia)
{
	inertia = _inertia;
}

//Update camera in First Person mode
void Camera::updateFP()
{
	//ProjectionMatrix
	projectionMatrix = perspective(fov*zoom, aspectRatio, nearPlane, farPlane);

	//Update orientation
	//Clamp orientationX [MIN_X_ROTATION..MAX_X_ROTATION]
	float newOrientationX = orientation[0] + inertia[1];
	if (newOrientationX > MAX_X_ROTATION)
		orientation[0] = MAX_X_ROTATION;
	else if (newOrientationX < MIN_X_ROTATION)
		orientation[0] = MIN_X_ROTATION;
	else
		orientation[0] = newOrientationX;

	orientation[1] += inertia[0];
	//Clamp orientationY [-360..360]
	orientation[1] = fmod(orientation[1], 360);

	//ViewMatrix
	viewMatrix = glm::rotate(glm::mat4(), orientation[0], glm::vec3(1.0, .0, .0));
	viewMatrix = glm::rotate(viewMatrix, orientation[1], glm::vec3(0.0, 1.0, .0));
	viewMatrix = glm::rotate(viewMatrix, orientation[2], glm::vec3(0.0, 0.0, 1.0));
	viewMatrix = glm::translate(viewMatrix, -position);
}

//Update camera when is attached to the rollercoaster
void Camera::updateFromRollerCoaster()
{
	//ProjectionMatrix
	projectionMatrix = perspective(fov*zoom, aspectRatio, nearPlane, farPlane);


	/*if (currentPoint >= rollerCoaster.nPoints())
		currentPoint = 0;*/

	//position = rollerCoaster.getPointList()[currentPoint] + (rollerCoaster.getBinormalList()[currentPoint]);

	//ViewMatrix
	//viewMatrix = glm::lookAt(position, position + rollerCoaster.getTangentList()[currentPoint], rollerCoaster.getBinormalList()[currentPoint]);

	currentPoint++;
}


//Getters
mat4 Camera::getViewMatrix()
{
	return viewMatrix;
}
mat4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}
/*
mat4 Camera::getModelView(mat4 modelMatrix)
{
	return viewMatrix*modelMatrix;
}
mat4 Camera::getModelViewProjection(mat4 modelMatrix)
{
	return projectionMatrix*viewMatrix*modelMatrix;
}
*/

//Get position 
vec3 Camera::getPosition()
{
	return position;
}

//Get Orientation X
vec3 Camera::getOrientation()
{
	return orientation;
}

void Camera::increaseZoom(float step)
{
	zoom = ((zoom + step)>MAX_ZOOM) ? MAX_ZOOM : (zoom + step);
}

void Camera::decreaseZoom(float step)
{
	zoom = ((zoom - step)<MIN_ZOOM) ? MIN_ZOOM : (zoom - step);
}

void Camera::setRollerCoasterCurve(BezierCurve c)
{
	rollerCoaster = c;
	currentPoint = 0;
}


