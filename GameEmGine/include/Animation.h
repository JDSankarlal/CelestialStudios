#pragma once
#include <vector>
#include <string>
#include <ctime>
#include "Mesh.h"
#include "Shader.h"

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

	void update(Shader* shader,Mesh* mesh);
	std::vector<Mesh*> getFrames();

	
	
private:
	std::vector<Mesh*> m_frames;
	std::vector<std::vector< std::pair<std::string, std::vector<Vertex3D>>>> m_unpackedData;
	bool init=false;
	float m_speed,m_lastTime,m_time;
	unsigned m_frame=0;
};

