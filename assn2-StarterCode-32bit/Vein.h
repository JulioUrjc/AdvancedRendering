#pragma once

#include "Mesh.h"
#include "Poligon.h"
#include "BezierCurve.h"

class Vein : public Mesh{
	private:
		int NP;
		GLfloat radius;
<<<<<<< HEAD
		BezierCurve *curve;
	public:
		Vein(int NP, GLfloat radius, BezierCurve *curve);
=======
		BezierCurve* curve;
	public:
		Vein(int NP, GLfloat tam, BezierCurve* curve);
>>>>>>> 17b82cdd6993c37c8ef6e2dbe064e732ce0ae344
		~Vein();

		void build();

		int nextVertex(int val);
		void draw(bool relleno);
};

