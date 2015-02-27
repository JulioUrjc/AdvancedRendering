#pragma once

#include "Mesh.h"
#include "Poligon.h"

class Vein : public Mesh{
	private:
		int NP;
		int NQ;
		GLfloat radius;
	public:
		Vein(int NP, int NQ, GLfloat tam);
		~Vein();

		void build();

		GLfloat functionX(GLfloat val);
		GLfloat functionY(GLfloat val);
		GLfloat functionZ(GLfloat val);

		GLfloat fDerivateX(GLfloat val);
		GLfloat fDerivateY(GLfloat val);
		GLfloat fDerivateZ(GLfloat val);

		GLfloat sDerivateX(GLfloat val);
		GLfloat sDerivateY(GLfloat val);
		GLfloat sDerivateZ(GLfloat val);

		Point3D* function(GLfloat val);

		Point3D* fDerivate(GLfloat val);
		Point3D* sDerivate(GLfloat val);

		int nextVertex(int val);
		void draw(bool relleno);
		void carStep(GLfloat step);
};

