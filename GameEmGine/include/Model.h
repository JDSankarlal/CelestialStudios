#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>	  
#include <vector>
#include <string>
#include <unordered_map>
#include "Camera.h"
#include "Shader.h"
#include "Utilities.h"
#include "Transformer.h"
#include "Physics3D.h"
#include "Mesh.h"
#include "Animation.h"
#include "FrameBuffer.h"

class Model
{
public:
	Model();
	Model(Model & model);
	Model(const char* path);
	~Model();

	void render(Shader & shader, Camera & cam);

	Transformer& getTransformer();

	void removeChild(Model * child);

	void addChild(Model* child);

	void setColour(float r, float g, float b, float a);

	void setColour(float r, float g, float b);

	void setColour(ColourRGBA colour);

	ColourRGBA getColour();

	bool loadModel(const char* path);

	void enableBoundingBox(bool enable);

	void addAnimation(std::string tag, Animation *animation);

//	void addFrameBuffer(FrameBuffer* buffer);
//
//	void removeFrameBuffer(std::string tag);
//
//	std::unordered_map<std::string, FrameBuffer*>& getFrameBuffers();

	float getWidth();
	float getHeight();
	float getDepth();

	Coord3D getCenter();

	void boundingBoxUpdate();

	Animation* getAnimation(const char* tag);
	void  setAnimation(const char* tag);

	Mesh* getMesh();
	Shader* getShader();

	void setToRender(bool render);

private:
	bool m_render = true;
	std::unordered_map< std::string, Animation *>m_animations;
	std::string m_animation;
	Mesh m_mesh;

	void boundingBoxInit();
	void drawBoundingBox();


	GLuint m_BBVaoID = 0, m_BBVboID = 0;
	Transformer m_transform;
	//Physics3D *physics;
	Coord3D m_left, m_right, m_top, m_bottom, m_front, m_back, m_center;
	float m_width, m_height, m_depth;
	ColourRGBA m_colour;
	Shader m_shader, m_shaderBB;
	Vertex3D *m_vertBBDat = new Vertex3D[12 * 3];
	glm::mat4 m_transBB;
	bool m_enableBB = false;

	//std::unordered_map<std::string, FrameBuffer*> m_frameBuffers;
	std::vector<Model*> m_children;
	Model* m_parent;
	//std::vector <Texture2D> loadedTextures;
	//std::string dir;



};
