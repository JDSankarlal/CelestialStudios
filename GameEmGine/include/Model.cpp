#include "Model.h" 
#include "ResourceManager.h"
#include <ctime>
Model::Model()
{}

Model::Model(Model& model) :
	m_transform(model.m_transform),
	m_mesh(model.m_mesh),
	m_colour(model.m_colour)
{}

Model::Model(const char * path)
{
	loadModel(path);
}

Model::~Model()
{}


void Model::render(GLSLCompiler& shader, Camera& cam)
{
	shader.enable();
	float colour[4]{ (float)m_colour.r / 255,(float)m_colour.g / 255,(float)m_colour.b / 255,(float)m_colour.a / 255 };

	// update the position of the object

	/// - Lighting Variables - ///

	glUniformMatrix4fv(shader.getUniformLocation("uModel"), 1, GL_FALSE, &((m_transform.getTransformation())[0][0]));

	glUniform4f(shader.getUniformLocation("LightPosition"), 2.0f, 0.0f, 0.0f, 1.0f);

	glUniform3f(shader.getUniformLocation("LightAmbient"), 0.6f, 0.6f, 0.6f);
	glUniform3f(shader.getUniformLocation("LightDiffuse"), 0.0f, 0.0f, 1.0f);
	glUniform3f(shader.getUniformLocation("LightSpecular"), 0.8f, 0.2f, 0.2f);

	glUniform1f(shader.getUniformLocation("LightSpecularExponent"), 50.0f);
	glUniform1f(shader.getUniformLocation("Attenuation_Constant"), 1.0f);
	glUniform1f(shader.getUniformLocation("Attenuation_Linear"), 0.1f);
	glUniform1f(shader.getUniformLocation("Attenuation_Quadratic"), 0.01);


	glUniform1f(shader.getUniformLocation("utime"), (float)clock() / 1000);

	glUniform4fv(shader.getUniformLocation("colourMod"), 1, colour);

	//if (m_transform.isUpdated())
	transformedUpdate();

	//render the mesh
	m_mesh.render(shader);

	shader.disable();
}

Transformer& Model::getTransformer()
{
	return m_transform;
}

void Model::setColour(float r, float g, float b, float a)
{
	m_colour.set(255 * r, 255 * g, 255 * b, 255 * a);
}

void Model::setColour(float r, float g, float b)
{
	m_colour.set(255 * r, 255 * g, 255 * b);
}

bool Model::loadModel(const char * path)
{
	return m_mesh.loadMesh(path);
}

float Model::getWidth()
{
	return m_mesh.right.x - m_mesh.left.x;
}

float Model::getHeight()
{

	return m_mesh.top.y - m_mesh.bottom.y;
}

float Model::getDepth()
{
	return m_mesh.front.z - m_mesh.back.z;
}

Coord3D Model::getCenter()
{
	return Coord3D((m_mesh.right.x + m_mesh.left.x )/2, (m_mesh.top.y + m_mesh.bottom.y)/2, (m_mesh.front.z + m_mesh.back.z)/2);
}

void Model::transformedUpdate()
{

	std::vector<glm::vec4  > thing{
		glm::vec4(m_mesh.right.x, m_mesh.right.y, m_mesh.right.z, 1), glm::vec4(m_mesh.left.x, m_mesh.left.y, m_mesh.left.z, 1),
			glm::vec4(m_mesh.top.x, m_mesh.top.y, m_mesh.top.z, 1), glm::vec4(m_mesh.bottom.x, m_mesh.bottom.y, m_mesh.bottom.z, 1),
			glm::vec4(m_mesh.front.x, m_mesh.front.y, m_mesh.front.z, 1), glm::vec4(m_mesh.back.x, m_mesh.back.y, m_mesh.back.z, 1) };

	for (auto &a : thing)
	{
		a = m_transform.getTransformation() *a;

		m_mesh.front = a.z > m_mesh.front.z ? Coord3D(a.x, a.y, a.z) : m_mesh.front;
		m_mesh.back = a.z < m_mesh.back.z ? Coord3D(a.x, a.y, a.z) : m_mesh.back;
		m_mesh.left = a.x < m_mesh.left.x ? Coord3D(a.x, a.y, a.z) : m_mesh.left;
		m_mesh.right = a.x > m_mesh.right.x ? Coord3D(a.x, a.y, a.z) : m_mesh.right;
		m_mesh.top = a.y > m_mesh.top.y ? Coord3D(a.x, a.y, a.z) : m_mesh.top;
		m_mesh.bottom = a.y < m_mesh.bottom.y ? Coord3D(a.x, a.y, a.z) : m_mesh.bottom;
	}
}


