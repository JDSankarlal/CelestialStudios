#pragma once

#include <vector>
#include "PointHandle.h"

//#include "Math.h"

enum InterpolationMode
{
	LINEAR = 0,
	CATMULL,
	BEZIER
};

class Path
{
public:
	InterpolationMode interpolationMode = CATMULL;
	std::vector<PointHandle> pointHandles;
	std::vector<float> distances;
	int p0 = 0;		
	int p1 = 1;
	int p2 = 2;
	int p3 = 3;
	float time = 0;		// interpolation parameter (Should only be 0 to 1)
	float speed = 1.0f; // The speed the path updates at (Higher is faster)
	float tension = 0.5f;
	glm::vec3 position;

	bool connectedEndpoints = false;

	void computeDistances();
	void computeIndex();
	void update(float &deltaTime);

	glm::vec3 interpolate(float t);
	
	glm::vec3 doLerp(float t);
	glm::vec3 doCatmull(float t);
	glm::vec3 doBezier(float t);
};