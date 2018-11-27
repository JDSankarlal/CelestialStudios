#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>	  
#include <vector>
//#include <map>
#include "Camera.h"
#include "Shader.h"
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

	void render(Shader& shader, Camera& cam);

	Transformer& getTransformer();

	void setColour(float r, float g, float b, float a);

	void setColour(float r, float g, float b);

	bool loadModel(const char* path);

	void enableBoundingBox(bool enable);

	float getWidth();
	float getHeight();
	float getDepth();

	Coord3D getCenter();


private:
	void boundingBoxUpdate();
	void boundingBoxInit();
	void drawBoundingBox();

	GLuint m_BBVaoID = 0, m_BBVboID = 0;
	Transformer m_transform;
	//Physics3D *physics;
	Coord3D m_left, m_right, m_top, m_bottom, m_front, m_back,m_center;
	float m_width, m_height, m_depth;
	Mesh m_mesh;
	ColourRGBA m_colour;
	Shader m_shaderBB;
	Vertex3D *m_vertBBDat = new Vertex3D[12 * 3];
	glm::mat4 m_transBB;
	bool m_enableBB = false;
	//std::vector <Texture2D> loadedTextures;
	//std::string dir;



};
