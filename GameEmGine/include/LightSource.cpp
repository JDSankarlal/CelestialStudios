#include "LightSource.h"

#pragma region Static Variables
ColourRGBA LightSource::m_ambient;
std::vector<LightInfo >LightSource::m_lights;

Shader *LightSource::m_shader;
Camera* LightSource::m_cam;
LightInfo LightSource::m_info;
#pragma endregion

void LightSource::initLight(LIGHT_TYPE type, Camera * cam, Shader * shader, LightInfo info, unsigned index)
{
	//char buff[90];
	//
	//sprintf_s(buff, "LightPosition[%d]", index);
	//glUniform4fv(shader->getUniformLocation(buff), 1, &(cam->getObjectMatrix()*glm::vec4(0.0f, 3.0f, 10.0f, 1.0f))[0]);
	//sprintf_s(buff, "LightAmbient[%d]", index);
	//glUniform3f(shader->getUniformLocation(buff), info.ambient[0] / 255.0f, info.ambient[1] / 255.0f, info.ambient[2] / 255.0f);
	//sprintf_s(buff, "LightDiffuse[%d]", index);
	//glUniform3f(shader->getUniformLocation(buff), info.diffuse[0] / 255.0f, info.diffuse[1] / 255.0f, info.diffuse[2] / 255.0f);
	//sprintf_s(buff, "LightSpecular[%d]", index);
	//glUniform3f(shader->getUniformLocation(buff), info.specular[0] / 255.0f, info.specular[1] / 255.0f, info.specular[2] / 255.0f);
	//sprintf_s(buff, "LightDirection[%d]", index);
	//glUniform3fv(shader->getUniformLocation(buff), 1, &info.direction[0]);
	//sprintf_s(buff, "LightSpecularExponent[%d]", index);
	//glUniform1f(shader->getUniformLocation(buff), 50.0f);
	//sprintf_s(buff, "Attenuation_Constant[%d]", index);
	//glUniform1f(shader->getUniformLocation(buff), info.attenuationConst);
	//sprintf_s(buff, "Attenuation_Linear[%d]", index);
	//glUniform1f(shader->getUniformLocation(buff), 0.1f);
	//sprintf_s(buff, "Attenuation_Quadratic[%d]", index);
	//glUniform1f(shader->getUniformLocation(buff), 0.01f);
}

void LightSource::setLightType(LIGHT_TYPE type, unsigned index)
{
	m_lights[index].type = type;
}

void LightSource::setPosition(Coord3D pos, unsigned index)
{
	m_lights[index].transform->setPosition(pos.coordX, pos.coordY, pos.coordZ);
}

void LightSource::setDirection(Coord3D dir, int index)
{
	m_lights[index].direction = -dir;
}

void LightSource::setSceneAmbient(ColourRGBA ambi, unsigned index)
{
	m_ambient = ambi;
}

void LightSource::setDiffuse(ColourRGBA diff, unsigned index)
{
	m_lights[index].diffuse = diff;
}

void LightSource::setSpecular(ColourRGBA spec, unsigned index)
{
	m_lights[index].specular = spec;
}

void LightSource::setSpecularExponent(float specEx, unsigned index)
{
	m_lights[index].specularExponent = specEx;
}

void LightSource::setAttenuationConstant(float attenConst, unsigned index)
{
	m_lights[index].attenuationConst = attenConst;
}

void LightSource::setAttenuationLinear(float attenLinear, unsigned index)
{
	m_lights[index].attenuationLinear = attenLinear;
}

void LightSource::setAttenuationQuadratic(float attenQuad, unsigned index)
{
	m_lights[index].attenuationQuadratic = attenQuad;
}

void LightSource::setParent(Model * parent, unsigned index)
{
	m_lights[index].parent = parent;
}

void LightSource::setCamera(Camera * cam)
{
	m_cam = cam;
}

void LightSource::setShader(Shader * shad)
{
	m_shader = shad;
}

void LightSource::setLightAmount(unsigned size)
{
	m_lights.resize(size);
}


void LightSource::update()
{
	char buff[90];
	m_shader->enable();
	glUniform1i(m_shader->getUniformLocation("LightAmount"), m_lights.size());
	glUniform3f(m_shader->getUniformLocation("LightAmbient"), m_ambient[0] / 255.0f, m_ambient[1] / 255.0f, m_ambient[2] / 255.0f);
	
	for(unsigned a = 0; a < m_lights.size(); a++)
	{
		Coord3D lp = m_lights[a].transform->getPosition();
		glm::vec4 pos(lp.coordX, lp.coordY, lp.coordZ, 1.0f);
			if(m_lights[a].parent)
			{
				Transformer *trans = &m_lights[a].parent->getTransformer();
				glm::vec3 pos2 (trans->getPosition().coordX, trans->getPosition().coordY, trans->getPosition().coordZ);
				glm::mat4 forward = glm::lookAt(pos2, pos2 + glm::vec3(cos(trans->getRotation().coordX), tan(trans->getRotation().coordY), sin(trans->getRotation().coordZ)), 
							{0,1,0});
				pos =  m_lights[a].parent->getTransformer().getTransformation() * m_lights[a].transform->getTransformation()*forward * glm::vec4(pos2,1.f);
			} else
			pos = m_lights[a].transform->getTransformation() * pos;
	
		sprintf_s(buff, "LightType[%d]", a);
		glUniform1i(m_shader->getUniformLocation(buff), (int)m_lights[a].type);
		sprintf_s(buff, "LightPosition[%d]", a);
		glUniform4fv(m_shader->getUniformLocation(buff), 1, &(m_cam->getCameraMatrix()*pos)[0]);	
		sprintf_s(buff, "LightDiffuse[%d]", a);
		glUniform3f(m_shader->getUniformLocation(buff), m_lights[a].diffuse[0] / 255.0f, m_lights[a].diffuse[1] / 255.0f, m_lights[a].diffuse[2] / 255.0f);
		sprintf_s(buff, "LightSpecular[%d]", a);
		glUniform3f(m_shader->getUniformLocation(buff), m_lights[a].specular[0] / 255.0f, m_lights[a].specular[1] / 255.0f, m_lights[a].specular[2] / 255.0f);
		sprintf_s(buff, "LightDirection[%d]", a);
		glUniform3fv(m_shader->getUniformLocation(buff), 1, &m_lights[a].direction[0]);
		sprintf_s(buff, "LightSpecularExponent[%d]", a);
		glUniform1f(m_shader->getUniformLocation(buff), 50.0f);
		sprintf_s(buff, "Attenuation_Constant[%d]", a);
		glUniform1f(m_shader->getUniformLocation(buff), m_lights[a].attenuationConst);
		sprintf_s(buff, "Attenuation_Linear[%d]", a);
		glUniform1f(m_shader->getUniformLocation(buff), m_lights[a].attenuationLinear);
		sprintf_s(buff, "Attenuation_Quadratic[%d]", a);
		glUniform1f(m_shader->getUniformLocation(buff), m_lights[a].attenuationQuadratic);
	}
	m_shader->disable();
}