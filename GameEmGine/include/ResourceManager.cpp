#include "ResourceManager.h"

Texture2DCache ResourceManager::m_textureCache2D;
Texture3DCache ResourceManager::m_textureCache3D;
ShaderCache ResourceManager::m_shaderCache;

Texture2D& Texture2DCache::getTexture(const char * path)
{
	auto it = m_texture.find(path);

	if(it == m_texture.end())
	{
			Texture2D tmp= ImageLoader::loadImage2D(path);
			m_texture.insert({path,tmp});
			return m_texture[path];
	}
	//printf("cashed image loaded\n");
	return it->second;
}

Texture3D & Texture3DCache::getTexture(const char *path)
{
	auto it = m_texture.find(path);

	if(it == m_texture.end())
	{
		Texture3D tmp = ImageLoader::loadImage3D(path);
		m_texture.insert({path,tmp});
		return m_texture[path];
	}
	return it->second;
}

GLSLCompiler& ShaderCache::getShader(std::string vertShader, std::string fragShader)
{
	
	auto it = m_cashedShaders.find({ vertShader, fragShader });

	if(it == m_cashedShaders.end())
	{
		GLSLCompiler tmp;
		tmp.create( vertShader, vertShader );
		m_cashedShaders.insert({ { vertShader, vertShader },tmp });
		return m_cashedShaders[{ vertShader, vertShader }];
	}
	//printf("cashed image loaded\n");
	
	
	return it->second;

}


Texture2D& ResourceManager::createTexture2D(const char *path)
{
	return m_textureCache2D.getTexture(path);
}

Texture3D& ResourceManager::getTexture3D(const char *path)
{
	return m_textureCache3D.getTexture(path);
}

GLSLCompiler& ResourceManager::createShader(const char *vertShader, const char *fragShader)
{
	return m_shaderCache.getShader(vertShader,fragShader);
}

