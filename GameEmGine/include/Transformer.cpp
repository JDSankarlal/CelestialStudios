#include "Transformer.h"


Transformer::Transformer() :m_translate(1), m_rotate(1), m_scale(1),m_scale2(1)
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
	glm::vec3 tmp(angles.x, angles.y, angles.z);
	glm::mat4 thing = glm::lookAt(pos = glm::vec3(m_pos.x, m_pos.y, m_pos.z), pos + glm::vec3(forward.x, forward.y, forward.z), {0,1,0});
	tmp = thing * glm::vec4(tmp,1.f);
	
	m_updatedRot = true;

	if(angles.z)
	{
		m_rotate = Quat::quatRotationMat(glm::radians(angles.z), 0, 0, 1);
		m_angles.z = tmp.z;
	}
	if(angles.y)
	{
		m_rotate = Quat::quatRotationMat(glm::radians(angles.y), 0, 1, 0);
		m_angles.y = tmp.y;
	}
	if(angles.x)
	{
		m_rotate = Quat::quatRotationMat(glm::radians(angles.x), 1, 0, 0);
		m_angles.x = tmp.x;
	}

	
}

void Transformer::rotateBy(Coord3D angles, Coord3D forward)
{
	m_updatedRot = true;
	
	glm::vec3 tmp(angles.x, angles.y, angles.z);
	forward.normalize();
	glm::vec3 pos;
	glm::mat4 thing = glm::lookAt(pos = glm::vec3(m_pos.x, m_pos.y, m_pos.z), pos + glm::vec3(forward.x, forward.y, forward.z), { 0,1,0 });
	tmp = thing * glm::vec4(tmp, 1.f);
	
	m_angles += {tmp.x,tmp.y,tmp.z};

	m_rotate = Quat::quatRotationMat(glm::radians(tmp.x), 1, 0, 0);
	m_rotate *= Quat::quatRotationMat(glm::radians(tmp.y), 0, 1, 0);
	m_rotate *= Quat::quatRotationMat(glm::radians(tmp.z), 0, 0, 1);
}

void Transformer::translateBy(float x, float y, float z, Coord3D forward)
{
	forward;
	setPosition(m_pos + Coord3D{x,y,z});
}

void Transformer::translateBy(Coord3D pos, Coord3D forward)
{
	forward;
	setPosition(m_pos + pos);
}

void Transformer::setPosition(float x, float y, float z, Coord3D forward)
{
	forward;
	setPosition(Coord3D{ x,y,z });
}

void Transformer::setPosition(Coord3D pos, Coord3D forward)
{
	forward;
	m_updatedTrans = true;
	int w, h;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
	float aspect = (float)w / h;
	m_pos = pos * aspect;
	m_translate = glm::translate(glm::mat4(1), glm::vec3(m_pos.x, m_pos.y, -m_pos.z));

	m_pos = m_pos / aspect;
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

void Transformer::setScale(Coord3D _Scale)
{
	m_updatedScale = true;
	setScale(_Scale.x, _Scale.y, _Scale.z);
}

void Transformer::setScale(float x, float y, float z)
{
	m_scale2 = Coord3D(x, y, z);
	m_updatedScale = true;
	m_scale = glm::scale(glm::mat4(1.f), glm::vec3(x, y, z));
}

void Transformer::scaleBy(float x, float y, float z)
{
	m_updatedScale = true;
	m_scale2 += Coord3D(x, y, z);
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

Coord3D Transformer::getScale()
{
	return m_scale2;
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
