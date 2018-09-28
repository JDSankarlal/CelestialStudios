#include "Transformer.h"


Transformer::Transformer() :m_translate(1), m_rotate(1), m_scale(1)
{}

Transformer::~Transformer()
{}

void Transformer::setRotation(Coord3D angles, Coord3D forward)
{
	m_updated = false;
	m_angles = angles;
	m_rotate = Quat::quatRotationMat(glm::radians(angles.x), 1, 0, 0);
	m_rotate *= Quat::quatRotationMat(glm::radians(angles.y), 0, 1, 0);
	m_rotate *= Quat::quatRotationMat(glm::radians(angles.z), 0, 0, 1);

}

void Transformer::rotateBy(Coord3D angles, Coord3D forward)
{
	m_updated = false;
	m_angles += angles;
	
	m_rotate = Quat::quatRotationMat(glm::radians(m_angles.x), 1, 0, 0);
	m_rotate *= Quat::quatRotationMat(glm::radians(m_angles.y), 0, 1, 0);
	m_rotate *= Quat::quatRotationMat(glm::radians(m_angles.z), 0, 0, 1);
}

void Transformer::setPosition(float x, float y, float z, Coord3D forward)
{
	m_updated = false;
	int w, h;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);

	m_translate = glm::translate(glm::mat4(1), glm::vec3(x / w - .1f, y / h - .1f, -z / 500));
}

void Transformer::translateBy(float x, float y, float z, Coord3D forward)
{
	m_updated = false;
	int w, h;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);

	m_translate = glm::translate(m_translate, glm::vec3(x / w - 1 / 10, y / h - 1 / 10, -z / 500));
}

void Transformer::setScale(float scale)
{
	setScale(scale, scale, scale);
}

void Transformer::scaleBy(float scale)
{
	scaleBy(scale, scale, scale);
}

void Transformer::setScale(float x, float y, float z)
{
	m_updated = false;
	m_scale = glm::scale(glm::mat4(1.f), glm::vec3(x, y, z));
}

void Transformer::scaleBy(float x, float y, float z)
{
	m_updated = false;
	m_scale = glm::scale(m_scale, glm::vec3(x, y, z));
}

glm::mat4 & Transformer::getRotationMatrix()
{
	return m_rotate;
}

glm::mat4 & Transformer::getScaleMatrix()
{
	return m_scale;
}

glm::mat4 & Transformer::getTranslationMatrix()
{
	return m_translate;
}

glm::mat4 Transformer::getTransformation()
{
	m_updated = true;
	return   m_translate * m_rotate * m_scale ;
}

bool Transformer::isUpdated()
{
	return !m_updated;
}
