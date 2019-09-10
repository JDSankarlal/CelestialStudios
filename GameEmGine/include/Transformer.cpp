#include "Transformer.h"
bool Transformer::m_aspect = false;

Transformer::Transformer():m_translate(1), m_rotate(1), m_scale(1), m_scaleDat(1)
{}

Transformer::~Transformer()
{}

void Transformer::reset()
{
	m_translate = m_rotate = m_scale = glm::mat4(1);
	m_posDat = m_rotDat = m_scaleDat = {0,0,0};
}

void Transformer::applyAspect(bool enable)
{
	m_aspect = enable;
}

void Transformer::enableFPSMode(bool enable)
{
	m_fps = enable;
	if(!enable)
	{
		m_forward = {0,0,1};
		m_up = {0,1,0};
		m_right = {1,0,0};
	}
}

void Transformer::rotate(Coord3D<> angles)
{
	m_updatedRot = true;
	m_rotate = glm::mat4(1);


	m_rotDat.z = fmodf(angles.z, 360.f);
	m_rotDat.y = fmodf(angles.y, 360.f);
	m_rotDat.x = fmodf(angles.x, 360.f);

	if(angles.z)
		m_rotate *= Quat::quatRotationMat(glm::radians(m_rotDat.z), Coord3D<>{0, 0, 1});

	if(angles.y)
		m_rotate *= Quat::quatRotationMat(glm::radians(m_rotDat.y), Coord3D<>{0, 1, 0});

	if(angles.x)
		m_rotate *= Quat::quatRotationMat(glm::radians(m_rotDat.x), Coord3D<>{1, 0, 0});



}

void Transformer::rotate(float x, float y, float z)
{
	Transformer::rotate({x,y,z});
}

void Transformer::rotateBy(Coord3D<> angles)
{
	Transformer::rotate(angles + m_rotDat);
}

void Transformer::rotateBy(float x, float y, float z)
{
	Transformer::rotateBy({x,y,z});
}

void Transformer::translateBy(float x, float y, float z)
{
	Transformer::translateBy({x, y, z});
}

void Transformer::translateBy(Coord3D<> pos)
{
	m_updatedTrans = true;

	if(m_fps)
	{
		m_forward = reclass(Coord3D<>, m_rotate * glm::vec4(0, 0, 1, 1));
		m_up = reclass(Coord3D<>, m_rotate * glm::vec4(0, 1, 0, 1));
		m_right = reclass(Coord3D<>, m_rotate * glm::vec4(1, 0, 0, 1));

		m_forward.normalize();
		m_up.normalize();
		m_right.normalize();
	}

	int w, h; float aspect;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
	aspect = m_aspect ? (float)w / h : 1;
	pos *= aspect;
	
	m_translate = glm::translate(m_translate, reclass(glm::vec3, ((m_forward * -pos.z) + (m_up * pos.y) + (m_right * pos.x))));
	
	pos /= aspect;
	m_posDat += ((m_forward * -pos.z) + (m_up * pos.y) + (m_right * pos.x));

}

void Transformer::translate(float x, float y, float z)
{
	Transformer::translate(Coord3D<>{x, y, z});
}

void Transformer::translate(Coord3D<> pos)
{
	m_updatedTrans = true;

	if(m_fps)
	{
		m_forward = reclass(Coord3D<>, m_rotate * glm::vec4(0, 0, 1, 1));
		m_up = reclass(Coord3D<>, m_rotate * glm::vec4(0, 1, 0, 1));
		m_right = reclass(Coord3D<>, m_rotate * glm::vec4(1, 0, 0, 1));

		m_forward.normalize();
		m_up.normalize();
		m_right.normalize();
	}

	int w, h; float aspect;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
	aspect = m_aspect ? (float)w / h : 1;

	pos *= aspect;
	m_translate = glm::translate(glm::mat4(1), reclass(glm::vec3, ((m_forward * -pos.z) + (m_up * pos.y) + (m_right * pos.x))));

	pos /= aspect;
	m_posDat = pos;

}

void Transformer::scaleBy(float scale)
{
	Transformer::scaleBy(m_scaleDat.x + scale, m_scaleDat.y + scale, m_scaleDat.z + scale);
}

void Transformer::scaleBy(float x, float y, float z)
{
	Transformer::setScale(m_scaleDat + Coord3D<>(x, y, z));
}

void Transformer::setScale(float scale)
{
	Transformer::setScale(scale, scale, scale);
}

void Transformer::setScale(Coord3D<> scale)
{
	Transformer::setScale(scale.x, scale.y, scale.z);
}

void Transformer::setScale(float x, float y, float z)
{
	m_updatedScale = true;
	m_scaleDat = Coord3D<>(x, y, z);
	m_scale = glm::scale(glm::mat4(1), reclass(glm::vec3, m_scaleDat));
}

Coord3D<> Transformer::getPosition()
{
	return m_posDat;
}

Coord3D<> Transformer::getRotation()
{
	return m_rotDat;
}

Coord3D<> Transformer::getScale()
{
	return m_scaleDat;
}

Coord3D<> Transformer::getForward()
{
	return m_forward;
}

Coord3D<> Transformer::getUp()
{
	return m_up;
}

Coord3D<> Transformer::getRight()
{
	return m_right;
}

glm::mat4 Transformer::getRotationMatrix()
{
	for(auto& a : m_children)
		a->m_updatedRot = true;

	static glm::mat4 tmp; tmp = m_rotate;
	static Transformer* parent; parent = m_parent;

	if(m_parent)
		while(parent)
			tmp = parent->m_rotate * tmp,
			parent = parent->m_parent;


	return tmp;
}

glm::mat4 Transformer::getScaleMatrix()
{
	for(auto& a : m_children)
		a->m_updatedScale = true;

	static glm::mat4 tmp; tmp = m_scale;
	static Transformer* parent; parent = m_parent;
	if(m_parent)
		while(parent)
			tmp = parent->m_scale * tmp,
			parent = parent->m_parent;


	return tmp;
}

glm::mat4 Transformer::getTranslationMatrix()
{
	for(auto& a : m_children)
		a->m_updatedTrans = true;

	static glm::mat4 tmp; tmp = m_translate;
	static Transformer* parent; parent = m_parent;

	if(m_parent)
		while(parent)
			tmp = parent->m_translate * tmp,
			parent = parent->m_parent;

	return tmp;
}

glm::mat4 Transformer::getTransformation()
{

	return   getTranslationMatrix() * getRotationMatrix() * getScaleMatrix();
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
	if(std::find(m_children.begin(), m_children.end(), child) == m_children.end())
		m_children.push_back(child);
	child->m_parent = this;
}

void Transformer::removeChild(Transformer* child)
{
	if(child)
	{
		std::vector<Transformer*>::iterator ref;
		while((ref = std::find(m_children.begin(), m_children.end(), child)) != m_children.end())
			m_children.erase(ref);

	}
}

Transformer* Transformer::getChild(unsigned int index)
{
	return m_children[index];
}

std::vector<Transformer*>& Transformer::getChildren()
{
	return m_children;
}

CLASS_TYPE Transformer::getType()
{
	return m_type;
}
