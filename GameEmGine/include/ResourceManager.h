#pragma once
#include <GL\glew.h>
#include <map>
#include <string>
#include "Texture.h"
#include "ImageLoader.h"

struct Texture2DCache
{			
	//creates a texture
	Texture2D& getTexture(const char*);

private:
	std::map<std::string, Texture2D> _texture;
};
		
struct Texture3DCache
{		 
	//creates a texture
	Texture3D& getTexture(const char*);

private:
	std::map<std::string, Texture3D> _texture;
};

class ResourceManager
{
public:
	static Texture2D getTexture2D(const char*);
	static Texture3D getTexture3D(const char*);

private:
	static Texture2DCache _cache2D;
	static Texture3DCache _cache3D;
};

