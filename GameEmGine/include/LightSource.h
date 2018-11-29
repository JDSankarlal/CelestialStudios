#pragma once
#include <vector>
#include "Shader.h"
#include "Transformer.h"
#include "Model.h"

enum class LIGHT_TYPE
{	
	DEFAULT,
	POINT,
	DIRECTIONAL,
	SPOTLIGHT
};

struct LightInfo
{
	LIGHT_TYPE type;
	Transformer *transform=new Transformer;
	Model* parent; 
	ColourRGBA diffuse, specular;
	Coord3D position,direction = { 0,-1,0 };
	float angleConstraint = 45,
		specularExponent=100,
		attenuationConst = .1f,
		attenuationLinear=0.01f,
		attenuationQuadratic=0.1f;

};

class LightSource
{
public:
	
	static void setLightType(LIGHT_TYPE type, unsigned index);

	static void setPosition(Coord3D pos, unsigned index);

	static void setSceneAmbient(ColourRGBA ambi);

	static void setDirection(Coord3D dir, int index);

	static 	void setDiffuse(ColourRGBA diff, unsigned index);

	static void setSpecular(ColourRGBA spec, unsigned index);

	static 	void setSpecularExponent(float specEx, unsigned index);

	static void setAttenuationConstant(float attenConst, unsigned index);

	static void setAttenuationLinear(float attenLinear, unsigned index);

	static void setAttenuationQuadratic(float attenQuad, unsigned index);

	static void setParent(Model* parent, unsigned index);

	static void setCamera(Camera* cam);

	static void setShader(Shader* shad);

	static void setLightAmount( unsigned size);

	static void update();
private:
	//Coord3D m_coord, m_spec;
	static ColourRGBA m_ambient;
	static std::vector<LightInfo >m_lights;

	static Shader *m_shader;
	static Camera* m_cam;
	static LightInfo m_info;

};

