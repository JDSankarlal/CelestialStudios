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

enum PRIMITIVE_TYPE
{
	NONE,
	PLANE,
	CUBE
};

struct primitiveMesh
{
	primitiveMesh() {}
	primitiveMesh(Coord3D<> dim, Coord3D<> offset = {}):m_dim(dim), m_offset(offset) {}
	primitiveMesh(float width, float height, float depth, Coord3D<> offset = {})
		:m_dim({width,height,depth}), m_offset(offset) {}

	virtual ~primitiveMesh() {}

	virtual void setDimentions(Coord3D<> dim, Coord3D<> offset = {}) { m_dim = dim; m_offset = offset; }
	virtual void setDimentions(float w, float h, float d = 0, Coord3D<> offset = {}) { m_dim = {w,h,d}; m_offset = offset; }
	virtual void createMesh() = 0;

	Coord3D<> getDimentions() { return m_dim; }

	PRIMITIVE_TYPE& getType() { return type; }

	std::vector<Vertex3D>& getData() { return m_unpackedData; };


	Coord3D<> m_top, m_bottom, m_left, m_right, m_front, m_back;
protected:
	Coord3D<> m_dim, m_offset;

	PRIMITIVE_TYPE type = NONE;
	std::vector<Vertex3D> m_unpackedData;
};

struct PrimitivePlane:public primitiveMesh
{
	PrimitivePlane():primitiveMesh() { type = PLANE; }

	PrimitivePlane(Coord3D<> dim, Coord3D<> offset = {}):primitiveMesh(dim, offset) { type = PLANE; }

	PrimitivePlane(float w, float h, float d = 0, Coord3D<> offset = {})
		:primitiveMesh(w, h, d, offset)
	{
		type = PLANE;
	}

	~PrimitivePlane() {};


	void createMesh()
	{
		m_unpackedData.resize(6);

		float
			halfW = m_dim.width * .5f,
			halfH = m_dim.height * .5f,
			halfD = m_dim.depth * .5f;

		//coords

		//bottom left tri
		m_unpackedData[0].coord = m_offset + Coord3D<>{-halfW, -halfH, halfD};//bottom left
		m_unpackedData[1].coord = m_offset + Coord3D<>{ halfW, -halfH, -halfD};//bottom right
		m_unpackedData[2].coord = m_offset + Coord3D<>{ -halfW, halfH, -halfD};//top left

		//top right tri
		m_unpackedData[3].coord = m_offset + Coord3D<>{  halfW, halfH, -halfD};//top right
		m_unpackedData[4].coord = m_offset + Coord3D<>{ -halfW, halfH, halfD};//top left
		m_unpackedData[5].coord = m_offset + Coord3D<>{ halfW, -halfH, halfD};//bottom right

		m_top = m_bottom = m_left = m_right = m_front = m_back = m_unpackedData[0].coord;

		for(unsigned int a = 1; a < m_unpackedData.size(); a++)
		{
			m_top = m_unpackedData[a].coord.y > m_top.y ? m_unpackedData[a].coord : m_top;
			m_bottom = m_unpackedData[a].coord.y < m_bottom.y ? m_unpackedData[a].coord : m_bottom;
			m_left = m_unpackedData[a].coord.x < m_left.x ? m_unpackedData[a].coord : m_left;
			m_right = m_unpackedData[a].coord.x > m_right.x ? m_unpackedData[a].coord : m_right;
			m_front = m_unpackedData[a].coord.z < m_front.z ? m_unpackedData[a].coord : m_front;
			m_back = m_unpackedData[a].coord.z > m_back.z ? m_unpackedData[a].coord : m_back;
		}


		//normal
		Coord3D<> norm = Coord3D<>::crossProduct(m_unpackedData[0].coord- m_unpackedData[1].coord, m_unpackedData[0].coord- m_unpackedData[2].coord).normal();

		m_unpackedData[0].norm = norm;
		m_unpackedData[1].norm = norm;
		m_unpackedData[2].norm = norm;

		m_unpackedData[3].norm = norm;
		m_unpackedData[4].norm = norm;
		m_unpackedData[5].norm = norm;

		//uv
		m_unpackedData[0].uv = {0.0f, 0.0f};
		m_unpackedData[1].uv = {1.0f, 0.0f};
		m_unpackedData[2].uv = {0.0f, 1.0f};

		m_unpackedData[3].uv = {1.0f, 1.0f};
		m_unpackedData[4].uv = {0.0f, 1.0f};
		m_unpackedData[5].uv = {1.0f, 0.0f};
	}
};

struct primitiveCube: public primitiveMesh
{
	primitiveCube():primitiveMesh() { type = CUBE; }

	primitiveCube(Coord3D<> dim, Coord3D<> offset = {}):primitiveMesh(dim, offset) { type = CUBE; }

	primitiveCube(float w, float h, float d = 0, Coord3D<> offset = {})
		:primitiveMesh(w, h, d, offset)
	{
		type = CUBE;
	}

	~primitiveCube() {}

	void createMesh()
	{
		static PrimitivePlane left, right, top, bottom, front, back;

		float
			halfW = m_dim.width * .5f,
			halfH = m_dim.height * .5f,
			halfD = m_dim.depth * .5f;

		right.setDimentions({0, m_dim.height, m_dim.depth}, m_offset + Coord3D<>{halfW, 0, 0}), left.setDimentions(0, m_dim.height, m_dim.depth, m_offset + Coord3D<>{-halfW, 0, 0});
		top.setDimentions(m_dim.width, 0, m_dim.depth, m_offset + Coord3D<>{0, halfH, 0}), bottom.setDimentions(m_dim.width, 0, m_dim.depth, m_offset + Coord3D<>{0, -halfH, 0});
		front.setDimentions({m_dim.width, m_dim.height, 0}, m_offset + Coord3D<> {0, 0, -halfD}), back.setDimentions({m_dim.width, m_dim.height, 0}, m_offset + Coord3D<>{0, 0, halfD});

		left.createMesh(), right.createMesh(), top.createMesh(), bottom.createMesh(), front.createMesh(), back.createMesh();

		for(auto& a : left.getData())
			a.norm *= -1;
		m_unpackedData.insert(m_unpackedData.end(), left.getData().rbegin(), left.getData().rend());
		m_unpackedData.insert(m_unpackedData.end(), right.getData().begin(), right.getData().end());
		
		for(auto& a : bottom.getData())
			a.norm *= -1;
		m_unpackedData.insert(m_unpackedData.end(), bottom.getData().rbegin(), bottom.getData().rend());
		m_unpackedData.insert(m_unpackedData.end(), top.getData().begin(), top.getData().end());
		
		for(auto& a : front.getData())
			a.norm *= -1;
		m_unpackedData.insert(m_unpackedData.end(), front.getData().rbegin(), front.getData().rend());
		m_unpackedData.insert(m_unpackedData.end(), back.getData().begin(), back.getData().end());

		m_top = m_bottom = m_left = m_right = m_front = m_back = m_unpackedData[0].coord;

		for(unsigned int a = 1; a < m_unpackedData.size(); a++)
		{
			m_top = m_unpackedData[a].coord.y > m_top.y ? m_unpackedData[a].coord : m_top;
			m_bottom = m_unpackedData[a].coord.y < m_bottom.y ? m_unpackedData[a].coord : m_bottom;
			m_left = m_unpackedData[a].coord.x < m_left.x ? m_unpackedData[a].coord : m_left;
			m_right = m_unpackedData[a].coord.x > m_right.x ? m_unpackedData[a].coord : m_right;
			m_front = m_unpackedData[a].coord.z < m_front.z ? m_unpackedData[a].coord : m_front;
			m_back = m_unpackedData[a].coord.z > m_back.z ? m_unpackedData[a].coord : m_back;
		}

	}

};

class Mesh
{
public:
	Mesh();
	Mesh(Mesh& mesh);
	~Mesh();

	bool loadMesh(std::string);

	bool loadPrimitive(primitiveMesh* mesh);

	std::vector< std::pair<std::string, std::vector<Vertex3D>>>& loadAni(std::string);

	void editVerts(std::vector< std::pair<std::string, std::vector<Vertex3D>>> verts1, std::vector< std::pair<std::string, std::vector<Vertex3D>>> verts2);

	void render(Shader& shader);

	void unload();

	void replaceTexture(int mesh, int index, GLuint tex);
	GLuint getNumFaces(int m_index)const;
	GLuint getNumVerticies(int m_index)const;

	Coord3D<> top, bottom, left, right, front, back;

private:
	void loadMaterials(const char* path);

	bool load(std::string path);

	void init();


	std::vector<std::vector<GLuint>> m_replaceTex;

	bool ani;

	std::vector< std::pair<std::string, GLuint>>  m_vaoID;
	std::vector<std::pair<GLuint, GLuint>> m_vboID;
	std::vector<unsigned>
		m_numFaces, m_numVerts;

	std::vector< std::pair<std::string, std::vector<Vertex3D>>> m_unpackedData;
	std::vector<std::pair<std::string, std::vector<Texture2D>>>m_textures;

};
