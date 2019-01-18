#pragma once

#include <GL/glew.h>

class FrameBuffer
{
public:
	FrameBuffer() = delete;
	FrameBuffer(unsigned numColourAttach);
	~FrameBuffer();

	//void setNumColourAttachments(unsigned num);

	void initDepthTexture(int width, int height);
	void initColourTexture(unsigned width, unsigned height, GLint internalFormat, GLint filter, GLint wrap, unsigned index);
	bool checkFBO();

	void unload();

	void clear();

	void enable();
	void disable();

	void moveToBackBuffer(int windWidth, int windHeight);

	GLuint GetDepthHandle()const;
	GLuint GetColourHandle(unsigned index)const;

private:

	GLuint	m_fboID = GL_NONE;
	GLuint m_depthAttachment = GL_NONE;
	GLuint	*m_colourAttachments = nullptr;
	GLenum *m_buffs = nullptr;

	unsigned m_numColourAttachments;

};