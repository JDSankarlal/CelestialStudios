#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(std::string tag,unsigned numColorAttachments)
{
	m_tag = tag;
	glGenFramebuffers(1, &m_fboID);
	m_numColorAttachments = numColorAttachments;

	m_colorAttachments = new GLuint[m_numColorAttachments];

	//Buffs is required as a parameter for glDrawBuffers()
	m_buffs = new GLenum[m_numColorAttachments];
	for(unsigned i = 0; i < m_numColorAttachments; i++)
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
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);

	//create depth texture
	glGenTextures(1, &m_depthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Bind texture to the fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

}
void FrameBuffer::initColourTexture(unsigned index, unsigned width, unsigned height, GLint internalFormat, GLint filter, GLint wrap)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);

	//create depth texture
	glGenTextures(1, &m_colorAttachments[index]);
	glBindTexture(GL_TEXTURE_2D, m_colorAttachments[index]);
	glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	//Bind texture to the fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, m_colorAttachments[index], 0);

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}
bool FrameBuffer::checkFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		unload();
		return false;
	}

	return true;
}

// Clears all OpenGL memory
void FrameBuffer::unload()
{
	if(m_buffs != nullptr)
	{
		delete[] m_buffs;
		m_buffs = nullptr;
	}

	if(m_colorAttachments != nullptr)
	{
		for(unsigned i = 0; i < m_numColorAttachments; i++)
		{
			glDeleteTextures(1, &m_colorAttachments[i]);
		}

		delete[] m_colorAttachments;
		m_colorAttachments = nullptr;
	}

	if(m_depthAttachment != GL_NONE)
	{
		glDeleteTextures(1, &m_depthAttachment);
		m_depthAttachment = GL_NONE;
	}

	m_numColorAttachments = 0;
}

// Clears all attached textures
void FrameBuffer::clear()
{
	GLbitfield temp = 0;

	if(m_depthAttachment != GL_NONE)
	{
		temp = temp | GL_DEPTH_BUFFER_BIT;
	}

	if(m_colorAttachments != nullptr)
	{
		temp = temp | GL_COLOR_BUFFER_BIT;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
	glClear(temp);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::enable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
	glDrawBuffers(m_numColorAttachments, m_buffs);
}

void FrameBuffer::disable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::moveToBackBuffer(int windowWidth, int windowHeight)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);

	glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

GLuint FrameBuffer::getDepthHandle() const
{
	return m_depthAttachment;
}
GLuint FrameBuffer::getColorHandle(unsigned index) const
{
	return m_colorAttachments[index];
}

void FrameBuffer::setPostProcess(std::function<void()>post, unsigned layer)
{
	m_postProcess =post;
	m_layer = layer;
}

std::function<void()> FrameBuffer::getPostProcess()
{
	return m_postProcess;
}

std::string FrameBuffer::getTag()
{
	return m_tag;
}

unsigned FrameBuffer::getLayer()
{
	return m_layer;
}
