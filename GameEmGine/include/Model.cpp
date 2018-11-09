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
	transformedUpdate(cam);
	
	/// - Lighting Variables - ///

	glUniformMatrix4fv(shader.getUniformLocation("uModel"), 1, GL_FALSE, &((m_transform.getTransformation())[0][0]));

	glUniform4f(shader.getUniformLocation("LightPosition"), 2.0f, 3.0f, 0.01f, 1.0f);

	glUniform3f(shader.getUniformLocation("LightAmbient"), 0.6f, 0.6f, 0.6f);
	glUniform3f(shader.getUniformLocation("LightDiffuse"), 0.0f, 0.0f, 1.0f);
	glUniform3f(shader.getUniformLocation("LightSpecular"), 0.8f, 0.2f, 0.2f);

	glUniform1f(shader.getUniformLocation("LightSpecularExponent"), 50.0f);
	glUniform1f(shader.getUniformLocation("Attenuation_Constant"), 1.0f);
	glUniform1f(shader.getUniformLocation("Attenuation_Linear"), 0.1f);
	glUniform1f(shader.getUniformLocation("Attenuation_Quadratic"), 0.01f);

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
	m_colour.set((GLubyte)(255 * r), (GLubyte)(255 * g), (GLubyte)(255 * b), (GLubyte)(255 * a));
}

void Model::setColour(float r, float g, float b)
{
	m_colour.set((GLubyte)(255 * r), (GLubyte)(255 * g), (GLubyte)(255 * b));
}

bool Model::loadModel(const char * path)
{
	return m_mesh.loadMesh(path);
}

float Model::getWidth()
{
	//	transformedUpdate();

	return std::abs(m_right.coordX - m_left.coordX);
}

float Model::getHeight()
{

	//transformedUpdate();
	return std::abs(m_top.coordY - m_bottom.coordY);
}

float Model::getDepth()
{
	//transformedUpdate();
	return std::abs(m_front.coordZ - m_back.coordZ);
}

Coord3D Model::getCenter()
{
	//if(m_transform.isUpdated())
	//transformedUpdate();
	glm::vec4 tmp =  glm::vec4((m_right.coordX + m_left.coordX) / 2, (m_top.coordY + m_bottom.coordY) / 2, (m_front.coordZ + m_back.coordZ) / 2, 1);

	return { tmp.x, tmp.y, tmp.z };
}

void Model::transformedUpdate(Camera& cam)
{
	m_front = m_back = m_top = m_bottom = m_left = m_right = Coord3D(0,0,0);
	std::vector<glm::vec4> thing
	{
	glm::vec4(m_mesh.right.coordX, m_mesh.right.coordY, m_mesh.right.coordZ, 1), glm::vec4(m_mesh.left.coordX,   m_mesh.left.coordY,   m_mesh.left.coordZ, 1),
	glm::vec4(m_mesh.top.coordX,   m_mesh.top.coordY,   m_mesh.top.coordZ, 1),   glm::vec4(m_mesh.bottom.coordX, m_mesh.bottom.coordY, m_mesh.bottom.coordZ, 1),
	glm::vec4(m_mesh.front.coordX, m_mesh.front.coordY, m_mesh.front.coordZ, 1), glm::vec4(m_mesh.back.coordX,   m_mesh.back.coordY,   m_mesh.back.coordZ, 1)
	};


	for(auto &a : thing)
	{
		
			a = cam.getCameraMatrix()*  m_transform.getTransformation() * a;
			

		m_front = a.z > m_front.coordZ ? Coord3D(a.x, a.y, a.z) : m_front;
		m_back = a.z < m_back.coordZ ? Coord3D(a.x, a.y, a.z) : m_back;
		m_left = a.x < m_left.coordX ? Coord3D(a.x, a.y, a.z) : m_left;
		m_right = a.x > m_right.coordX ? Coord3D(a.x, a.y, a.z) : m_right;
		m_top = a.y > m_top.coordY ? Coord3D(a.x, a.y, a.z) : m_top;
		m_bottom = a.y < m_bottom.coordY ? Coord3D(a.x, a.y, a.z) : m_bottom;
	}
}


