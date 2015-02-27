#pragma once

class VertexNormal{
	private:
		int indexV;
		int indexN;
	public:
		VertexNormal(int v, int n);
		int getNormal();
		int getVertex();
		~VertexNormal(void);
};

