#pragma once

#include <GL/glew.h>

class FrameBuffer
{
public:
	FrameBuffer() = delete;
	FrameBuffer(unsigned numColorAttachments);
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

private:
	GLuint m_fboID = GL_NONE
	, m_depthAttachment = GL_NONE
	, *m_colorAttachments = nullptr;
	GLenum *m_buffs = nullptr;

	unsigned int m_numColorAttachments = 0;
};