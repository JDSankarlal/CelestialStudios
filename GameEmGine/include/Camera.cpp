#include "Camera.h"

Camera::Camera(Size3D size, CAMERA_TYPE type):m_scale(1), m_projMat(1), m_objMat(1), m_cameraUpdate(true), m_position(new Coord3D{0,0,0})
{
	//m_position = new Coord3D{-.25,-.5,0};
	init(size, type);
}

Camera::~Camera()
{}

void Camera::init(Size3D size, CAMERA_TYPE type)
{
	int w, h;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h); //window size in pixels
	size.width /= w;
	size.height /= h;
	*m_size = size;

	m_viewMat = glm::lookAt(glm::vec3{0,0,0.1f}, glm::vec3{0,0,0}, glm::vec3{0.0f,1.0f,0.0f});
	setType(type);
}

void Camera::setType(CAMERA_TYPE type)
{
	switch(m_type = type)
	{
	case ORTHOGRAPHIC:
		m_projMat = glm::ortho(-m_size->width * 100, m_size->width * 100, -m_size->height * 100, m_size->height * 100, -m_size->depth, m_size->depth);
		break;
	case PERSPECTIVE:
		m_projMat = glm::perspective(glm::radians(75.f), m_size->width / m_size->height, .1f, m_size->depth);
		break;
	default:
		m_projMat = glm::mat4(1);
	}
	m_cameraUpdate = true;
}

CAMERA_TYPE Camera::getType() 
{
	return m_type;
}

bool Camera::update()
{
	if(m_cameraUpdate)
	{

		m_transform.setPosition(*m_position);
		m_transform.setScale(m_scale);
		m_objMat = glm::inverse(m_transform.getTranslationMatrix() * m_transform.getRotationMatrix())/** m_transform.getScaleMatrix()*/;

		m_cameraMat = m_projMat * m_viewMat * m_objMat;
		m_cameraUpdate = false;

		return true;
	}
	return false;
}

void Camera::setPosition(Coord3D position)
{
	*m_position = Coord3D{position.x, position.y, position.z};

	m_cameraUpdate = true;
}

void  Camera::movePositionBy(Coord3D position)
{
	setPosition(*m_position + position);
}

void Camera::setScale(const float scale)
{
	m_scale = scale;
	m_cameraUpdate = true;
}

void Camera::setAngle(float angle, Coord3D direction)
{
	//glm rotation
	//m_rotMat = glm::mat4(1);
	//	m_rotMat = glm::rotate(m_rotMat, glm::radians(-angle), glm::vec3(direction.x, direction.y, direction.z));

	//my rotation
	//m_rotMat = Quat::quatRotationMat(glm::radians(angle), -direction.x, direction.y, direction.z);

	direction.y *= -1;
	m_transform.setRotation(direction * angle);

	m_cameraUpdate = true;
}

void Camera::moveAngleBy(float angle, Coord3D direction)
{
	//glm rotation
	//if(angle != 0)
	//	m_rotMat = glm::rotate(m_rotMat, glm::radians(-angle), glm::vec3(direction.x, direction.y, direction.z));

	//my rotation
	//if(angle != 0)
	//	m_rotMat *= Quat::quatRotationMat(glm::radians(angle), -direction.x, direction.y, direction.z);
	direction.y *= -1;
	m_transform.rotateBy(direction * angle);

	m_cameraUpdate = true;
}

void Camera::render(Shader * shader, std::vector<Model*> & models, bool trans)
{
	for(auto& a : models)
		if(trans == a->isTransparent())
		{
			a->render(*shader, this->m_cameraMat);
		}
}


Coord3D Camera::getPosition()
{
	return -m_transform.getPosition();
}

float& Camera::getScale()
{
	return m_scale;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return m_projMat;
}

glm::mat4 Camera::getViewMatrix()
{
	return m_viewMat;
}

glm::mat4 Camera::getCameraMatrix()
{
	return m_cameraMat;
}

glm::mat4 Camera::getObjectMatrix()
{
	return m_objMat;
}

Transformer& Camera::getTransformer()
{
	return m_transform;
}
