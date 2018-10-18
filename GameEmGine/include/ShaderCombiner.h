#pragma once
#include <string>
#include "GLSLCompiler.h"

class ShaderCombiner
{
public:
	ShaderCombiner();
	~ShaderCombiner();

	void combine(std::string base, std::string addition, std::string dir);

	std::string getCombinedShaderLocation();

private:
	std::string m_shaderLocal;
};

