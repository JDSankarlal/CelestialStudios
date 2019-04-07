#include "Animation.h"
#include <filesystem>
namespace fs = std::experimental::filesystem;


Animation::Animation()
{}

Animation::~Animation()
{}

void Animation::addFrame(Mesh* frame, float speed)
{
	frame, speed;
}

void Animation::setAnimationSpeed(float speed)
{
	m_speed = speed;
}

void Animation::addDir(const char * dir)
{
	std::string path(dir);
	//path += fileName;
	auto filePathData = fs::directory_iterator(path);

	m_unpackedData.clear();
	for(auto&a : filePathData)
	{
		std::wstring tmpPath = a.path();
		int check = (int)tmpPath.find(L".obj");
		if(check < 0)continue;

		Mesh tmp;
		std::string str;
		for(auto&b : tmpPath)
			str += (char)b;

		m_unpackedData.push_back(tmp.loadAni(str));
	}
}

void Animation::update(Shader* shader, Mesh* mesh)
{
	float time;
	
	time = (float)clock() / CLOCKS_PER_SEC;

	if(!init)
	{
		m_lastTime = time;
		init = true;
	}

	if(!m_pause && !m_stop)
	{
		if(mesh)
			if((time = (time - m_lastTime)) >= m_speed)
			{
				if(m_repeat)
				{
					m_frame = int(time / m_speed) % m_unpackedData.size();
					mesh->editVerts(m_unpackedData[m_frame], m_unpackedData[m_frameNext = (m_frame + 1) % m_unpackedData.size()]);
				} else
				{
					m_frame = int(time / m_speed);
					m_frame = m_frame >= m_unpackedData.size() - 1 ? unsigned((m_unpackedData.size() - 2) % m_unpackedData.size() ): m_frame;

					if(m_frame < m_unpackedData.size() - 2)
						mesh->editVerts(m_unpackedData[m_frame], m_unpackedData[m_frameNext = (m_frame + 1) % m_unpackedData.size()]);
					else
						mesh->editVerts(m_unpackedData[m_frame], m_unpackedData[m_frameNext = m_frame]);
				}

			}
	} else
	{
		if(mesh)
			if((time = (time - m_lastTime)) >= m_speed)
			{
				if(m_pause)
				{
					mesh->editVerts(m_unpackedData[m_frame], m_unpackedData[m_frame]);
					m_lastTime = time;
				}
				else
					if(m_stop)
				{
					mesh->editVerts(m_unpackedData[0], m_unpackedData[0]);
					m_lastTime = time;
				}
			}
	}

	//printf("%f\n\n", fmodf(time / m_speed, 1));
	shader->enable();
	glUniform1f(shader->getUniformLocation("uTime"), fmodf(time / m_speed, 1));
	shader->disable();
}

int Animation::getFrameNumber()
{
	return m_frame;
}

bool Animation::hasEnded()
{
	return m_frame == m_frames.size()&& !m_repeat;
}

bool Animation::checkPlay()
{
	return !m_pause;
}

void Animation::stop()
{
	m_frame = 0;
	m_stop = true;
}

void Animation::play()
{
	m_pause = m_stop = false;
}

void Animation::pause()
{
	m_pause = true;
}

void Animation::repeat(bool repeat)
{
	m_repeat = repeat;
}
