#include "Model.h" 
#include "ResourceManager.h"
#include <ctime>
Model::Model()
{}

Model::Model(Model& model) :
	m_transform(model.m_transform),
	m_mesh(model.m_mesh),
	m_colour(model.m_colour)
{
	m_left = m_mesh.left;
	m_right = m_mesh.right;
	m_top = m_mesh.top;
	m_bottom = m_mesh.bottom;
	m_front = m_mesh.front;
	m_back = m_mesh.back;
}

Model::Model(const char * path)
{
	if(loadModel(path))
	{
		m_left = m_mesh.left;
		m_right = m_mesh.right;
		m_top = m_mesh.top;
		m_bottom = m_mesh.bottom;
		m_front = m_mesh.front;
		m_back = m_mesh.back;
	}
}

Model::~Model()
{}


void Model::render(GLSLCompiler& shader, Camera& cam)
{
	shader.enable();
	float colour[4]{ (float)m_colour.colorR / 255,(float)m_colour.colorG / 255,(float)m_colour.colorB / 255,(float)m_colour.colorA / 255 };

	// update the position of the object
	if(m_transform.isScaleUpdated() || m_transform.isRotationUpdated())
		transformedUpdate();
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


	//render the mesh
	m_mesh.render(shader);

	shader.disable();
	m_transform.resetUpdated();
}

Transformer& Model::getTransformer()
{
	return m_transform;
}

void Model::setColour(float r, float g, float b, float a)
{
	m_colour.set(255 * (GLubyte)r, 255 * (GLubyte)g, 255 * (GLubyte)b, 255 * (GLubyte)a);
}

void Model::setColour(float r, float g, float b)
{
	m_colour.set(255 * (GLubyte)r, 255 * (GLubyte)g, 255 * (GLubyte)b);
}

bool Model::loadModel(const char * path)
{
	return m_mesh.loadMesh(path);
}

float Model::getWidth()
{
	//	transformedUpdate();

	return std::abs(m_right.x - m_left.x);
}

float Model::getHeight()
{

	//transformedUpdate();
	return std::abs(m_top.y - m_bottom.y);
}

float Model::getDepth()
{
	//transformedUpdate();
	return std::abs(m_front.z - m_back.z);
}

Coord3D Model::getCenter()
{
	//if(m_transform.isUpdated())
	//transformedUpdate();
	glm::vec4 tmp = m_transform.getTranslationMatrix() *  glm::vec4((m_right.x + m_left.x) / 2, (m_top.y + m_bottom.y) / 2, (m_front.z + m_back.z) / 2, 1);

	return { tmp.x, tmp.y, tmp.z };
}

void Model::transformedUpdate()
{

	std::vector<glm::vec4  > thing
	{
	glm::vec4(m_right.x, m_right.y, m_right.z, 1), glm::vec4(m_left.x,   m_left.y,   m_left.z, 1),
	glm::vec4(m_top.x,   m_top.y,   m_top.z, 1),   glm::vec4(m_bottom.x, m_bottom.y, m_bottom.z, 1),
	glm::vec4(m_front.x, m_front.y, m_front.z, 1), glm::vec4(m_back.x,   m_back.y,   m_back.z, 1)
	};


	for(auto &a : thing)
	{
		if(m_transform.isScaleUpdated())
			a = m_transform.getScaleMatrix() * a;
		if(m_transform.isRotationUpdated())
			a = m_transform.getRotationMatrix() * a;

		m_front = a.z > m_front.z ? Coord3D(a.x, a.y, a.z) : m_front;
		m_back = a.z < m_back.z ? Coord3D(a.x, a.y, a.z) : m_back;
		m_left = a.x < m_left.x ? Coord3D(a.x, a.y, a.z) : m_left;
		m_right = a.x > m_right.x ? Coord3D(a.x, a.y, a.z) : m_right;
		m_top = a.y > m_top.y ? Coord3D(a.x, a.y, a.z) : m_top;
		m_bottom = a.y < m_bottom.y ? Coord3D(a.x, a.y, a.z) : m_bottom;
	}
}


