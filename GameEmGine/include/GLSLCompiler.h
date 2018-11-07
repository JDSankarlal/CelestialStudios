#pragma once
#include <GL\glew.h>
#include <string>
#include <fstream>
#include "Logger.h"

class GLSLCompiler
{
public:
	GLSLCompiler();
	~GLSLCompiler();

	void refresh();

	//compiles and links shaders
	void create(const std::string& vertFilePath, const std::string& fragFilePath);

	void createDefault();

	//compiles shaders into code
	void compileShaders(const std::string& vertFilePath, const std::string& fragFilePath);
	//links vertx and fragment shaders into a single shader
	void linkShaders();
	/*
	adds atributes to the shader
	(ONLY USE if in-shader indexes are not specified)
	*/
	void addAtribute(const std::string attributeName, short attribSize = 1);

	GLint getUniformLocation(const char *uniform);

	//enables shader program for use
	void enable();
	//disables shader program 
	void disable();

private:
	enum Shaders
	{
		VT_SHADER,
		FG_SHADER
	};

	bool compileShader(Shaders shadNum, const std::string filePath, GLuint id);
	void findAtributes();

	std::string m_vtsh,m_vtPath,m_fmPath;
	int m_attribNum = 0;
	bool m_enabled = false;
	GLuint
		m_programID = 0,
		m_vertID = 0,
		m_fragID = 0;
	static GLuint *m_programs, *m_attribs, m_num;

	Logger *m_log = new Logger("ShaderLog");
};


