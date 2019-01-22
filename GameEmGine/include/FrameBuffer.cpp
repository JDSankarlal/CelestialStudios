#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(unsigned numColorAttachments)
{
	m_numColorAttachments = numColorAttachments;

	glGenFramebuffers(1, &_FBO);

	_ColorAttachments = new GLuint[m_numColorAttachments];

	//Buffs is required as a parameter for glDrawBuffers()
	m_buffs = new GLenum[m_numColorAttachments];
	for (unsigned i = 0; i < m_numColorAttachments; i++)
	{
		m_buffs[i] = GL_COLOR_ATTACHMENT0 + i;
	}

}

FrameBuffer::~FrameBuffer()
{
	unload();
}

void FrameBuffer::initDepthTexture(unsigned width, unsigned height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	//create depth texture
	glGenTextures(1, &_DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, _DepthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Bind texture to the fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _DepthAttachment, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

}
void FrameBuffer::initColourTexture(unsigned index, unsigned width, unsigned height, GLint internalFormat, GLint filter, GLint wrap)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	
	//create depth texture
	glGenTextures(1, &_ColorAttachments[index]);
	glBindTexture(GL_TEXTURE_2D, _ColorAttachments[index]);
	glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	//Bind texture to the fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, _ColorAttachments[index], 0);

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}
bool FrameBuffer::checkFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		unload();
		return false;
	}

	return true;
}

// Clears all OpenGL memory
void FrameBuffer::unload()
{
	if (m_buffs != nullptr)
	{
		delete[] m_buffs;
		m_buffs = nullptr;
	}

	if (_ColorAttachments != nullptr)
	{
		for (unsigned i = 0; i < m_numColorAttachments; i++)
		{
			glDeleteTextures(1, &_ColorAttachments[i]);
		}

		delete[] _ColorAttachments;
		_ColorAttachments = nullptr;
	}

	if (_DepthAttachment != GL_NONE)
	{
		glDeleteTextures(1, &_DepthAttachment);
		_DepthAttachment = GL_NONE;
	}

	m_numColorAttachments = 0;
}

// Clears all attached textures
void FrameBuffer::clear()
{
	GLbitfield temp = 0;

	if (_DepthAttachment != GL_NONE)
	{
		temp = temp | GL_DEPTH_BUFFER_BIT;
	}

	if (_ColorAttachments != nullptr)
	{
		temp = temp | GL_COLOR_BUFFER_BIT;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glClear(temp);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::enable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glDrawBuffers(m_numColorAttachments, m_buffs);
}

void FrameBuffer::disable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::moveToBackBuffer(int windowWidth, int windowHeight)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);

	glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

GLuint FrameBuffer::getDepthHandle() const
{
	return _DepthAttachment;
}
GLuint FrameBuffer::getColorHandle(unsigned index) const
{
	return _ColorAttachments[index];
}