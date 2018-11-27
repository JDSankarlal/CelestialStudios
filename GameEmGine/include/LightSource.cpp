#include "LightSource.h"

LightSource::LightSource()
{}

LightSource::~LightSource()
{}

void LightSource::initLight(LIGHT_TYPE type, Camera * cam, Shader * shader, LightInfo info, unsigned index)
{
	char buff[90];

	sprintf_s(buff, "LightPosition[%d]", index);
	glUniform4fv(shader->getUniformLocation(buff), 1, &(cam->getObjectMatrix()*glm::vec4(0.0f, 3.0f, 10.0f, 1.0f))[0]);
	sprintf_s(buff, "LightAmbient[%d]", index);
	glUniform3f(shader->getUniformLocation(buff), info.ambient[0] / 255.0f, info.ambient[1] / 255.0f, info.ambient[2] / 255.0f);
	sprintf_s(buff, "LightDiffuse[%d]", index);
	glUniform3f(shader->getUniformLocation(buff), info.diffuse[0] / 255.0f, info.diffuse[1] / 255.0f, info.diffuse[2] / 255.0f);
	sprintf_s(buff, "LightSpecular[%d]", index);
	glUniform3f(shader->getUniformLocation(buff), info.specular[0] / 255.0f, info.specular[1] / 255.0f, info.specular[2] / 255.0f);
	sprintf_s(buff, "LightDirection[%d]", index);
	glUniform3fv(shader->getUniformLocation(buff), 1, &info.direction[0]);
	sprintf_s(buff, "LightSpecularExponent[%d]", index);
	glUniform1f(shader->getUniformLocation(buff), 50.0f);
	sprintf_s(buff, "Attenuation_Constant[%d]", index);
	glUniform1f(shader->getUniformLocation(buff), info.attenuationConst);
	sprintf_s(buff, "Attenuation_Linear[%d]", index);
	glUniform1f(shader->getUniformLocation(buff), 0.1f);
	sprintf_s(buff, "Attenuation_Quadratic[%d]", index);
	glUniform1f(shader->getUniformLocation(buff), 0.01f);
}

void LightSource::setPosition(Coord3D pos, unsigned index)
{
	m_trans.setPosition(pos.coordX, pos.coordY, pos.coordZ);
}

void LightSource::setSceneAmbient(ColourRGBA ambi, unsigned index)
{

}

void LightSource::setDiffuse(ColourRGBA diff, unsigned index)
{

}

void LightSource::setSpecular(ColourRGBA spec, unsigned index)
{

}

void LightSource::setSpecularExponent(float specEx, unsigned index)
{

}

void LightSource::setAttenuationConstant(float attenConst, unsigned index)
{

}

void LightSource::setAttenuationLinear(float attenLinear, unsigned index)
{

}

void LightSource::setAttenuationQuadratic(float attenQuad, unsigned index)
{

}

Transformer* LightSource::getTransformer()
{
	return &m_trans;
}

void LightSource::update()
{
	char buff[90];

	for(unsigned a = 0; a < m_amount; a++)
	{

		sprintf_s(buff, "LightPosition[%d]", a);
		glUniform4fv(m_shader->getUniformLocation(buff), 1, &(m_cam->getCameraMatrix()*glm::vec4(0.0f, 3.0f, 10.0f, 1.0f))[0]);
		sprintf_s(buff, "LightAmbient[%d]", a);
		glUniform3f(m_shader->getUniformLocation(buff), m_ambient[0] / 255.0f, m_ambient[1] / 255.0f, m_ambient[2] / 255.0f);
		sprintf_s(buff, "LightDiffuse[%d]", a);
		glUniform3f(m_shader->getUniformLocation(buff), m_diffuse[0] / 255.0f, m_diffuse[1] / 255.0f, m_diffuse[2] / 255.0f);
		sprintf_s(buff, "LightSpecular[%d]", a);
		glUniform3f(m_shader->getUniformLocation(buff), m_specular[0] / 255.0f, m_specular[1] / 255.0f, m_specular[2] / 255.0f);
		sprintf_s(buff, "LightDirection[%d]", a);
		glUniform3fv(m_shader->getUniformLocation(buff), 1, &info.direction[0]);
		sprintf_s(buff, "LightSpecularExponent[%d]", a);
		glUniform1f(m_shader->getUniformLocation(buff), 50.0f);
		sprintf_s(buff, "Attenuation_Constant[%d]", a);
		glUniform1f(m_shader->getUniformLocation(buff), info.attenuationConst);
		sprintf_s(buff, "Attenuation_Linear[%d]", a);
		glUniform1f(m_shader->getUniformLocation(buff), 0.1f);
		sprintf_s(buff, "Attenuation_Quadratic[%d]", a);
		glUniform1f(m_shader->getUniformLocation(buff), 0.01f);
	}
}