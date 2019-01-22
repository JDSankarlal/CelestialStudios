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

	void setRotation(Coord3D direction, Coord3D forward = { 1,0,0 });
	void rotateBy(Coord3D direction, Coord3D forward = { 1,0,0 });

	/*SET POSITION*/

	void setPosition(float x, float y, float z, Coord3D forward = { 1,0,0 });
	void setPosition(Coord3D pos, Coord3D forward = { 1,0,0 });
	void translateBy(float x, float y, float z, Coord3D forward = { 1,0,0 });
	void translateBy(Coord3D pos, Coord3D forward = { 1,0,0 });


	/*SET SCALE*/

	void setScale(float scale);
	void scaleBy(float scale);
	void setScale(float x, float y, float z);
	void scaleBy(float x, float y, float z);

	/*GETERS*/
	Coord3D getPosition();
	Coord3D getRotation();
	glm::mat4& getRotationMatrix();
	glm::mat4& getScaleMatrix();
	glm::mat4& getTranslationMatrix();

	/*Gets a combination of the rotation, scale, and translation matricies*/
	glm::mat4 getTransformation();

	void resetUpdated();

	bool isUpdated();
	bool isScaleUpdated();
	bool isRotationUpdated();
	bool isTranslatinUpdated();
private:
	Coord3D m_pos, m_angles;
	glm::mat4
		m_translate,
		m_rotate,
		m_scale;

	bool  m_updatedRot = true,
		m_updatedTrans= true,
		m_updatedScale= true;

};

