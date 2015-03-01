#include <cstdlib>
#include <iostream>
#include <time.h>

#include "PerlinGenerator.h"
#include "BitmapSaver.h"

//Constructor. Set the number of levels
PerlinGenerator::PerlinGenerator(int _levels, int _size)
{
	levels = _levels;
	size = _size;

	//Initialize image
	perlinNoise = new float*[size];
	for(int i=0;i<size;i++)
	{
		perlinNoise[i]=new float[size];
		for(int j=0;j<size;j++)
			perlinNoise[i][j]=0;
	}
}

//Destructor
PerlinGenerator::~PerlinGenerator()
{
	BitmapSaver saver(size,size);
	saver.saveBitmap(perlinNoise,L"C:/Users/Raquel/Documents/master/RA/AdvancedRendering/assn2-StarterCode-32bit/Output/perlin.bmp",levels);

	//Delete image
	for(int i=0;i<size;i++)
		delete perlinNoise[i];
	delete perlinNoise;
}

int PerlinGenerator::getLevels()
{
	return levels;
}

//Generate complete perlin map
void PerlinGenerator::generate()
{
	int i,j;

	BitmapSaver saver(size,size);

	//Initialize images
	float ***image = new float**[levels];
	for(i=0;i<levels;i++)
	{
		image[i]=new float*[size];
		for(j=0;j<size;j++)
			image[i][j]=new float[size];
	}

	//Seed
	//srand(4);
	srand(time(NULL));

	LPCWSTR imageNames[6] = {L"C:/Users/Raquel/Documents/master/RA/AdvancedRendering/assn2-StarterCode-32bit/Output/level0.bmp",L"C:/Users/Raquel/Documents/master/RA/AdvancedRendering/assn2-StarterCode-32bit/Output/level1.bmp",L"C:/Users/Raquel/Documents/master/RA/AdvancedRendering/assn2-StarterCode-32bit/Output/level2.bmp",
							 L"C:/Users/Raquel/Documents/master/RA/AdvancedRendering/assn2-StarterCode-32bit/Output/level3.bmp",L"C:/Users/Raquel/Documents/master/RA/AdvancedRendering/assn2-StarterCode-32bit/Output/level4.bmp",L"C:/Users/Raquel/Documents/master/RA/AdvancedRendering/assn2-StarterCode-32bit/Output/level5.bmp"};

	int numSquares = 2;
	for(int level = 0; level<levels;level++)
	{
		float **levelImage = image[level];

		generateNextLevel(numSquares,levelImage);

		saver.saveBitmap(levelImage,imageNames[level],0);

		numSquares*=2;
	}

	//Add all levels
	int k;
	for(i=0;i<size;i++)
		for(j=0;j<size;j++)
			for(k=0;k<levels;k++)
				perlinNoise[i][j] += image[k][i][j];


	//Delete temporal levels
	for(i=0;i<levels;i++)
	{
		for(j=0;j<size;j++)
			delete image[i][j];

		delete image[i];
	}
	delete image;
}

//Generate a level, by indicating the number of levels
void PerlinGenerator::generateNextLevel(int numSquares,float **image)
{
	int i,j;

	int firstPositionI=0;
	int step = size/numSquares;
	int lastPositionI=step;

	float randValue;

	//Set random points
	//First edges
	for(i=0;i<size;i+=step)
	{
		randValue = getRandom();
		image[i][0]=randValue;

		//randValue = getRandom();
		image[i][size-1]=randValue;
	}
	randValue = getRandom();
	image[size-1][0]=randValue;
	//randValue = getRandom();
	image[size-1][size-1]=randValue;
	for(j=step;j<size;j+=step)
	{
		randValue = getRandom();
		image[0][j]=randValue;
		image[size-1][j]=randValue;

		randValue = getRandom();
	}

	//Mid points
	i=step;
	for(int squareI=0;squareI<(numSquares-1);squareI++)
	{
		j=step;
		for(int squareJ=0;squareJ<(numSquares-1);squareJ++)
		{
			//Just initialize right lower vertex, if not in edge
			randValue = getRandom();
			image[i][j]=randValue;

			j+=step;
		}
		i+=step;
	}

	//Interpolate
	i=0;
	int finalI;
	int finalJ;
	for(int squareI=0;squareI<numSquares;squareI++)
	{
		j=0;
		for(int squareJ=0;squareJ<numSquares;squareJ++)
		{
			finalI = i+step;
			if(finalI >= size)
				finalI = size-1;
			finalJ = j+step;
			if(finalJ >= size)
				finalJ = size-1;

			interpolate(i,finalI,j,finalJ,image);
			j+=step;
		}

		i+=step;
	}
}

//Interpolate a square
void PerlinGenerator::interpolate(int firstRow, int lastRow, int firstCol, int lastCol, float **image)
{
	float iInterpolate1, iInterpolate2;

	// Vertex values
	// x1-----------x3
	// |-------------|
	// |-------------|
	// |-------------|
	// x2-----------x4
	float x1,x2,x3,x4;
	x1 = image[firstRow][firstCol];
	x2 = image[lastRow][firstCol];
	x3 = image[firstRow][lastCol];
	x4 = image[lastRow][lastCol];

	//Float variables for not using 1000000000 casts
	float firstRowF = firstRow;
	float lastRowF = lastRow;
	float firstColF = firstCol;
	float lastColF = lastCol;

	//Interpolate
	for(int i=firstRow;i<=lastRow;i++)
	{
		//Float variables for not using 1000000000 casts
		float iF=i;

		//Interpolate in rows
		iInterpolate1 =  ((lastRowF-i)/(lastRowF-firstRowF))*x1 + ((i-firstRowF)/(lastRowF-firstRowF))*x2;
		iInterpolate2 = ((lastRowF-i)/(lastRowF-firstRowF))*x3 + ((i-firstRowF)/(lastRowF-firstRowF))*x4;

		for(int j=firstCol;j<=lastCol;j++)
		{
			if(!(i==firstRow && (j==firstCol || j==lastCol)) && !(i==lastRow && (j==firstCol || j==lastCol)))
			{
				float jF=j;

				//std::cout<<"("<<i<<","<<j<<")"<<std::endl;

				//Interpolate in columns
				float interpolation = ((lastColF-j)/(lastColF-firstColF))*iInterpolate1 + 
										((j-firstColF)/(lastColF-firstColF))*iInterpolate2;

				image[i][j] = interpolation;
			}
		}
	}
}

//Random number between -1 and 1
float PerlinGenerator::getRandom()
{
	return (((float)rand()) / RAND_MAX)*2 -1;
}

//Get noise image pointer
float **PerlinGenerator::getNoiseImage()
{
	return perlinNoise;
}