#include "LightSource.h"

#pragma region Static Variables
ColourRGBA LightSource::m_ambient;
std::vector<LightInfo >LightSource::m_lights;

Shader *LightSource::m_shader;
Camera* LightSource::m_cam;
LightInfo LightSource::m_info;
#pragma endregion



void LightSource::setLightType(LIGHT_TYPE type, unsigned index)
{
	m_lights[index].type = type;
}

void LightSource::setPosition(Coord3D pos, unsigned index)
{
	m_lights[index].transform->setPosition(pos.x, pos.y, pos.z);
}

void LightSource::setDirection(Coord3D dir, int index)
{
	m_lights[index].direction = -dir;
}

void LightSource::setSceneAmbient(ColourRGBA ambi)
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
		glm::vec4 pos(lp.x, lp.y, lp.z, 1.0f);
		glm::vec4 dir{ m_lights[a].direction.x,m_lights[a].direction.y ,m_lights[a].direction.z ,1.0f};
		
		if(m_lights[a].parent)
		{
			Transformer *trans = &m_lights[a].parent->getTransformer();
			glm::vec3 pos2(trans->getPosition().x, trans->getPosition().y, trans->getPosition().z);
			glm::mat4 forward = glm::lookAt(pos2, pos2 + glm::vec3(cos(trans->getRotation().x), tan(trans->getRotation().y), sin(trans->getRotation().z)),
											{ 0,1,0 });
			pos = m_lights[a].parent->getTransformer().getTransformation() * m_lights[a].transform->getTransformation()*forward * glm::vec4(pos2, 1.f);
			// dir = forward * dir;
			//m_lights[a].direction = {tmp.x,tmp.y,tmp.z};
		}
		else
		{
			pos = m_lights[a].transform->getTransformation() * pos;
		}

		sprintf_s(buff, "LightType[%d]", a);
		glUniform1i(m_shader->getUniformLocation(buff), (int)m_lights[a].type);
		sprintf_s(buff, "LightPosition[%d]", a);
		glUniform4fv(m_shader->getUniformLocation(buff), 1, &(m_cam->getCameraMatrix()*pos)[0]);
		sprintf_s(buff, "LightDiffuse[%d]", a);
		glUniform3f(m_shader->getUniformLocation(buff), m_lights[a].diffuse[0] / 255.0f, m_lights[a].diffuse[1] / 255.0f, m_lights[a].diffuse[2] / 255.0f);
		sprintf_s(buff, "LightSpecular[%d]", a);
		glUniform3f(m_shader->getUniformLocation(buff), m_lights[a].specular[0] / 255.0f, m_lights[a].specular[1] / 255.0f, m_lights[a].specular[2] / 255.0f);
		sprintf_s(buff, "LightDirection[%d]", a);
		glUniform3fv(m_shader->getUniformLocation(buff), 1, &dir[0]);
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