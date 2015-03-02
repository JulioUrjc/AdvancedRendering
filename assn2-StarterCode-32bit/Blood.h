#pragma once

#include <vector>

#include "BezierCurve.h"
#include "BloodElement.h"

class Blood
{

private:
	
	int numRedCorpuscles;
	int numWhiteCorpuscles;
	int numObj;

	std::vector<BloodElement> bloodObj;

	BezierCurve* curve;

public:

	Blood(int numRed, int numWhite, BezierCurve *c);
	~Blood();

	void generateRedCorpuscles();
	void generateWhiteCorpuscles();

	void draw(int modo);

	float numRandom();
	int intRandom(int max);
};

