#pragma once
#include <GL\glew.h>
#include <SOIL2\SOIL2.h>
#include <cstdlib>
#include "Texture.h"
#include "fileIO.h"

class ImageLoader
{
public:
	static Texture2D loadImage2D(const char* path);
	static Texture3D loadImage3D(const char* path);
private:
};

