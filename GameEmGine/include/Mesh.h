#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/mesh.h>
#include <vector>
#include <unordered_map>
#include "Transformer.h"
#include "Shader.h"
#include "Utilities.h"
#include "ResourceManager.h"
#include "FrameBuffer.h"

#define CHAR_BUFF_SIZE 1000

class Mesh
{
public:
	Mesh();
	Mesh(Mesh & mesh);
	~Mesh();

	bool loadMesh(std::string);

	std::vector< std::pair<std::string, std::vector<Vertex3D>>> loadAni(std::string);

	void editVerts(std::vector< std::pair<std::string, std::vector<Vertex3D>>> verts1, std::vector< std::pair<std::string, std::vector<Vertex3D>>> verts2);

	void render(Shader & shader);

	void unload();

	GLuint getNumFaces(int m_index)const;
	GLuint getNumVerticies(int m_index)const;

	Coord3D top , bottom , left, right , front, back;

private:
	void loadMaterials(const char * path);

	void init();

	bool ani;

	std::vector< std::pair<std::string, GLuint>>  m_vaoID;
	std::vector<std::pair<GLuint, GLuint>> m_vboID;
	std::vector<unsigned>
		m_numFaces, m_numVerts;

	std::vector< std::pair<std::string, std::vector<Vertex3D>>> m_unpackedData;
	std::vector<std::pair<std::string, std::vector<Texture2D>>>m_textures;

};
