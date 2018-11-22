#pragma once
#include <GL\glew.h>
#include <map>
#include <string>
#include <vector>
#include "Texture.h"
#include "ImageLoader.h"
#include "GLSLCompiler.h"

struct Texture2DCache
{			
	//creates a texture
	Texture2D& getTexture(const char*);

private:
	std::map<std::string, Texture2D> m_textures;
};
		
struct Texture3DCache
{		 
	//creates a texture
	Texture3D& getTexture(const char*);

private:
	std::map<std::string, Texture3D> m_textures;
};

struct ShaderCache
{
	//creates a texture
	GLSLCompiler& getShader(const char*, const char*);

private:
	std::map<std::vector<std::string>, GLSLCompiler> m_shaders;
};

class ResourceManager
{
public:
	static Texture2D getTexture2D(const char*);
	static Texture3D getTexture3D(const char*);
	static GLSLCompiler getShader(const char*, const char*);

	//static GLSLCompiler& createShader(const char*, const char*);

private:
	static Texture2DCache m_texter2DCache;
	static Texture3DCache m_texture3DCache;
	static ShaderCache m_shaderCache;
};

