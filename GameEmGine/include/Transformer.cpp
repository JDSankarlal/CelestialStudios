#include "Transformer.h"


Transformer::Transformer() :m_translate(1), m_rotate(1), m_scale(1)
{}

Transformer::~Transformer()
{}

void Transformer::setRotation(Coord3D angles, Coord3D forward)
{
	m_updatedRot = true;

	if(angles.z)
	{
		m_rotate = Quat::quatRotationMat(glm::radians(angles.z), 0, 0, 1);
		m_angles.z = angles.z;
	}
	if(angles.y)
	{
		m_rotate = Quat::quatRotationMat(glm::radians(angles.y), 0, 1, 0);
		m_angles.y = angles.y;
	}
	if(angles.x)
	{
		m_rotate = Quat::quatRotationMat(glm::radians(angles.x), 1, 0, 0);
		m_angles.x = angles.x;
	}
}

void Transformer::rotateBy(Coord3D angles, Coord3D forward)
{
	m_updatedRot = true;
	m_angles += angles;

	m_rotate = Quat::quatRotationMat(glm::radians(m_angles.x), 1, 0, 0);
	m_rotate *= Quat::quatRotationMat(glm::radians(m_angles.y), 0, 1, 0);
	m_rotate *= Quat::quatRotationMat(glm::radians(m_angles.z), 0, 0, 1);
}

void Transformer::setPosition(float x, float y, float z, Coord3D forward)
{
	m_updatedTrans = true;
	int w, h;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
	float aspect = (float)w / h;
	m_pos = Coord3D(x * aspect, y * aspect, z * aspect);
	m_translate = glm::translate(glm::mat4(1), glm::vec3(m_pos.x, m_pos.y, -m_pos.z));
}

void Transformer::translateBy(float x, float y, float z, Coord3D forward)
{
	m_updatedTrans = true;
	int w, h;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);

	float aspect = (float)w / h;
	 
	m_translate = glm::translate(m_translate, glm::vec3(x* aspect, y* aspect, -z* aspect));
	m_pos+= Coord3D(x * aspect, y *aspect, z *aspect);
}

void Transformer::setScale(float scale)
{
	m_updatedScale = true;
	setScale(scale, scale, scale);
}

void Transformer::scaleBy(float scale)
{
	m_updatedScale = true;
	scaleBy(scale, scale, scale);
}

void Transformer::setScale(float x, float y, float z)
{
	m_updatedScale = true;
	m_scale = glm::scale(glm::mat4(1.f), glm::vec3(x, y, z));
}

void Transformer::scaleBy(float x, float y, float z)
{
	m_updatedScale = true;
	m_scale = glm::scale(m_scale, glm::vec3(x, y, z));
}

Coord3D Transformer::getPosition()
{
	return m_pos;
}

glm::mat4 & Transformer::getRotationMatrix()
{
	m_updatedRot = false;
	return m_rotate;
}

glm::mat4 & Transformer::getScaleMatrix()
{
	m_updatedScale = false;
	return m_scale;
}

glm::mat4 & Transformer::getTranslationMatrix()
{
	m_updatedTrans = false;
	return m_translate;
}

glm::mat4 Transformer::getTransformation()
{
	m_updatedRot = m_updatedTrans
		= m_updatedScale = false;
	return   m_translate * m_rotate * m_scale;
}

bool Transformer::isUpdated()
{
	return m_updatedRot || m_updatedTrans || m_updatedScale;
}
