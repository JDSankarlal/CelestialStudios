#pragma once
#include <GL\glew.h>
#include <string>
#include "StructInfo.h"

 enum class TextureType
{
	NONE,
	DIFFUSE,
	SPECULAR
};
struct Texture2D
{
	GLuint id;
	ColourRGBA colour;
	int width, height;
	TextureType type;

	bool operator==(Texture2D arg)
	{
		return id == arg.id;
	}
};

struct Texture3D
{
	GLuint id;
	int width, height, depth;
};

