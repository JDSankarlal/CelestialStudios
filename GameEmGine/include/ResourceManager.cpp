#include "ResourceManager.h"

std::map<std::string, Texture2D> Texture2DCache::m_textures;
std::map<std::string, Texture3D> Texture3DCache::m_textures;

//Texture3DCache ResourceManager::m_texture3DCache;
std::map<std::vector<std::string>, Shader*>  ShaderCache::m_shaders;

Texture2D& Texture2DCache::getTexture(const char* path)
{
	auto it = m_textures.find(path);

	if(it == m_textures.end())
	{
		Texture2D tmp = ImageLoader::loadImage2D(path);
		m_textures.insert({path,tmp});
		return m_textures[path];
	}
	//printf("cashed image loaded\n");
	return it->second;
}

Texture3D& Texture3DCache::getTexture(const char* path)
{
	auto it = m_textures.find(path);

	if(it == m_textures.end())
	{
		Texture3D tmp = ImageLoader::loadImage3D(path);
		m_textures.insert({path,tmp});
		return m_textures[path];
	}
	return it->second;
}

Shader* ShaderCache::getShader(const char* vtsh, const char* fmsh)
{
	auto it = m_shaders.find(std::vector<std::string>{vtsh, fmsh});

	if(it == m_shaders.end())
	{
		Shader* tmp = new Shader;
		tmp->create(vtsh, fmsh);
		m_shaders[{ (std::string)vtsh, (std::string)fmsh }] = tmp;

		return m_shaders[{ (std::string)vtsh, (std::string)fmsh }];
	}

	return it->second;
}

Texture2D ResourceManager::getTexture2D(const char* path)
{
	return Texture2DCache::getTexture(path);
}


Texture3D ResourceManager::getTexture3D(const char* path)
{

	return Texture3DCache::getTexture(path);
}

Shader* ResourceManager::getShader(const char* vtsh, const char* fmsh)
{

	return ShaderCache::getShader(vtsh, fmsh);
}

