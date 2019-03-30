#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstddef>
#include "ResourceManager.h"
#include "Shader.h"
#include "Quat.h"
#include "Utilities.h"
class Sprite : protected VboInfo2D
{
public:
	Sprite();
	Sprite(VboInfo2D);
	Sprite(VboInfo2D, const char* path);
	~Sprite();
	void draw();
	void setPosition(float, float);
	void setSize(float width, float height);
	void setRotation(float);
	void rotateBy(float);

	Size2D& getSize();

private:
	void init();
	void init(const char* path);
	GLuint _vboID,_bound;
	Texture2D _texture;
	Vertex2D _vertData[6];
	static Shader* _textureShader;
};

