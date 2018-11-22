#pragma once
#include "GLSLCompiler.h"
#include "Transformer.h"
#include "Model.h"

enum LIGHT_TYPE
{
	NONE,
	DIRECTIONAL,
	POINT
};

struct LightInfo
{
	ColourRGBA ambient, diffuse, specular;
	Coord3D position, direction = { 0,0,1 };
	float angleConstraint = 45, attenuationConst = .1;

};

class LightSource
{
public:
	LightSource();
	~LightSource();

	void initLight(LIGHT_TYPE type, Camera * cam, GLSLCompiler * shader, LightInfo info, unsigned index= 0);

	void update();
private:
	GLuint m_index;
	LightInfo m_info;
	Transformer m_trans;
	Model* m_child;
};

