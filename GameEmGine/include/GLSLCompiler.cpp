#include "GLSLCompiler.h"

GLuint *GLSLCompiler::m_programs = new GLuint[0], *GLSLCompiler::m_attribs = new GLuint[0], GLSLCompiler::m_num;

GLSLCompiler::GLSLCompiler()
{}

GLSLCompiler::~GLSLCompiler()
{}

void GLSLCompiler::create(const std::string & vertFilePath, const std::string & fragFilePath)
{
	compileShaders(vertFilePath, fragFilePath);
	linkShaders();
}

void GLSLCompiler::compileShaders(const std::string & vertFilePath, const std::string & fragFilePath)
{
	glewInit();
	m_programID = glCreateProgram();
	m_vertID = glCreateShader(GL_VERTEX_SHADER);
	compileShader(VT_SHADER, vertFilePath, m_vertID);

	m_fragID = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(FG_SHADER, fragFilePath, m_fragID);
}

void GLSLCompiler::linkShaders()
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

		// Use the infoLog as you see fit.
		m_log->writeLog(infoLog + '\n');
		// In this simple program, we'll just leave
		return;
	}
	glDetachShader(m_programID, m_vertID);
	glDetachShader(m_programID, m_fragID);
	glDeleteShader(m_vertID);
	glDeleteShader(m_fragID);
}

void GLSLCompiler::addAtribute(const std::string attributeName, short attribSize)
{
	glBindAttribLocation(m_programID, m_attribNum, attributeName.c_str());
	m_attribNum += attribSize;
}

GLint GLSLCompiler::getUniformLocation(const char * uniform)
{
	return glGetUniformLocation(m_programID, uniform);
}

void GLSLCompiler::enable()
{
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
		m_enabled = !m_enabled;
	}
}

void GLSLCompiler::disable()
{
	if(m_enabled)
	{
		for(int a = 0; a < m_attribNum; a++)
			glDisableVertexAttribArray(a);

		if(m_num - 1 > 0)
		{
			glUseProgram(m_programs[--m_num - 1]);
			for(int a = 0; a < m_attribs[m_num - 1]; a++)
				glEnableVertexAttribArray(a);
			m_enabled = false;
		} else
			glUseProgram(0);

		m_enabled = !m_enabled;
	}
}

void GLSLCompiler::compileShader(Shaders shadNum, const std::string filePath, GLuint id)
{
	std::ifstream
		shader(filePath);

	std::string fileContent = "", line;

	while(std::getline(shader, line))
		fileContent += line + '\n';
	shader.close();

	if(shadNum == VT_SHADER) //stores vtsh
		m_vtsh = fileContent;

	const char* tmpFileContent = fileContent.c_str();
	glShaderSource(id, 1, &tmpFileContent, nullptr);
	glCompileShader(id);

	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if(success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		char* errorLog = new char[maxLength];
		glGetShaderInfoLog(id, maxLength, &maxLength, errorLog);

		m_log->writeLog(errorLog + '\n');
		glDeleteShader(id);

	}
}

void GLSLCompiler::findAtributes()
{
	unsigned short count = 0;
	for(int a = 0; a < m_vtsh.size(); a++)
	{
		if(m_vtsh.substr(a, 3) == "in ")
		{
			count++;
			a += 3;
		}
	}
	m_attribNum = count;
}

