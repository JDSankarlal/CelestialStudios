#include "Animation.h"
#include <filesystem>
namespace fs = std::experimental::filesystem;


Animation::Animation()
{}


Animation::~Animation()
{}

void Animation::addFrame(Mesh * frame, float speed)
{

}

void Animation::addDir(const char * dir, const char * fileName)
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

void Animation::setSpeed(float speed)
{
	m_speed = speed;
}

void Animation::update(Shader* shader, Mesh* mesh)
{
	float time = (float)clock() / CLOCKS_PER_SEC;

	if(!init)
	{
		m_lastTime = time;
		init = true;
	}

	if(mesh)
		if((time =(time - m_lastTime)) >= m_speed)
		{
			//m_lastTime = (float)clock() / CLOCKS_PER_SEC;
			//init = false;
			m_frame = int(time / m_speed) % m_unpackedData.size();


			mesh->editVerts(m_unpackedData[m_frame], m_unpackedData[(m_frame + 1) % m_unpackedData.size()]);

		}
	//printf("%f\n\n", fmodf(time / m_speed,1));
	shader->enable();
	glUniform1f(shader->getUniformLocation("uTime"), fmodf(time / m_speed, 1));
	shader->disable();
}

std::vector<Mesh*> Animation::getFrames()
{
	return std::vector<Mesh*>();
}
