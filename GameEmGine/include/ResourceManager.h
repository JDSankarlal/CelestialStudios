#pragma once
#include <GL\glew.h>
#include <map>
#include <string>
#include <vector>
#include "Texture.h"
#include "ImageLoader.h"
#include "Shader.h"
#include "Character.h"


struct CharacterCache
{
	//creates a character
	static Character& getCharacter(const char, const char*);

private:
	static std::map<std::pair<char, std::string>, Character> m_characters;
};

struct Texture2DCache
{
	//creates a 2D texture
	static Texture2D& getTexture(const char*);

private:
	static std::map<std::string, Texture2D> m_textures;
};


struct Texture3DCache
{
	//creates a 3D texture
	static Texture3D& getTexture(const char*);

private:
	static std::map<std::string, Texture3D> m_textures;
};

struct ShaderCache
{
	//creates a Shader
	static Shader* getShader(const char*, const char*);

private:
	static std::map<std::vector<std::string>, Shader*> m_shaders;
};

class ResourceManager
{
public:
	static Texture2D& getTexture2D(const char*);
	static Texture3D& getTexture3D(const char*);
	static Shader* getShader(const char*, const char*);
	static Character& getCharacter(char, const char*);

	//static GLSLCompiler& createShader(const char*, const char*);

};

