#pragma once

#include "Mesh.h"
#include "Poligon.h"
#include "BezierCurve.h"

class Vein : public Mesh{
	private:
		int NP;
		GLfloat radius;
		BezierCurve *curve;
	public:
		Vein(int NP, GLfloat radius, BezierCurve *curve);
		~Vein();

		void build();

		int nextVertex(int val);
		void draw(bool relleno);
};

