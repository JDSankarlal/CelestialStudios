#pragma once
#include <vector>
#include "Shader.h"
#include "Transformer.h"
#include "Model.h"
#include "Camera.h"

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
	Transformer *transform = new Transformer;
	Model* parent;
	ColourRGBA diffuse, specular;
	Coord3D position, direction = {0,-1,0};
	float angleConstraint = 45,
		specularExponent = 100,
		attenuationConst = .1f,
		attenuationLinear = 0.01f,
		attenuationQuadratic = 0.1f;

};

class LightSource
{
public:

	static void setLightType(LIGHT_TYPE type, unsigned m_index);

	static void setPosition(Coord3D pos, unsigned m_index);

	static void setSceneAmbient(ColourRGBA ambi);

	static void setDirection(Coord3D dir, int m_index);

	static void setDiffuse(ColourRGBA diff, unsigned m_index);

	static void setSpecular(ColourRGBA spec, unsigned m_index);

	static void setSpecularExponent(float specEx, unsigned m_index);

	static void setAttenuationConstant(float attenConst, unsigned m_index);

	static void setAttenuationLinear(float attenLinear, unsigned m_index);

	static void setAttenuationQuadratic(float attenQuad, unsigned m_index);

	static void setParent(Model* parent, unsigned index);

	static void setCamera(Camera* cam);

	static void setShader(Shader* shad);

	static void setLightAmount(unsigned size);

	static unsigned size();

	static std::vector<FrameBuffer*> shadowBuffers(unsigned w, unsigned h, std::vector<Model*>&, unsigned index);

	static LightInfo getLightInfo(unsigned index);

	static void update();
private:
	//Coord3D m_coord, m_spec;
	static ColourRGBA m_ambient;
	static std::vector<LightInfo >m_lights;
	static std::vector<std::vector<FrameBuffer*>>m_shadows;
	static unsigned m_size;

	static Shader *m_shader;
	static Camera* m_cam;
	static LightInfo m_info;

};

