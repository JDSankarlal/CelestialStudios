#include "LightSource.h"

#pragma region Static Variables
ColourRGBA LightSource::m_ambient;
std::vector<LightInfo >LightSource::m_lights;
std::vector<std::vector<FrameBuffer*>>LightSource::m_shadows;

Shader* LightSource::m_shader;
Camera* LightSource::m_cam;
LightInfo LightSource::m_info;
unsigned LightSource::m_size;
#pragma endregion



void LightSource::setLightType(LIGHT_TYPE type, unsigned index)
{
	m_lights[index].type = type;

	if(type == LIGHT_TYPE::POINT)
	{
		m_shadows[index].resize(6, new FrameBuffer("Shadow", 0));
		for(int a = 0; a < 6; a++)
		{
			
			m_shadows[index][a]->initDepthTexture(500, 500);

			if(!m_shadows[index][a]->checkFBO())
			{
				printf("%s FBO failed Creation", m_shadows[index][a]->getTag().c_str());
				system("pause");
			}
		}
	}
	else
		m_shadows[index].resize(0);


}

void LightSource::setPosition(Coord3D pos, unsigned m_index)
{
	m_lights[m_index].transform->setPosition(pos.x, pos.y, pos.z);
}

void LightSource::setDirection(Coord3D dir, int m_index)
{
	m_lights[m_index].direction = -dir;
}

void LightSource::setSceneAmbient(ColourRGBA ambi)
{
	m_ambient = ambi;
}

void LightSource::setDiffuse(ColourRGBA diff, unsigned m_index)
{
	m_lights[m_index].diffuse = diff;
}

void LightSource::setSpecular(ColourRGBA spec, unsigned m_index)
{
	m_lights[m_index].specular = spec;
}

void LightSource::setSpecularExponent(float specEx, unsigned m_index)
{
	m_lights[m_index].specularExponent = specEx;
}

void LightSource::setAttenuationConstant(float attenConst, unsigned m_index)
{
	m_lights[m_index].attenuationConst = attenConst;
}

void LightSource::setAttenuationLinear(float attenLinear, unsigned m_index)
{
	m_lights[m_index].attenuationLinear = attenLinear;
}

void LightSource::setAttenuationQuadratic(float attenQuad, unsigned m_index)
{
	m_lights[m_index].attenuationQuadratic = attenQuad;
}

void LightSource::setParent(Model* parent, unsigned m_index)
{
	m_lights[m_index].parent = parent;
}

void LightSource::setCamera(Camera* cam)
{
	m_cam = cam;
}

void LightSource::setShader(Shader* shad)
{
	m_shader = shad;
}

void LightSource::setLightAmount(unsigned size)
{
	m_size = size > 0 ? size : 0;
	m_lights.resize(m_size);
	m_shadows.resize(m_size, std::vector<FrameBuffer*>(0, new FrameBuffer("shadow", 0)));
}

unsigned LightSource::size()
{
	return m_size;
}

std::vector<FrameBuffer*> LightSource::shadowBuffers(unsigned w, unsigned h, std::vector<Model*>& models, unsigned index)
{
	models;
	if(m_lights[index].type == LIGHT_TYPE::POINT)
	{

		glViewport(0, 0, w, h);
		

		static Camera cam;
		cam.init({(float)w,(float)h,500}, ORTHOGRAPHIC);

		cam.setPosition(m_lights[index].transform->getPosition());

		for(int a = 0; a < 6; a++)
		{
		//	m_shadows[index][a]->resizeColour(0, w, h);
			m_shadows[index][a]->resizeDepth(w, h);

			m_shadows[index][a]->enable();
			//m_shader->enable();
			Shader* shad = ResourceManager::getShader("Shaders/ShadowDepth.vtsh","Shaders/ShadowDepth.fmsh");
			shad->enable();
			shad->sendUniform("lightSpaceMatrix", m_cam->getProjectionMatrix() *  glm::lookAt(m_lights[index].position.toVec3(),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f)));



			switch(a)
			{
			case 0:
				cam.setAngle(90, {1,0,0});
				break;
			case 1:
				cam.setAngle(90, {-1,0,0});
				break;
			case 2:
				cam.setAngle(90, {0,1,0});
				break;
			case 3:
				cam.setAngle(90, {0,-1,0});
				break;
			case 4:
				cam.setAngle(0, {0,1, 0});
				break;
			case 5:
				cam.setAngle(180, {0,-1,0});

				break;
			default:
				break;
			}

			cam.render(shad, models);
			
			//Shader* shad = ResourceManager::getShader("Shaders/ShadowShader.vtsh", "Shaders/ShadowShader.frag");
			//shad->enable();
			//shad->sendUniform("depthMap", 0);
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, m_shadows[index][a]->getDepthHandle());
			//FrameBuffer::drawFullScreenQuad();
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, GL_NONE);
		FrameBuffer::disable();
		}

		//m_shader->disable();
	}
	return m_shadows[index];
}

LightInfo LightSource::getLightInfo(unsigned index)
{
	return m_lights[index];
}


void LightSource::update()
{
	char buff[90];
	m_shader->enable();
	m_shader->sendUniform("LightAmount", (int)m_lights.size());
	m_shader->sendUniform("LightAmbient", Coord3D{m_ambient[0] / 255.0f, m_ambient[1] / 255.0f, m_ambient[2] / 255.0f});

	for(unsigned a = 0; a < m_lights.size(); a++)
	{
		Coord3D lp = m_lights[a].transform->getPosition();
		glm::vec4 pos(lp.x, lp.y, lp.z, 1.0f);
		glm::vec4 dir{m_lights[a].direction.x,m_lights[a].direction.y ,m_lights[a].direction.z ,1.0f};

		if(m_lights[a].parent)
		{
			Transformer* trans = &m_lights[a].parent->getTransformer();
			glm::vec3 pos2(trans->getPosition().x, trans->getPosition().y, trans->getPosition().z);
			glm::mat4 forward = glm::lookAt(pos2, pos2 + glm::vec3(cos(trans->getRotation().x), tan(trans->getRotation().y), sin(trans->getRotation().z)),
				{0,1,0});
			pos = m_lights[a].parent->getTransformer().getTransformation() * m_lights[a].transform->getTransformation() * forward * glm::vec4(pos2, 1.f);
			// dir = forward * dir;
			//m_lights[a].direction = {tmp.x,tmp.y,tmp.z};
		}
		else
		{
			pos = m_lights[a].transform->getTransformation() * pos;
		}


		pos = m_cam->getCameraMatrix() * pos;

		sprintf_s(buff, "LightType[%d]", a);
		glUniform1i(m_shader->getUniformLocation(buff), (int)m_lights[a].type);
		sprintf_s(buff, "LightPosition[%d]", a);
		glUniform4fv(m_shader->getUniformLocation(buff), 1, &(pos)[0]);
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