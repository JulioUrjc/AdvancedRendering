#pragma once

#include "Mesh.h"
#include "Poligon.h"
#include "BezierCurve.h"
#include "Camara.h"
#include <vector>
#include <glm\gtx\transform.hpp>


#include <windows.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"

class Vein : public Mesh{
	private:
		int NP;
		GLfloat radius;
		BezierCurve* curve;
	public:
		Vein(int NP, GLfloat tam, BezierCurve* curve);
		~Vein();

		void build();

		int nextVertex(int val);

		void draw(int modo);

		void draw(bool relleno);

		void addPerlinNoise(float** perlinNoise);

};

