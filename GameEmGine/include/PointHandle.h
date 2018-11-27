#pragma once

#include <GLM/glm.hpp>
#include <string>
#include <vector>

class PointHandle
{
public:
	// Point size is in pixels
	PointHandle(float _pointSize, glm::vec3 _position, std::string _label = "");

	// Does a simple radius based intersection test
	bool isInside(glm::vec3 p);

	void draw();

	static void erasePoint(std::vector<PointHandle>& pointVec, int index);

	glm::vec3 position;
	std::string label;
	float pointSize;
};