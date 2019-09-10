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
//#include "FrameBuffer.h"

class Camera;

class Model: public Transformer
{
public:
	Model();
	Model(Model& model, const char* tag = "");
	Model(primitiveMesh* model, const char* tag = "");
	Model(const char* path, const char* tag = "");
	virtual ~Model();

	bool collision2D(Model* k, Coord3D<float> ignore);

	static bool collision2D(Model* l, Model* k, Coord3D<float> ignore);

	bool collision3D(Model* k);

	static bool collision3D(Model* l, Model* k);

	static bool getSeparatingPlane(const Coord3D<>& RPos, const Coord3D<>& Plane, Model& box1, Model& box2);
	
	virtual void render(Shader& shader, Camera* cam);

	void setColour(float r, float g, float b, float a);

	void setColour(float r, float g, float b);

	void setColour(ColourRGBA colour);

	ColourRGBA getColour();

	bool loadModel(const char* path);

	void enableBoundingBox(bool enable);

	void addAnimation(std::string tag, Animation* animation);

	//	void addFrameBuffer(FrameBuffer* buffer);
	//
	//	void removeFrameBuffer(std::string tag);
	//
	//	std::unordered_map<std::string, FrameBuffer*>& getFrameBuffers();

	float getWidth();
	float getHeight();
	float getDepth();

	Coord3D<> getSize();

	Coord3D<> getCenter();

	void boundingBoxUpdate();

	Animation* getAnimation(const char* tag);
	Animation* getCurrentAnimation();
	void  setAnimation(const char* tag);

	Mesh* getMesh();
	Shader* getShader();

	void replaceTexture(int mesh,int index,GLuint tex);

	void setToRender(bool render);
	void setTransparent(bool trans);
	bool isTransparent();
	void print();

protected:
	ColourRGBA m_colour;

private:
	void boundingBoxInit();
	void drawBoundingBox();

	bool m_render = true;
	bool m_transparent = false;
	const char* m_tag;

	std::unordered_map< std::string, Animation*>m_animations;
	std::string m_animation;
	Mesh* m_mesh;

	Camera* m_camera;

	GLuint m_BBVaoID = 0, m_BBVboID = 0;
	//Transformer m_transform;

	Coord3D<>
		m_topLeftBack,
		m_topRightBack,
		m_topLeftFront,
		m_topRightFront,
		m_bottomLeftBack,
		m_bottomRightBack,
		m_bottomLeftFront,
		m_bottomRightFront,
		m_center;

	float m_width, m_height, m_depth;
	Shader* m_shader, * m_shaderBB;
	Vertex3D m_vertBBDat[12 * 3];
	glm::mat4 m_transBB;
	bool m_enableBB = false,m_copy=false;

	//std::unordered_map<std::string, FrameBuffer*> m_frameBuffers;
	//std::vector<Model*> m_children;
	//Model* m_parent;
	//std::vector <Texture2D> loadedTextures;
	//std::string dir;



};
