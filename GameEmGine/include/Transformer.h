#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Quat.h"
#include "Utilities.h"

enum CLASS_TYPE
{
	TRANSFORMER,
	MODEL,
	TEXT
};

class Transformer
{
public:

	Transformer();
	~Transformer();

	void reset();
	void enableFPSMode(bool enable = true);
	static void applyAspect(bool enable = true);

	/*SET ROTATION*/

	virtual void rotate(Coord3D<> angles);
	virtual void rotate(float x,float y ,float z);
	virtual void rotateBy(Coord3D<> angles);
	virtual void rotateBy(float x, float y, float z);

	/*SET POSITION*/

	virtual void translate(float x, float y, float z);
	virtual void translate(Coord3D<> pos);
	virtual void translateBy(float x, float y, float z);
	virtual void translateBy(Coord3D<> pos);


	/*SET SCALE*/

	virtual void scaleBy(float scale);
	virtual void scaleBy(float x, float y, float z);
	virtual void setScale(Coord3D<> scale);
	virtual void setScale(float scale);
	virtual void setScale(float x, float y, float z);

	/*GETTERS*/
	virtual Coord3D<> getPosition();
	virtual Coord3D<> getRotation();
	virtual Coord3D<> getScale();
	Coord3D<> getForward();
	Coord3D<> getUp();
	Coord3D<> getRight();

	glm::mat4 getRotationMatrix();
	glm::mat4 getScaleMatrix();
	glm::mat4 getTranslationMatrix();

	/*Gets a combination of the rotation, scale, and translation matricies*/
	glm::mat4 getTransformation();

	virtual void resetUpdated();
	virtual bool isUpdated();
	virtual bool isScaleUpdated();
	virtual bool isRotationUpdated();
	virtual bool isTranslatinUpdated();

	virtual void addChild(Transformer* child);
	virtual void removeChild(Transformer* child);
	virtual Transformer* getChild(unsigned int index);
	virtual std::vector<Transformer*>& getChildren();

	CLASS_TYPE getType();
private:

	Coord3D<> m_posDat, m_rotDat, m_scaleDat;
	Coord3D<> m_forward = {0,0,1}, m_up = {0,1,0}, m_right = {1,0,0};
	glm::mat4
		m_translate,
		m_rotate,
		m_scale;

	std::vector<Transformer* >m_children;
	Transformer* m_parent;
	bool  m_updatedRot = true,
		m_updatedTrans = true,
		m_updatedScale = true,
		m_fps;
	static bool
		m_aspect;

protected:
	CLASS_TYPE m_type;
};

