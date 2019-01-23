#pragma once

#include <GL/glew.h>
#include <functional>

class FrameBuffer
{
public:
	FrameBuffer() = delete;
	FrameBuffer(std::string tag,unsigned numColorAttachments);
	~FrameBuffer();

	void initDepthTexture(unsigned width, unsigned height);
	void initColourTexture(unsigned index, unsigned width, unsigned height, GLint internalFormat, GLint filter, GLint wrap);
	bool checkFBO();

	// Clears all OpenGL memory
	void unload();

	// Clears all attached textures
	void clear();

	void enable();
	void disable();

	void moveToBackBuffer(int windowWidth, int windowHeight);

	GLuint getDepthHandle() const;
	GLuint getColorHandle(unsigned index) const;

	void setPostProcess(std::function<void()>, unsigned layer=0);
	std::function<void()> getPostProcess();

	std::string getTag();
	unsigned getLayer();

private:
	GLuint
		m_layer = GL_NONE,
		m_fboID = GL_NONE, 
		m_depthAttachment = GL_NONE, 
		*m_colorAttachments = nullptr;

	GLenum *m_buffs = nullptr;

	unsigned int m_numColorAttachments = 0;

	std::string m_tag;
	std::function<void()>m_postProcess;
};