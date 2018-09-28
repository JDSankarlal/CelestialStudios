#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <assimp/mesh.h>
#include <vector>
#include "Transformer.h"
#include "GLSLCompiler.h"
#include "StructInfo.h"
#include "ResourceManager.h"

class Mesh
{
public:
	Mesh();
	Mesh(std::vector<Vertex3D>verts,
			   std::vector<GLuint>indicies,
			   std::vector<Texture2D>textures);
	~Mesh();

	////just a test
	//void updateVerts(Transformer& transform);
	void render(GLSLCompiler& shader);

	std::vector<Vertex3D> m_verts;
	std::vector<GLuint>   m_indicies;
	std::vector<Texture2D> m_textures;
private:
	void init();

	GLuint m_vboID, m_vaoID, m_iboID;
};

