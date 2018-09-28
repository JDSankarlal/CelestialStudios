#include "Mesh.h"

using namespace std;

Mesh::Mesh()
{}

Mesh::Mesh(std::vector<Vertex3D>verts,
		   std::vector<GLuint>indicies,
		   std::vector<Texture2D>textures)
	:m_verts(verts), m_indicies(indicies), m_textures(textures)
{

	init();
}

Mesh::~Mesh()
{}

void Mesh::render(GLSLCompiler& shader)
{
	GLuint diffuseNr = 0, specularNr = 0;
	for(int a = 0; a < m_textures.size(); a++)
	{
		glActiveTexture(GL_TEXTURE0 + a);

		string number;
		string name = m_textures[a].type;
		if(name == "diffuseTexture")
			number = to_string(++diffuseNr);
		else if(name == "specularTexture")
			number = to_string(++specularNr);

		glUniform1i(shader.getUniformLocation(("material." + name + number).c_str()), a);

		glBindTexture(GL_TEXTURE_2D, m_textures[a].id);
	}

	glUniform1i(shader.getUniformLocation("material.shininess"), 16.f);

	glBindVertexArray(m_vaoID);
	glDrawElements(GL_TRIANGLES, m_indicies.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	for(int a = 0; a < m_textures.size(); a++)
	{
		glActiveTexture(GL_TEXTURE0 + a);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::init()
{
	glGenVertexArrays(1, &m_vaoID);
	glGenBuffers(1, &m_vboID);
	glGenBuffers(1, &m_iboID);

	glBindVertexArray(m_vaoID);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, m_verts.size() * sizeof(Vertex3D), m_verts.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicies.size() * sizeof(GLuint), m_indicies.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	//vertex     atributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*) offsetof(Vertex3D, coord));
	//ColourRGBA atributes
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex3D), (void*) offsetof(Vertex3D, colour));
	//UV         atributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*) offsetof(Vertex3D, uv));
	//normal     atributes
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*) offsetof(Vertex3D, norm));

	glBindVertexArray(0);
}