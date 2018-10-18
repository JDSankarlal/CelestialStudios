#pragma once
#include <GL\glew.h>

#include <MiniMath\Core.h>
#include <string>

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	//Loads a Vertex and a Fragment shader, places them places them in a program
	bool Load(const std::string &vertFile, const std::string &fragFile);
	bool IsLoaded() const;
	//Clear all data from OpenGL
	void UnLoad();
	bool LinkProgram();

	//Use / detach the shader from use
	void Bind() const;
	void UnBind();

	//Requires a relink before OpenGL will register the change
	void AddAttribute(unsigned int index, const std::string &attribName);

	//Returns -1 if the attribute doesn't exist
	int GetAttribLocation(const std::string &attribName);
	//Returns -1 if uniform does not exist
	int GetUniformLocation(const std::string &uniformName);

	//Send data to Shaders
	void SendUniform(const std::string &name, int integer);
	void SendUniform(const std::string &name, unsigned int unsigned_integer);
	void SendUniform(const std::string &name, float scalar);
	void SendUniform(const std::string &name, const vec2 &vector);
	void SendUniform(const std::string &name, const vec3 &vector);
	void SendUniform(const std::string &name, const vec4 &vector);
	void SendUniformMat3(const std::string &name, float *matrix, bool transpose);
	void SendUniformMat4(const std::string &name, float *matrix, bool transpose);

private:

	bool _IsInit = false;
	GLuint _VertexShader = 0;
	GLuint _FragShader = 0;
	GLuint _Program = 0;

	std::string ReadFile(const std::string &fileName) const;
	bool CompileShader(GLuint shader) const;
	void OutputShaderLog(GLuint shader) const;
	void OutputProgramLog() const;

};