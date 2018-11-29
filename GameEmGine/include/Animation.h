#pragma once
#include <vector>
#include "Mesh.h"

class Animation
{
public:
	Animation();
	~Animation();

	void addFrame(Mesh* frame,float speed);
	void addDir(const char* dir);

	std::vector<Mesh*> getFrames();

private:

	std::vector<Mesh*> m_frames;
};

