#include "Blood.h"
#include<iostream>

Blood::Blood(int numRed, int numWhite, BezierCurve *c, float rad){
	numRedCorpuscles = numRed;
	numWhiteCorpuscles = numWhite;
	numObj = numRed + numWhite;
	curve = c;
	radius = rad;

	generateRedCorpuscles();
	generateWhiteCorpuscles();
}


Blood::~Blood(){}

void Blood::generateRedCorpuscles(){
	for (int i = 0; i < numRedCorpuscles; i++){
		int aux = intRandom(curve->nPoints());
		PV3D auxP = *curve->getPointList().at(aux);
		//At the point that we are (position x, y z) we add a random number at x, y, z to create our corpuscle.
		//We apply the random in the range of the diameter of the vein, and later we substract the radius, so we can have negative and positive numbers
		auxP = *auxP.addition(new PV3D(radius - numRandom(radius * 2), radius - numRandom(radius * 2), radius - numRandom(radius * 2)));
		//We rotate the position  a random in the tree axis, between 2 PI
		//PV3D auxR = PV3D(numRandom(M_PI * 2), numRandom(M_PI * 2), numRandom(M_PI * 2));
		//PV3D auxR = PV3D(intRandom(270), intRandom(270), intRandom(270));
		PV3D auxR = PV3D(0, M_PI / 2, 0);
		//auxP.toString();
		BloodElement redCorpuscle = BloodElement(RED, auxP, auxR);
		bloodObj.push_back(redCorpuscle);
	}
}

void Blood::generateWhiteCorpuscles(){
	for (int i = 0; i < numWhiteCorpuscles; i++){
		int aux = intRandom(curve->nPoints());
		PV3D auxP = *curve->getPointList().at(aux);
		//At the point that we are (position x, y z) we add a random number at x, y, z to create our corpuscle.
		//We apply the random in the range of the diameter of the vein, and later we substract the radius, so we can have negative and positive numbers
		auxP = *auxP.addition(new PV3D(radius - numRandom(radius * 2), radius - numRandom(radius * 2), radius - numRandom(radius * 2)));
		//We rotate the position  a random in the tree axis, between 2 PI
		//Rotation vector is: x = rotation at x-axis; y = rotation at y-axis; z=rotation at z-axis;
		//PV3D auxR = PV3D(numRandom(M_PI * 2), numRandom(M_PI * 2), numRandom(M_PI * 2));
		//PV3D auxR = PV3D(intRandom(270), intRandom(270), intRandom(270));
		PV3D auxR = PV3D(0, M_PI / 2, 0);
		//auxP.toString();
		BloodElement whiteCorpuscle1 = BloodElement(WHITE,auxP,auxR);
		auxR = PV3D();
		BloodElement whiteCorpuscle2 = BloodElement(WHITE, auxP, auxR);
		//bloodObj.push_back(whiteCorpuscle1);
		bloodObj.push_back(whiteCorpuscle2);
	}
}

void Blood::draw(int modo){
	for (int i = 0; i < bloodObj.size(); i++){
		bloodObj.at(i).draw(modo);
	}
}

void Blood::draw(Camara* camara, int modo){
	for (int i = 0; i < bloodObj.size(); i++){
		bloodObj.at(i).draw(camara, modo);
	}
}

//Generating random float from a range=highest-lowest
float Blood::numRandom(float range){
	return range*((float)rand()/RAND_MAX+1.0f);
}

int Blood::intRandom(int max){
	return((int)rand() % (max + 1));
}
