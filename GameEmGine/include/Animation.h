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

	
	void addFrame(Mesh* frame, float speed = 1);
	void addDir(const char * dir );
	
	//setts the delay between the animations
	void setAnimationSpeed(float speed);

	void update(Shader* shader,Mesh* mesh);
	
	int getFrameNumber();

	bool hasEnded();

	bool checkPlay();

	void stop();

	void play();

	void pause();

	void repeat(bool repeat);
private:
	bool m_pause=0, m_stop=0,m_repeat=0;

	std::vector<Mesh*> m_frames;
	std::vector<std::vector< std::pair<std::string, std::vector<Vertex3D>>>> m_unpackedData;
	bool init=false;
	float m_speed,m_lastTime,m_time;
	unsigned m_frame = 0, m_frameNext;
};

