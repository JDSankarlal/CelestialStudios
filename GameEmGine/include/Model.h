#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>	  
#include <vector>
//#include <map>
#include "Camera.h"
#include "GLSLCompiler.h"
#include "StructInfo.h"
#include "Transformer.h"
#include "Physics3D.h"
#include "Mesh.h"

class Model
{
public:
	Model();
	Model(Model & model);
	Model(const char* path);
	~Model();

	void render(GLSLCompiler& shader,Camera& cam);

	Transformer& getTransformer();

	void setColour(float r, float g, float b, float a);

	void setColour(float r, float g, float b);

	bool loadModel(const char* path);


	float getWidth();
	float getHeight();
	float getDepth();

	Coord3D getCenter();

private:
	Transformer m_transform;
	//Physics3D *physics;
	void transformedUpdate();
	Coord3D m_left, m_right, m_top, m_bottom, m_front, m_back;
	Mesh m_mesh;
	ColourRGBA m_colour;
	//std::vector <Texture2D> loadedTextures;
	//std::string dir;
	
	
	
};
