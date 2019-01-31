#include "Shader.h"

GLuint *Shader::m_programs = new GLuint[0], *Shader::m_attribs = new GLuint[0], Shader::m_num;

Shader::Shader() :m_vtsh(""), m_vtPath(""), m_fmPath(""), m_attribNum(0),
m_enabled(false), m_programID(0),
m_vertID(0),
m_fragID(0)
{
	glewInit();
}

Shader::Shader(Shader& shad):m_vtsh(shad.m_vtsh), m_vtPath(shad.m_vtPath), m_fmPath(shad.m_fmPath), m_attribNum(shad.m_attribNum),
m_enabled(shad.m_enabled), m_programID(shad.m_programID),
m_vertID(0),
m_fragID(0)
{
	glewInit();
}

Shader::~Shader()
{
	if(m_programID)
		glDeleteProgram(m_programID);
}

void Shader::refresh()
{
	create(m_vtPath, m_fmPath);
}

void Shader::create(const std::string & vertFilePath, const std::string & fragFilePath)
{
	if(compileShaders(vertFilePath, fragFilePath))
		linkShaders();
}

void Shader::create(const std::string & vertFilePath, const std::string & fragFilePath, const std::string & geoFilePath)
{
	if(compileShaders(vertFilePath, fragFilePath,geoFilePath))
		linkShaders();
}

void Shader::createDefault()
{
	//create("Shaders/error.vert", "Shaders/error.frag");


	if(m_programID)
		glDeleteProgram(m_programID);

	if(m_fragID)
		glDeleteShader(m_fragID);
	if(m_vertID)
		glDeleteShader(m_vertID);

	m_programID = m_vertID = m_fragID = 0;


	m_programID = glCreateProgram();
	m_vertID = glCreateShader(GL_VERTEX_SHADER);
	m_fragID = glCreateShader(GL_FRAGMENT_SHADER);


	std::string tmpFileContent =
		"#version 420\n"

		"uniform mat4 uModel;\n"
		"uniform mat4 uView;\n"
		"uniform mat4 uProj;\n"

		"layout(location = 0) in vec3 in_vert;\n"
		"layout(location = 1) in vec2 in_uv;\n"
		"layout(location = 2) in vec3 in_normal;\n"

		"out vec2 texcoord;\n"
		"out vec3 norm;\n"
		"out vec3 pos;\n"

		"void main()\n"
		"{\n"
		"texcoord = in_uv;\n"
		"norm = mat3(uView) * mat3(uModel) * in_normal;\n"

		"pos = (uView * uModel * vec4(in_vert, 1.0f)).xyz;\n"

		"gl_Position = uProj * vec4(pos, 1.0f);\n"
		"}\n"
		;

	m_vtsh = tmpFileContent;
	const char * tmp = tmpFileContent.c_str();

	glShaderSource(m_vertID, 1, &tmp, nullptr);
	glCompileShader(m_vertID);


	tmpFileContent =
		"#version 420\n"

		"in vec2 texcoord;\n"
		"in vec3 norm;\n"
		"in vec3 pos;\n"

		"out vec4 outColor;\n"

		"void main()\n"
		"{\n"
		"outColor = vec4(1, 0, 1, 1);\n"
		"}\n"
		;
	tmp = tmpFileContent.c_str();

	glShaderSource(m_fragID, 1, &tmp, nullptr);
	glCompileShader(m_fragID);

	//linkShaders ();
	//delete tmpFileContent;
}

bool Shader::compileShaders(const std::string & vertFilePath, const std::string & fragFilePath)
{
	m_vtPath = vertFilePath;
	m_fmPath = fragFilePath;
	
	glDeleteProgram(m_programID);

	m_programID = glCreateProgram();
	m_vertID = glCreateShader(GL_VERTEX_SHADER);
	m_fragID = glCreateShader(GL_FRAGMENT_SHADER);

	if(!compileShader(VERT_SHADER, vertFilePath, m_vertID))return false;
	if(!compileShader(FRAG_SHADER, fragFilePath, m_fragID))return false;

	return true;
}

bool Shader::compileShaders(const std::string & vertFilePath, const std::string & fragFilePath, const std::string & geoFilePath)
{
	m_vtPath = vertFilePath;
	m_fmPath = fragFilePath;

	glDeleteProgram(m_programID);

	m_programID = glCreateProgram();
	m_vertID = glCreateShader(GL_VERTEX_SHADER);
	m_fragID = glCreateShader(GL_FRAGMENT_SHADER);
	m_geomID = glCreateShader(GL_GEOMETRY_SHADER);

	if(!compileShader(VERT_SHADER, vertFilePath, m_vertID))return false;
	if(!compileShader(FRAG_SHADER, fragFilePath, m_fragID))return false;
	if(!compileShader(GEOM_SHADER, geoFilePath, m_geomID))return false;



	return true;
}

void Shader::linkShaders()
{
	glAttachShader(m_programID, m_vertID);
	if(m_geomID)
		glAttachShader(m_programID, m_geomID);
	glAttachShader(m_programID, m_fragID);

	glLinkProgram(m_programID);

	GLint isLinked = 0;
	glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);
	
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		char* infoLog = new char[maxLength];
		glGetProgramInfoLog(m_programID, maxLength, &maxLength, infoLog);

		// We don't need the program anymore.
		glDeleteProgram(m_programID);

		// Don't leak shaders either.
		glDeleteShader(m_vertID);
		glDeleteShader(m_fragID);

		//Reset to zero.
		m_programID = m_vertID = m_fragID = 0;

		// Use the infoLog as you see fit.
		printf(infoLog + '\n');

		system("pause");
		// In this simple program, we'll just leave
		return;
	}
	
	glDetachShader(m_programID, m_vertID);
	glDetachShader(m_programID, m_fragID);
	if(m_geomID)
		glDetachShader(m_programID, m_geomID);

	glDeleteShader(m_vertID);
	glDeleteShader(m_fragID);
	if(m_geomID)
		glDeleteShader(m_geomID);

}

void Shader::addAtribute(const std::string attributeName, short index)
{
	glBindAttribLocation(m_programID, index, attributeName.c_str());
	linkShaders();
}

GLint Shader::getAttribLocation(const std::string attributeName)
{
	return glGetAttribLocation(m_programID,attributeName.c_str());
}

GLint Shader::getUniformLocation(const char * uniform)
{
	GLuint uni = glGetUniformLocation(m_programID, uniform);
	return uni;
}

void Shader::enable()
{
	glUseProgram (m_programID);

	
}

void Shader::disable()
{
	
	glUseProgram(0);

}

bool Shader::compileShader(Shaders shadType, const std::string filePath, GLuint id)
{
	std::ifstream
		shader(filePath);

	std::string fileContent = "", line;

	while(std::getline(shader, line))
		fileContent += line + '\n';
	shader.close();

	if(shadType == VERT_SHADER) //stores vtsh
		m_vtsh = fileContent;

	const char* tmpFileContent = fileContent.c_str();
	glShaderSource(id, 1, &tmpFileContent, nullptr);
	glCompileShader(id);
	//delete tmpFileContent;

	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if(success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		char* errorLog = new char[maxLength];
		glGetShaderInfoLog(id, maxLength, &maxLength, errorLog);

		printf("error in file: %s\n",filePath.c_str());
		printf("%s\n\n",errorLog );

		createDefault();
		return false;
	}
	return true;
}

void Shader::findAtributes()
{
	unsigned short count = 0;
	for(unsigned a = 0; a < m_vtsh.size(); a++)
	{
		if(m_vtsh.substr(a, 3) == "in ")
		{
			count++;
			a += 3;
		}
	}
	m_attribNum = count;
}

