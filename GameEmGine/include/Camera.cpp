#include "Camera.h"

Camera::Camera(Coord3D<> size, CAMERA_TYPE type, ProjectionPeramiters* peram)
	:Transformer(), m_scale(1), m_projMat(1), m_objMat(1), m_cameraUpdate(true)
{
	//m_position = new Coord3D{-.25,-.5,0};
	init(size, type, peram);
}

Camera::~Camera()
{}

void Camera::init(Coord3D<> size, CAMERA_TYPE type, ProjectionPeramiters* peram)
{
	m_size = size;

	m_viewMat = glm::lookAt(glm::vec3(0, 0, .1f), glm::vec3{0,0,0}, glm::vec3{0.0f,1.0f,0.0f});
	setType(type, peram);
}

void Camera::setType(CAMERA_TYPE type, ProjectionPeramiters* peram)
{
	OrthoPeramiters* peram1 = reclass(OrthoPeramiters*, peram);
	FrustumPeramiters* peram2 = reclass(FrustumPeramiters*, peram);
	switch(m_type = type)
	{
	case ORTHOGRAPHIC:
		if(!peram)
			m_projMat = glm::ortho(-m_size.width , m_size.width , -m_size.height , m_size.height , -m_size.depth, m_size.depth);
		else
			m_projMat = glm::ortho(peram1->left, peram1->right, peram1->bottom, peram1->top, peram1->zNear, peram1->zFar);

		break;
	case FRUSTUM:
		if(!peram)
			m_projMat = glm::perspective(glm::radians(75.f), m_size.width / m_size.height, .1f, m_size.depth);
		else
			m_projMat = glm::perspective(glm::radians(peram2->angle), peram2->aspect, peram2->zNear, peram2->zFar);
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
		if(m_isTranslate)
			Transformer::translate(m_position);

		if(m_isTranslateBy)
			Transformer::translateBy(m_positionBy);

		if(m_isRotate)
			m_rotate.y *= -1,
			Transformer::rotate(m_rotate);

		if(m_isRotateBy)
			m_rotateBy.y *= -1,
			Transformer::rotateBy(m_rotateBy);


		Transformer::setScale(m_scale);
		m_objMat = glm::inverse(Transformer::getTranslationMatrix() * Transformer::getRotationMatrix())/* * Transformer::getScaleMatrix()*/;

		m_cameraMat = m_projMat * m_viewMat * m_objMat;

		m_position += m_positionBy;
		m_rotate += m_isRotateBy;
		m_rotateBy = m_positionBy = Coord3D<>{0,0,0};


		m_isRotate = m_isRotateBy =
			m_isTranslate = m_isTranslateBy =
			m_cameraUpdate = false;

		return true;
	}
	return false;
}

void Camera::translate(float x, float y, float z)
{
	translate({x,y,z});
}

void Camera::translate(Coord3D<> position)
{
	m_position = position;
	m_positionBy = {0,0,0};
	m_isTranslate = m_cameraUpdate = true;
}

void Camera::translateBy(float x, float y, float z)
{
	translateBy({x,y,z});
}

void  Camera::translateBy(Coord3D<> position)
{
	m_positionBy += position;
	m_isTranslateBy = m_cameraUpdate = true;
}

void Camera::setScale(const float scale)
{
	m_scale = scale;
	m_cameraUpdate = true;
}

void Camera::rotate(Coord3D<> angle)
{
	m_rotate = angle;
	m_rotateBy = {0,0,0};

	m_isRotate = m_cameraUpdate = true;
}

void Camera::rotate(float x, float y, float z)
{
	rotate({x,y,z});
}

void Camera::rotateBy(Coord3D<> angle)
{
	m_rotateBy += angle;
	m_isRotateBy = m_cameraUpdate = true;
}

void Camera::rotateBy(float x, float y, float z)
{
	rotateBy({x,y,z});
}

void Camera::render(Shader* shader, std::map<void*, Model*>& models, bool trans)
{
	Shader* shader2 = ResourceManager::getShader("shaders/freetype.vtsh", "shaders/freetype.fmsh");
	for(auto& a : models)
		switch(a.second->getType())
		{
		case MODEL:

			if(trans == a.second->isTransparent())
				a.second->render(*shader, this);
			break;

		case TEXT:

			Text* tmp = reclass(Text*, a.second);
			if(trans == tmp->isTransparent())
				tmp->render(*shader2, this);
			break;
		}

}

Coord3D<> Camera::getRotation()
{
	return Transformer::getRotation() * Coord3D<>{1,-1,1};
}

glm::mat4 Camera::getObjectMatrix()
{
	return m_objMat;
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
