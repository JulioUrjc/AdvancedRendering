#pragma once

#include "VertexNormal.h"
#include <vector>
using namespace std;

class Face{
	private:
		int numV; // Numero de Vertices de la cara
		std::vector<VertexNormal*> *indicesVN;
	public:
		Face(int nV);
		int getNumV();
		int getNormalIndex(int i);
		int getVertexIndex(int i);
		void setIndicesVN(vector<VertexNormal*> *array);
		~Face(void);
};

