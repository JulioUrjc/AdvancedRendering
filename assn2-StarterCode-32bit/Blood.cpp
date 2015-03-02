#include "Blood.h"


Blood::Blood(int numRed, int numWhite, BezierCurve *c)
{
	numRedCorpuscles = numRed;
	numWhiteCorpuscles = numWhite;
	numObj = numRed + numWhite;
	curve = c;

	generateRedCorpuscles();
	generateWhiteCorpuscles();
}


Blood::~Blood()
{
}

void Blood::generateRedCorpuscles(){
	for (int i = 0; i < numRedCorpuscles; i++){
		int aux = intRandom(curve->nPoints());
		PV3D auxP = *curve->getPointList().at(aux)->addition(numRandom());
		PV3D auxR = PV3D(numRandom()*((aux % 3) == 0), numRandom()*((aux % 3) == 1), numRandom()*((aux % 3) == 2));
		BloodElement redCorpuscle = BloodElement(RED, auxP, auxR);
		bloodObj.push_back(redCorpuscle);
	}
}

void Blood::generateWhiteCorpuscles(){
	for (int i = 0; i < numWhiteCorpuscles; i++){
		int aux = intRandom(curve->nPoints());
		PV3D auxP = *curve->getPointList().at(aux)->addition(numRandom());
		PV3D auxR = PV3D(numRandom()*((aux % 3)==0), numRandom()*((aux % 3)==1), numRandom()*((aux % 3)==2));
		BloodElement whiteCorpuscle1 = BloodElement(WHITE,auxP,auxR);
		auxR = PV3D();
		BloodElement whiteCorpuscle2 = BloodElement(WHITE, auxP, auxR);
		bloodObj.push_back(whiteCorpuscle1);
		bloodObj.push_back(whiteCorpuscle2);
	}
}

void Blood::draw(int modo){
	for (int i = 0; i < bloodObj.size(); i++){
		bloodObj.at(i).draw(modo);
	}
}


float Blood::numRandom(){
	return ((float)rand() / (RAND_MAX + 1));
}

int Blood::intRandom(int max){
	return((int)rand() / (max + 1));
}
