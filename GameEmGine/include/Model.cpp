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

	
	glUniformMatrix4fv(shader.getUniformLocation("uModel"), 1, GL_FALSE, &((m_transform.getTransformation())[0][0]));


	glUniform4f(shader.getUniformLocation("LightPosition"), 0,0,0,0);
	
	glUniform3f(shader.getUniformLocation("LightAmbient"), 0.8f, .8f, .8f);
	glUniform3f(shader.getUniformLocation("LightDiffuse"), 0.7f, 0.1f, 0.2f);
	glUniform3f(shader.getUniformLocation("LightSpecular"), 0.8f, 0.1f, 0.1f);
	
	glUniform1f(shader.getUniformLocation("LightSpecularExponent"), 50.0f);
	glUniform1f(shader.getUniformLocation("Attenuation_Constant"), 5.f);
	glUniform1f(shader.getUniformLocation("Attenuation_Linear"), 5.0f);
	glUniform1f(shader.getUniformLocation("Attenuation_Quadratic"), 5.f);

	
	glUniform1f(shader.getUniformLocation("utime"), (float)clock() / 1000);

	glUniform4fv(shader.getUniformLocation("colourMod"), 1, colour);
	
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


