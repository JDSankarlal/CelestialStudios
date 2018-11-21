#pragma once
#include <GL\glew.h>
#include <map>
#include <string>
#include "Texture.h"
#include "ImageLoader.h"
#include "GLSLCompiler.h"

struct Texture2DCache
{			
	//creates a texture
	Texture2D& getTexture(const char*);

private:
	std::map<std::string, Texture2D> m_texture;
};
		
struct Texture3DCache
{		 
	//creates a texture
	Texture3D& getTexture(const char*);

private:
	std::map<std::string, Texture3D> m_texture;
};

struct ShaderCache
{
	GLSLCompiler& getShader(std::string, std::string);

	template<class T1, class T2>
	friend bool operator==(std::pair<T1, T2> p1, std::pair<T1, T2> p2)
	{
		return p1.first == p2.first &&
			p1.second == p2.second;
	}
private:
	std::map<std::pair<std::string, std::string>, GLSLCompiler> m_cashedShaders;
};


class ResourceManager
{
public:
	static Texture2D& createTexture2D(const char*);
	static Texture3D& getTexture3D(const char*);

	static GLSLCompiler& createShader(const char*, const char*);

private:
	static Texture2DCache m_textureCache2D;
	static Texture3DCache m_textureCache3D;
	static ShaderCache m_shaderCache;
};

