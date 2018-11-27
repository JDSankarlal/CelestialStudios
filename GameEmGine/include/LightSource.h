#pragma once
#include <vector>
#include "Shader.h"
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
	Coord3D position, direction = { 0,-1,0 };
	float angleConstraint = 45, 
		attenuationConst = .1,
		attenuationLinear=0.01,
		attenuationQuadratic=0.001;

};

class LightSource
{
public:
	LightSource();
	~LightSource();

	void initLight(LIGHT_TYPE type, Camera * cam, Shader * shader, LightInfo info, unsigned index= 0);

	void setPosition(Coord3D pos, unsigned index);

	void setSceneAmbient(ColourRGBA ambi, unsigned index);

	void setDiffuse(ColourRGBA diff, unsigned index);

	void setSpecular(ColourRGBA spec, unsigned index);

	void setSpecularExponent(float specEx, unsigned index);

	void setAttenuationConstant(float attenConst, unsigned index);

	void setAttenuationLinear(float attenLinear, unsigned index);

	void setAttenuationQuadratic(float attenQuad, unsigned index);

	Transformer * getTransformer();

	void update();
private:
	//Coord3D m_coord, m_spec;
	ColourRGBA m_ambient,m_diffuse,m_specular;
	float;

	Shader *m_shader;
	Camera* m_cam;
	GLuint m_amount;
	LightInfo m_info;
	Transformer m_trans;
	Model* m_child;
};

