#include "Shader.h"

GLuint *Shader::m_programs = new GLuint[0], *Shader::m_attribs = new GLuint[0], Shader::m_num;

Shader::Shader():m_vtsh (""), m_vtPath ( ""), m_fmPath ( ""), m_attribNum ( 0),
m_enabled ( false), m_programID ( 0),
m_vertID ( 0),
m_fragID (0)
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
	compileShaders(vertFilePath, fragFilePath);
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

void Shader::compileShaders(const std::string & vertFilePath, const std::string & fragFilePath)
{
	m_vtPath = vertFilePath;
	m_fmPath = fragFilePath;


	if(m_programID)
		glDeleteProgram(m_programID);

	m_programID = glCreateProgram();
	m_vertID = glCreateShader(GL_VERTEX_SHADER);
	m_fragID = glCreateShader(GL_FRAGMENT_SHADER);

	if(!compileShader(VT_SHADER, vertFilePath, m_vertID))return;
	if(!compileShader(FG_SHADER, fragFilePath, m_fragID))return;
}

void Shader::linkShaders()
{
	glAttachShader(m_programID, m_vertID);
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

		m_programID = m_vertID = m_fragID = 0;

		// Use the infoLog as you see fit.
		printf(infoLog + '\n');
		// In this simple program, we'll just leave
		return;
	}
	glDetachShader(m_programID, m_vertID);
	glDetachShader(m_programID, m_fragID);
	glDeleteShader(m_vertID);
	glDeleteShader(m_fragID);
}

void Shader::addAtribute(const std::string attributeName, short attribSize)
{
	glBindAttribLocation(m_programID, m_attribNum, attributeName.c_str());
	m_attribNum += attribSize;
}

GLint Shader::getUniformLocation(const char * uniform)
{
	return glGetUniformLocation(m_programID, uniform);
}

void Shader::enable()
{
	//glUseProgram (m_programID);

	if(!m_enabled)
	{
		m_attribs = (GLuint*)realloc(m_attribs, ++m_num * sizeof(GLuint));
		m_programs = (GLuint*)realloc(m_programs, m_num * sizeof(GLuint));

		m_programs[m_num - 1] = m_programID;
		findAtributes();
		m_attribs[m_num - 1] = m_attribNum;

		glUseProgram(m_programID);
		for(int a = 0; a < m_attribNum; a++)
			glEnableVertexAttribArray(a);
		m_enabled = true;
	}
}

void Shader::disable()
{
	if(m_enabled)
	{
		for(int a = 0; a < m_attribNum; a++)
			glDisableVertexAttribArray(a);

		if(m_num - 1 > 0)
		{
			glUseProgram(m_programs[--m_num - 1]);
			for(unsigned a = 0; a < m_attribs[m_num - 1]; a++)
				glEnableVertexAttribArray(a);
			m_enabled = false;
		} else
			glUseProgram(0);

		m_enabled = !m_enabled;
	}

	//glUseProgram(0);

}

bool Shader::compileShader(Shaders shadType, const std::string filePath, GLuint id)
{
	std::ifstream
		shader(filePath);

	std::string fileContent = "", line;

	while(std::getline(shader, line))
		fileContent += line + '\n';
	shader.close();

	if(shadType == VT_SHADER) //stores vtsh
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

		printf(errorLog + '\n');

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

