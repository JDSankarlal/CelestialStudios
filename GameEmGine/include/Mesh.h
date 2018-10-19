#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <assimp/mesh.h>
#include <vector>
#include <map>
#include "Transformer.h"
#include "GLSLCompiler.h"
#include "StructInfo.h"
#include "ResourceManager.h"

#define CHAR_BUFF_SIZE 125

class Mesh
{
public:
	Mesh();
	Mesh(Mesh & mesh);
	~Mesh();

	bool loadMesh(std::string);

	void render(GLSLCompiler& shader);

	void unload();

	GLuint getNumFaces(int index)const;
	GLuint getNumVerticies(int index)const;

private:
	void loadMaterials(std::string path);

	void init();

	std::vector< std::pair<std::string, GLuint>>  m_vaoID;
	std::vector<GLuint> m_vboID,
		m_numFaces, m_numVerts;

	std::vector< std::pair<std::string, std::vector<Vertex3D>>> m_unpackedData;
	std::vector<std::pair<std::string, Texture2D>>m_textures;
};
