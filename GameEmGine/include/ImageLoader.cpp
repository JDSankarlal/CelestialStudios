#include "ImageLoader.h"
Logger ImageLoader::_imageLoader = ("imageLoader");
Texture2D ImageLoader::loadImage2D(const char * path)
{
	Texture2D texture;
	int width, height;
	
	unsigned char *image = SOIL_load_image(path, &width, &height, nullptr, SOIL_LOAD_RGBA);
	
	if(image == nullptr)
	{
		_imageLoader.writeLog("Image returned with null pointer\n");
		return texture;
	}

	//Bind texture to model
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	texture.width = width;
	texture.height = height;
	return texture;
}

Texture3D ImageLoader::loadImage3D(const char * path)
{
	Texture3D texture;
	int width=0, height=0,depth=0;
	unsigned char *image=nullptr;

	//Load 3D texture information


	//Bind texture to model
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_3D, texture.id);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, width, height,depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_3D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_3D, 0);

	texture.width = width;
	texture.height = height;
	texture.depth = depth;

	return texture;
}
