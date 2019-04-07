#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Quat.h"
#include "Utilities.h"

class Transformer
{
public:

	Transformer();
	~Transformer();

	void reset();

	/*SET ROTATION*/

	void setRotation(Coord3D direction);
	void rotateBy(Coord3D direction);

	/*SET POSITION*/

	void setPosition(float x, float y, float z);
	void setPosition(Coord3D pos);
	void translateBy(float x, float y, float z);
	void translateBy(Coord3D pos);


	/*SET SCALE*/

	void setScale(float scale);
	void scaleBy(float scale);
	void setScale(Coord3D);
	void setScale(float x, float y, float z);
	void scaleBy(float x, float y, float z);

	/*GETERS*/
	Coord3D getPosition();
	Coord3D getRotation();
	Coord3D getScale();
	glm::mat4& getRotationMatrix();
	glm::mat4& getScaleMatrix();
	glm::mat4& getTranslationMatrix();

	/*Gets a combination of the rotation, scale, and translation matricies*/
	glm::mat4 getTransformation();

	void resetUpdated();

	void enableDirCam(bool enable);

	bool isUpdated();
	bool isScaleUpdated();
	bool isRotationUpdated();
	bool isTranslatinUpdated();

	void addChild(Transformer* child);
private:
	Coord3D m_posData, m_rotData, m_scaleData;
	glm::vec3 m_forward = {0,0,1}, m_up{0,1,0}, m_right{1,0,0};
	glm::mat4
		m_translate,
		m_rotate,
		m_scale;

	std::vector<Transformer* >m_child;
	Transformer* m_parent;
	bool  m_updatedRot = true,
		m_updatedTrans = true,
		m_updatedScale = true;

};

