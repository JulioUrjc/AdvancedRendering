#include "Blood.h"


Blood::Blood(int numRed, int numWhite, BezierCurve *c)
{
	numRedCorpuscles = numRed;
	numWhiteCorpuscles = numWhite;
	numObj = numRed + numWhite;
	curve = c;
}


Blood::~Blood()
{
}

float Blood::numRandom(){
	return ((float)rand() / (RAND_MAX + 1)) * 10;
}
