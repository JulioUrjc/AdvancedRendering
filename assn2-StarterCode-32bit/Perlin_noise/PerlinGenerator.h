#pragma once

#ifndef ITDL_PERLINGENERATOR
#define ITDL_PERLINGENERATOR

class PerlinGenerator
{
public:
	//Constructor. Set the number of levels
	PerlinGenerator(int levels,int size);

	//Destructor
	~PerlinGenerator();

	int getLevels();

	//Generate perlin noise
	void generate();
	//Generate perlin noise level
	void generateNextLevel(int numSquares,float **image);
	//Interpolate a square
	void interpolate(int firstRow, int lastRow, int firstCol, int lastCol, float **image);
	//Get a random number between -1 and 1
	float getRandom();

private:
	int levels;
	int size;

	float **perlinNoise;
	float ***image;

};

#endif
