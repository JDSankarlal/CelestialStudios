#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transformer.h"
#include "StructInfo.h"
#include "Quat.h"

class Camera3D
{
public:
	Camera3D(Size3D = {});
	~Camera3D();

	void init(Size3D);
	bool update();

	void setPosition(Coord3D);
	void movePositionBy(Coord3D position);
	void setScale(const float);
	void setAngle(float angle,Coord3D direction);

	void moveAngleBy(float angle, Coord3D direction);

	Quat& getPosition();
	float& getScale();
	glm::mat4 getCameraMatrix();

	glm::mat4 getObjectMatrix();
	
protected:
	bool m_cameraUpdate;
	float m_scale;
	Size3D *m_size = new Size3D;
	Quat *m_position;

	Transformer m_transform;
	glm::mat4 m_cameraMat;
	glm::mat4 m_projMat;
	glm::mat4 m_rotMat;
	glm::mat4 m_viewMat;
	glm::mat4 m_objMat;
};

