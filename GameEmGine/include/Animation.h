#pragma once
#include <vector>
#include <string>
#include "Mesh.h"

class Animation
{
public:
	Animation();
	~Animation();

	void setAnimationSpeed();

	void addFrame(Mesh* frame, float speed = 1);
	void addDir(const char * dir, const char * fileName);
	
	//gets a speed in seconds
	void setSpeed(float speed);

	void update();
	std::vector<Mesh*> getFrames();

private:
	std::vector<Mesh*> m_frames;
	std::vector<std::vector< std::pair<std::string, std::vector<Vertex3D>>>> m_unpackedData;
	float m_speed;
};

