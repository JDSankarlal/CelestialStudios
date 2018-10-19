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

	std::vector<std::pair<std::string, Texture2D>>& loadMaterials(std::string path);

	void render(GLSLCompiler& shader,Camera& cam);

	Transformer& getTransformer();

	void setColour(float r, float g, float b, float a);

	void setColour(float r, float g, float b);

	bool loadModel(const char* path);
private:
	Transformer m_transform;
	//Physics3D *physics;

	Mesh m_mesh;
	ColourRGBA m_colour;
	//std::vector <Texture2D> loadedTextures;
	//std::string dir;
	
	
	
};
