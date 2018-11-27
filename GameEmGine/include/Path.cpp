#include "Path.h"
#include "ExtraMath.h"

void Path::computeDistances()
{
	distances.resize(pointHandles.size());
	if (pointHandles.size() > 1)
	{
		for (int i = 0; i < pointHandles.size() - 1; ++i)
		{
			distances[i] = glm::distance(pointHandles[i].position, pointHandles[i + 1].position);
		}
		distances[distances.size()-1] = glm::distance(pointHandles[pointHandles.size()-1].position, pointHandles[0].position);
	}
}

void Path::computeIndex()
{
	int numOfPoints = pointHandles.size();
	int offset = 1-connectedEndpoints;
	switch (interpolationMode)
	{
	case LINEAR:
			if (p0 >= numOfPoints - offset)
			{
				p0 = 0;
				p1 = 1;
			}
			if (p1 >= numOfPoints)
				p1 = 0;
		break;
	case CATMULL:
		if (p0 >= numOfPoints - offset * 3)
			p0 = 0;

		p1 = vectorWrap(p0 + 1, numOfPoints);
		p2 = vectorWrap(p0 + 2, numOfPoints);
		p3 = vectorWrap(p0 + 3, numOfPoints);
		break;
	case BEZIER:
		p0 = 0;
		p1 = 1;
		p2 = 2;
		p3 = 3;
		break;
	}
	
}

void Path::update(float &deltaTime)
{
	if (pointHandles.size() > 0)
	{
		time += deltaTime * speed;

		computeIndex();
		while (time >= 1.0f)
		{
			time -= 1.0f;
			++p0;
			p1 = p0 + 1;
			computeIndex();
		}

		position = interpolate(time);
	}
}

glm::vec3 Path::interpolate(float t)
{
	switch (interpolationMode)
	{
	case LINEAR:
		return doLerp(t);
		break;
	case CATMULL:
		return doCatmull(t);
		break;
	case BEZIER:
		return doBezier(t);
		break;
	}
	return glm::vec3();
}

glm::vec3 Path::doLerp(float t)
{
	if(pointHandles.size() < 2)
		return glm::vec3();

	return lerp(pointHandles[p0].position, pointHandles[p1].position, time);
}

glm::vec3 Path::doCatmull(float t)
{
	if (pointHandles.size() < 4)
		return glm::vec3();
	return catmullUMP(
		pointHandles[p0].position,
		pointHandles[p1].position,
		pointHandles[p2].position,
		pointHandles[p3].position, time);
}

glm::vec3 Path::doBezier(float t)
{
	if (pointHandles.size() < 4)
		return glm::vec3();
	
	return bezier(
		pointHandles[p0].position,
		pointHandles[p1].position,
		pointHandles[p2].position, 
		pointHandles[p3].position, time);
}
