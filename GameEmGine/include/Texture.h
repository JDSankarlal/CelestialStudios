#pragma once
#include <GL\glew.h>
#include <string>
#include "Utilities.h"

enum class TEXTURE_TYPE
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
	TEXTURE_TYPE type;

	void deleteTexture()
	{
		if (id)
		{
			glDeleteTextures(1, &id);
			id = 0;
		}
	}

	void bindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}	  

	static void unbindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}

	bool operator==(Texture2D arg)
	{
		return id == arg.id;
	}
};

struct Texture3D
{
	GLuint id;
	int lutSize;
	Texture3D() = default;
	void deleteTexture()
	{
		if(id)
		{
			glDeleteTextures(1, &id);
			id = 0;
		}
	}

	void bindTexture()
	{
		glBindTexture(GL_TEXTURE_3D, id);
	}

	static void unbindTexture()
	{
		glBindTexture(GL_TEXTURE_3D, GL_NONE);
	}

	bool operator==(Texture3D arg)
	{
		return id == arg.id;
	}
};

