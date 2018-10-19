#pragma once
#include "Transform.h"
#include <string>
#include <vector>
#include <GL\glew.h>

class Mesh
{
public:
	Mesh();
	~Mesh();

	//Load a mesh and send it to OpenGL
	bool LoadFromFile(const std::string &File);
	// Release data from OpenGL (VRAM)
	void Unload();

	unsigned int GetNumFaces() const;
	unsigned int GetNumVertices() const;

	//Opengl buffers and objects
	GLuint VBO_Verticies = 0;
	GLuint VBO_UVs = 0;
	GLuint VBO_Normals = 0;
	GLuint VAO = 0;

private:
	unsigned int _NumFaces = 0;
	unsigned int _NumVertices = 0;

};