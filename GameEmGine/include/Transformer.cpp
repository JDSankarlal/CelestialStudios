#include "Transformer.h"


Transformer::Transformer() :m_translate(1), m_rotate(1), m_scale(1)
{}

Transformer::~Transformer()
{}

void Transformer::reset()
{
	m_translate = m_rotate = m_scale = glm::mat4(1);
}

void Transformer::setRotation(Coord3D angles, Coord3D forward)
{
	forward.normalize();
	glm::vec3 pos;
	glm::vec3 tmp(angles.coordX, angles.coordY, angles.coordZ);
	glm::mat4 thing = glm::lookAt(pos = glm::vec3(m_pos.coordX, m_pos.coordY, m_pos.coordZ), pos + glm::vec3(forward.coordX, forward.coordY, forward.coordZ), {0,1,0});
	tmp = thing * glm::vec4(tmp,1.f);
	
	m_updatedRot = true;

	if(angles.coordZ)
	{
		m_rotate = Quat::quatRotationMat(glm::radians(angles.coordZ), 0, 0, 1);
		m_angles.coordZ = tmp.z;
	}
	if(angles.coordY)
	{
		m_rotate = Quat::quatRotationMat(glm::radians(angles.coordY), 0, 1, 0);
		m_angles.coordY = tmp.y;
	}
	if(angles.coordX)
	{
		m_rotate = Quat::quatRotationMat(glm::radians(angles.coordX), 1, 0, 0);
		m_angles.coordX = tmp.x;
	}

	
}

void Transformer::rotateBy(Coord3D angles, Coord3D forward)
{
	m_updatedRot = true;
	
	glm::vec3 tmp(angles.coordX, angles.coordY, angles.coordZ);
	forward.normalize();
	glm::vec3 pos;
	glm::mat4 thing = glm::lookAt(pos = glm::vec3(m_pos.coordX, m_pos.coordY, m_pos.coordZ), pos + glm::vec3(forward.coordX, forward.coordY, forward.coordZ), { 0,1,0 });
	tmp = thing * glm::vec4(tmp, 1.f);
	
	m_angles += {tmp.x,tmp.y,tmp.z};

	m_rotate = Quat::quatRotationMat(glm::radians(tmp.x), 1, 0, 0);
	m_rotate *= Quat::quatRotationMat(glm::radians(tmp.y), 0, 1, 0);
	m_rotate *= Quat::quatRotationMat(glm::radians(tmp.z), 0, 0, 1);
}

void Transformer::setPosition(float x, float y, float z, Coord3D forward)
{
	setPosition(Coord3D{ x,y,z });
}

void Transformer::setPosition(Coord3D pos, Coord3D forward)
{
	m_updatedTrans = true;
	int w, h;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
	float aspect = (float)w / h;
	m_pos = pos * aspect;
	m_translate = glm::translate(glm::mat4(1), glm::vec3(m_pos.coordX, m_pos.coordY, -m_pos.coordZ));
	m_pos = m_pos / aspect;
}

void Transformer::translateBy(float x, float y, float z, Coord3D forward)
{
	setPosition(m_pos + Coord3D{x,y,z});
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

Coord3D Transformer::getRotation()
{
	return m_angles;
}

glm::mat4 & Transformer::getRotationMatrix()
{
	//	m_updatedRot = false;
	return m_rotate;
}

glm::mat4 & Transformer::getScaleMatrix()
{
	//	m_updatedScale = false;
	return m_scale;
}

glm::mat4 & Transformer::getTranslationMatrix()
{
	//	m_updatedTrans = false;
	return m_translate;
}

glm::mat4 Transformer::getTransformation()
{
	//	m_updatedRot = m_updatedTrans
	//		= m_updatedScale = false;
	return   m_translate * m_rotate * m_scale;
}

void Transformer::resetUpdated()
{
	m_updatedRot = m_updatedTrans
		= m_updatedScale = false;
}

bool Transformer::isUpdated()
{
	return m_updatedRot || m_updatedTrans || m_updatedScale;
}

bool Transformer::isScaleUpdated()
{
	return m_updatedScale;
}

bool Transformer::isRotationUpdated()
{
	return m_updatedRot;
}

bool Transformer::isTranslatinUpdated()
{
	return m_updatedTrans;
}
