/*
  15-462 Computer Graphics I
  Assignment 3: Ray Tracer
  C++ RayTracer Class
  Author: rtark
  Oct 2007

  NOTE: This is the file you will need to begin working with
		You will need to implement the RayTrace::CalculatePixel () function

  This file defines the following:
	RayTrace Class
*/
#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "Utils.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Ray.h"

const int REBOUNDS = 4;
/* Clase que define el algoritmo RayTrace Raquel Peces y Julio Martín */
/*
	RayTrace Class - The class containing the function you will need to implement.This is the class with the function you need to implement
*/
class RayTrace{
	public:
		/* - Scene Variable for the Scene Definition - */
		Scene m_Scene;

		// -- Constructors & Destructors --
		RayTrace(void) {}
		~RayTrace (void) {}

		// -- Main Functions --
		// - CalculatePixel - Returns the Computed Pixel for that screen coordinate
	   Vector CalculatePixel (int screenX, int screenY);
	
	private:
		
		float fovX, fovY;
		Vector look, up, normal;
		Vector camPos;

		void initialize(void);
};

