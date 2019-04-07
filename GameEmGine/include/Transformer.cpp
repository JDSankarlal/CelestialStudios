#include "Transformer.h"


Transformer::Transformer():m_translate(1), m_rotate(1), m_scale(1), m_scaleData(1)
{}


Transformer::~Transformer()
{}

void Transformer::reset()
{
	m_translate = m_rotate = m_scale = glm::mat4(1);
	m_posData = m_rotData = m_scaleData = {0,0,0};
}

void Transformer::setRotation(Coord3D angles)
{
	//glm::vec3 tmp(angles.x, angles.y, angles.z);
	//tmp = glm::vec4(tmp, 1.0f);

	//m_forward = {cosf(glm::radians(angles.x)),cosf(glm::radians(angles.y)),sinf(glm::radians(angles.x))};
	//m_up = {cosf(glm::radians(angles.y)),sinf(glm::radians(angles.y)),cosf(glm::radians(angles.z))};
	//m_right = {sinf(glm::radians(angles.x)),cosf(glm::radians(angles.z)),cosf(glm::radians(angles.z))};



	m_updatedRot = true;
	m_rotate = glm::mat4(1);
	if(m_parent)
		m_rotate = m_parent->m_rotate;
	if(m_forward.length())
	{
		m_rotate *= Quat::quatRotationMat(glm::radians(angles.z),m_forward);
		m_rotData.z = angles.z;
	}
	if(m_up.length())
	{
		m_rotate *= Quat::quatRotationMat(glm::radians(angles.y), m_up);
		m_rotData.y = angles.y;
	}
	if(m_right.length())
	{
		m_rotate *= Quat::quatRotationMat(glm::radians(angles.x), m_right);
		m_rotData.x = angles.x;
	}


}

void Transformer::rotateBy(Coord3D angles)
{
	m_updatedRot = true;

	//m_forward = {cosf(glm::radians(angles.x)),cosf(glm::radians(angles.y)),sinf(glm::radians(angles.x))};
	//m_up = {sinf(glm::radians(angles.y)),sinf(glm::radians(angles.y)),cosf(glm::radians(angles.z))};
	//m_right = {sinf(glm::radians(angles.x)),cosf(glm::radians(angles.z)),cosf(glm::radians(angles.z))};


	glm::vec3 tmp(angles.x, angles.y, angles.z);
	tmp = glm::vec4(tmp, 1.f);

	m_rotData += {tmp.x, tmp.y, tmp.z};

	setRotation(m_rotData);
	//m_rotate = Quat::quatRotationMat(glm::radians(tmp.x), 1, 0, 0);
	//m_rotate *= Quat::quatRotationMat(glm::radians(tmp.y), 0, 1, 0);
	//m_rotate *= Quat::quatRotationMat(glm::radians(tmp.z), 0, 0, 1);
}

void Transformer::translateBy(float x, float y, float z)
{
	setPosition(m_posData + Coord3D{x,y,z});
}

void Transformer::translateBy(Coord3D pos)
{
	setPosition(m_posData + pos);
}

void Transformer::setPosition(float x, float y, float z)
{
	setPosition(Coord3D{x,y,z});
}

void Transformer::setPosition(Coord3D pos)
{
	m_updatedTrans = true;
	int w, h;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
	float aspect = (float)w / h;
	m_posData = pos * aspect;

	m_translate = glm::translate(glm::mat4(1), (m_forward * -m_posData.z) + (m_up * m_posData.y) + (m_right * m_posData.x));

	m_posData = m_posData / aspect;
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
	m_scaleData = Coord3D(x, y, z);
	m_updatedScale = true;
	m_scale = glm::scale(glm::mat4(1.f), glm::vec3(x, y, z));
}

void Transformer::scaleBy(float x, float y, float z)
{
	m_updatedScale = true;
	m_scaleData += Coord3D(x, y, z);
	m_scale = glm::scale(m_scale, glm::vec3(x, y, z));
}

Coord3D Transformer::getPosition()
{
	return m_posData;
}

Coord3D Transformer::getRotation()
{
	return m_rotData;
}

Coord3D Transformer::getScale()
{
	return m_scaleData;
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

void Transformer::addChild(Transformer* child)
{
	m_child.push_back(child);
	child->m_parent = this;
}
