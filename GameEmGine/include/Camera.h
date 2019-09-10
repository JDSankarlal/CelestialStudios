#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include "Transformer.h"
#include "Utilities.h"
#include "Quat.h"
#include "Shader.h"
#include "Model.h"
#include "Text.h"

class Model;

enum CAMERA_TYPE
{
	ORTHOGRAPHIC,
	FRUSTUM
};

#pragma region Structs

struct ProjectionPeramiters
{
	ProjectionPeramiters(float znear, float zfar):zNear(znear), zFar(zfar) {}
	~ProjectionPeramiters() = default;

	void setNear(float znear) { zNear = znear; }
	void setFar(float zfar) { zFar = zfar; }
	void setZAxis(float znear, float zfar) { zNear = znear; zFar = zfar; }

	float zNear, zFar;
};

struct OrthoPeramiters:public ProjectionPeramiters
{
	OrthoPeramiters(float  a_left, float a_right, float a_bottom, float a_top, float a_near, float a_far):
		ProjectionPeramiters(a_near, a_far),
		left(a_left), right(a_right), bottom(a_bottom), top(a_top) {}

	void setLeft(float a_left) { left = a_left; }
	void setRight(float a_right) { right = a_right; }
	void setBottom(float a_bottom) { bottom = a_bottom; }
	void setTop(float a_top) { top = a_top; }

	void setXAxis(float a_left, float a_right) { left = a_left; right = a_right; }
	void setYAxis(float a_top, float a_bottom) { top = a_top; bottom = a_bottom; }
	float  left, right, bottom, top;
};

struct FrustumPeramiters:public ProjectionPeramiters
{
	FrustumPeramiters(float a_angle, float a_aspect, float a_near, float a_far):
		ProjectionPeramiters(a_near, a_far),
		angle(a_angle), aspect(a_aspect) {}

	void setAngle(float a_angle) { angle = a_angle; }
	void setAspect(float a_aspect) { aspect = a_aspect; }
	float angle, aspect;
};
#pragma endregion

class Camera:public Transformer
{
public:
	Camera(Coord3D<> = {1,1,1}, CAMERA_TYPE = FRUSTUM, ProjectionPeramiters* peram = nullptr);
	~Camera();

	void init(Coord3D<> = {}, CAMERA_TYPE = FRUSTUM, ProjectionPeramiters* peram = nullptr);
	void setType(CAMERA_TYPE type, ProjectionPeramiters* peram = nullptr);
	bool update();

	/*SET POSITION*/

	void translate(float x, float y, float z);
	void translate(Coord3D<> pos);
	void translateBy(float x, float y, float z);
	void translateBy(Coord3D<> pos);

	/*SET SCALE*/

	void setScale(float scale);

	/*SET ROTATION*/

	void rotate(Coord3D<> angles);
	void rotate(float x, float y, float z);
	void rotateBy(Coord3D<> angles);
	void rotateBy(float x, float y, float z);


	//render objects
	void render(Shader* shader, std::map<void*, Model*>& models, bool transparent = false);

	
	bool isUpdated() { return m_cameraUpdate; }
	
	/*GETTERS*/
	Coord3D<> getRotation();

	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	glm::mat4 getCameraMatrix();
	glm::mat4 getObjectMatrix();

	CAMERA_TYPE getType();

protected:

	bool m_cameraUpdate,
		m_isTranslate, m_isTranslateBy,
		m_isRotate, m_isRotateBy;

	float m_scale;

	Coord3D<> m_size,
		m_position, m_positionBy,
		m_rotate, m_rotateBy;

	CAMERA_TYPE m_type = FRUSTUM;

	glm::mat4 m_cameraMat;
	glm::mat4 m_projMat;
	glm::mat4 m_viewMat;
	glm::mat4 m_objMat;

private:
	/*REMOVED FROM TRANSFORMER*/

	void scaleBy(float scale) { scale; }
	void scaleBy(float x, float y, float z) { x, y, z; }
	void setScale(Coord3D<> scale) { scale; }
	void setScale(float x, float y, float z) { x, y, z; }

	bool isScaleUpdated() { return false; }
	bool isRotationUpdated() { return false; }
	bool isTranslatinUpdated() { return false; }
};

