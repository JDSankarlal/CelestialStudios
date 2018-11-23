#include "LightSource.h"

LightSource::LightSource()
{}

LightSource::~LightSource()
{}

void LightSource::initLight(LIGHT_TYPE type, Camera * cam, GLSLCompiler * shader, LightInfo info, unsigned index)
{
	char buff[90];

	sprintf_s(buff, "LightPosition[%d]", index);
	glUniform4fv(shader->getUniformLocation(buff), 1, &(cam->getObjectMatrix()*glm::vec4(0.0f, 3.0f, 10.0f, 1.0f))[0]);
	sprintf_s(buff, "LightAmbient[%d]", index);
	glUniform3f(shader->getUniformLocation(buff), info.ambient[0]/255.0f, info.ambient[1] / 255.0f, info.ambient[2] / 255.0f);
	sprintf_s(buff, "LightDiffuse[%d]", index);
	glUniform3f(shader->getUniformLocation(buff), info.diffuse[0] / 255.0f, info.diffuse[1] / 255.0f, info.diffuse[2] / 255.0f);
	sprintf_s(buff, "LightPosition[%d]", index);
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

void LightSource::update()
{}
