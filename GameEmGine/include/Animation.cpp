#include "Animation.h"
#include <filesystem>
namespace fs = std::experimental::filesystem;


Animation::Animation()
{}

Animation::~Animation()
{}

void Animation::addFrame(Mesh * frame, float speed)
{}

void Animation::setAnimationSpeed(float speed)
{
	m_speed = speed;
}

void Animation::addDir(const char * dir)
{
	std::string path(dir);
	//path += fileName;
	auto filePath = fs::directory_iterator(path);

	m_unpackedData.clear();
	for(auto&a : filePath)
	{
		std::wstring tmpPath = a.path();
		int check = tmpPath.find(L".obj");
		if(check < 0)continue;

		Mesh tmp;
		m_unpackedData.push_back(tmp.loadAni(std::string(tmpPath.begin(), tmpPath.end())));
	}

}

void Animation::update(Shader* shader, Mesh* mesh)
{
	float time = (float)clock() / CLOCKS_PER_SEC;

	if(!init)
	{
		m_lastTime = time;
		init = true;
	}


	if(!m_pause && !m_stop)
		if(mesh)
			if((time = (time - m_lastTime)) >= m_speed)
			{
				if(m_repeat)
					m_frame = int(time / m_speed) % m_unpackedData.size();
				else
				{
					m_frame = int(time / m_speed);
					m_frame = m_frame >= m_unpackedData.size() - 1 ? (m_unpackedData.size() - 2) % m_unpackedData.size() : m_frame;
				}

				if(m_repeat)
					mesh->editVerts(m_unpackedData[m_frame], m_unpackedData[(m_frame + 1) % m_unpackedData.size()]);
				else
				{
					if(m_frame < m_unpackedData.size() - 2)
						mesh->editVerts(m_unpackedData[m_frame], m_unpackedData[(m_frame + 1) % m_unpackedData.size()]);
					else
						mesh->editVerts(m_unpackedData[m_frame], m_unpackedData[m_frame]);
				}
			}
	//printf("%f\n\n", fmodf(time / m_speed, 1));
	shader->enable();
	glUniform1f(shader->getUniformLocation("uTime"), fmodf(time / m_speed, 1));
	shader->disable();
}

void Animation::stop()
{
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
