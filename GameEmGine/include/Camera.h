#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transformer.h"
#include "Utilities.h"
#include "Quat.h"
#include "Shader.h"
#include "Model.h"

class Model;

enum CAMERA_TYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE
};
class Camera
{
public:
	Camera(Size3D = {1,1,1}, CAMERA_TYPE = PERSPECTIVE);
	~Camera();

	void init(Size3D = {}, CAMERA_TYPE = PERSPECTIVE);
	void setType(CAMERA_TYPE type);
	CAMERA_TYPE getType();
	bool update();

	void setPosition(Coord3D);
	void movePositionBy(Coord3D position);
	void setScale(const float);
	void setAngle(float angle, Coord3D direction);

	void moveAngleBy(float angle, Coord3D direction);

	void render(Shader * shader, std::vector<Model*>& models, bool transparent = false);


	Coord3D getPosition();
	float& getScale();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	glm::mat4 getCameraMatrix();

	glm::mat4 getObjectMatrix();
	Transformer& getTransformer();

protected:
	bool m_cameraUpdate;
	float m_scale;
	Size3D *m_size = new Size3D;
	Coord3D *m_position;
	CAMERA_TYPE m_type = PERSPECTIVE;

	Transformer m_transform;
	glm::mat4 m_cameraMat;
	glm::mat4 m_projMat;
	glm::mat4 m_viewMat;
	glm::mat4 m_objMat;
};

