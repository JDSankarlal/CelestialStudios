#pragma once
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <unordered_map>
#include "Utilities.h"
class Shader
{
public:
	Shader();
	//Shader(Shader& shad);
	~Shader();
	
	//compiles and links shaders
	bool create(const std::string& vertFilePath, const std::string& fragFilePath);
	bool create(const std::string& vertFilePath, const std::string& fragFilePath, const std::string& geoFilePath);

	void createDefault();

	//compiles shaders into code
	bool compileShaders(const std::string& vertFilePath, const std::string& fragFilePath);
	bool compileShaders(const std::string& vertFilePath, const std::string& fragFilePath, const std::string& geoFilePath);

	//links vertx and fragment shaders into a single shader
	bool linkShaders();
	/*
	adds atributes to the shader
	(ONLY USE if in-shader indexes are not specified)
	*/
	void addAtribute(const std::string attributeName, short m_index = 1);

	GLint getAttribLocation(const std::string attributeName);
	GLint getUniformLocation(const char* uniform);

	void sendUniform(const char* uniform, glm::mat4 val);
	void sendUniform(const char* uniform, glm::vec4 val);
	void sendUniform(const char* uniform, Coord3D<> val);
	void sendUniform(const char* uniform, float x, float y, float z);
	void sendUniform(const char* uniform, float x, float y, float z,float w);
	void sendUniform(const char* uniform, float val);
	void sendUniform(const char* uniform, int val);

	//enables shader program for use
	void enable();
	//disables shader program 
	static void disable();

	static void refresh();
private:
	enum Shaders
	{
		VERT_SHADER,
		FRAG_SHADER,
		GEOM_SHADER
	};

	bool compileShader(Shaders shadNum, const std::string filePath, GLuint id);
	void findAtributes();

	std::string m_vtsh = "", m_vtPath = "", m_fmPath = "", m_goPath = "";
	int m_attribNum = 0;
	bool m_enabled = false;

	GLuint
		m_programID = 0,
		m_vertID = 0,
		m_fragID = 0,
		m_geomID = 0;

	static std::unordered_map<GLuint,Shader*> m_shaders;
	//	static GLuint *m_programs, *m_attribs, m_num;

};


