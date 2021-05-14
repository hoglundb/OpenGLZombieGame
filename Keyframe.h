#pragma once
#include<vector>

struct Vector {
	float xPos, yPos, zPos, xNorm, yNorm, zNorm;
};

class Keyframe {
public:
	Keyframe() {
	   this->vertices = new std::vector<Vector>;
	}

	void AddVertex(float x, float y, float z, float nx, float ny, float nz) {
		Vector v;
		v.xPos = x;
		v.yPos = y;
		v.zPos = z;
		v.xNorm = nx;
		v.yNorm = ny;
		v.zNorm = nz;
		this->vertices->push_back(v);
	}
	float keytime;
	std::vector<Vector> *vertices;
};