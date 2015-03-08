#pragma once

#include <vector>
#include "BloodElement.h"

class Blood{
	private:
	
		int numRedCorpuscles;
		int numWhiteCorpuscles;
		int numObj;

		std::vector<BloodElement*> bloodObj;
		
		BezierCurve* curve;
		float radius;
		int showTexture;

	public:

		Blood(int numRed, int numWhite, BezierCurve *c, float rad);
		~Blood();

		void generateRedCorpuscles();
		void generateWhiteCorpuscles();

		void draw(int modo);
		void draw(Camara* camara, int modo, bool mutation);
		float numRandom(float range);
		int intRandom(int max);
		void setShowTexture(int showText);
		void freeMemory();
};

