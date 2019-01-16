#include "FrameBuffer.h"
#include <cstdlib>
#include <cstring>

FrameBuffer::FrameBuffer(unsigned numColourAttach)
{
	glGenBuffers(1, &m_fboID);

	setNumColourAttachments(numColourAttach);
}

FrameBuffer::~FrameBuffer()
{
	unload();
}

void FrameBuffer::setNumColourAttachments(unsigned num)
{
	unsigned tmp = m_numColourAttachments;
	m_numColourAttachments = num;

	m_colourAttachments = !m_colourAttachments ?
		new GLuint[m_numColourAttachments] :
		(GLuint*)std::realloc(m_colourAttachments, sizeof(GLuint)*num);

	std::memset(m_colourAttachments + tmp, 0, sizeof(GLuint)*(num - tmp > 0 ? num - tmp : 0));

	m_buffs = !m_buffs ?
		new GLenum[m_numColourAttachments] :
		(GLenum*)std::realloc(m_buffs, sizeof(GLenum)*num);

	for(unsigned a = 0; a < m_numColourAttachments; a++)
		m_buffs[a] = GL_COLOR_ATTACHMENT0 + a;
}

void FrameBuffer::initDepthTexture(int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);

	if(!m_depthAttachment)
		glGenTextures(1, &m_depthAttachment);

	glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
	glTextureStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::initColourTexture(unsigned width, unsigned height, GLint internalFormat, GLint filter, GLint wrap, unsigned index)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);

	if(!m_colourAttachments[index])
		glGenTextures(1, &m_colourAttachments[index]);

	glBindTexture(GL_TEXTURE_2D, m_colourAttachments[index]);
	glTextureStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, m_colourAttachments[index], 0);

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

bool FrameBuffer::checkFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void FrameBuffer::unload()
{
	if(m_buffs)
		delete[] m_buffs,
		m_buffs = nullptr;

	if(m_colourAttachments)
	{
		for(unsigned a = 0; a < m_numColourAttachments; a++)
			glDeleteTextures(1, &m_colourAttachments[a]);

		delete[] m_colourAttachments;
	}

	if(m_depthAttachment)
		glDeleteTextures(1, &m_depthAttachment),
		m_depthAttachment = GL_NONE;

	m_numColourAttachments = 0;
}

void FrameBuffer::clear()
{
	GLbitfield tmp = 0;

	tmp |= m_depthAttachment ? GL_DEPTH_BUFFER_BIT : 0;
	tmp |= m_colourAttachments ? GL_COLOR_BUFFER_BIT : 0;

	glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
	glClear(tmp);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::enable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
	glDrawBuffers(m_numColourAttachments, m_buffs);
}

void FrameBuffer::disable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::moveToBackBuffer(int windWidth, int windHeight)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GL_NONE);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fboID);

	glBlitFramebuffer(0, 0, windWidth, windHeight, 0, 0, windWidth, windHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

GLuint FrameBuffer::GetDepthHandle() const
{
	return m_depthAttachment;
}

GLuint FrameBuffer::GetColourHandle(unsigned index) const
{
	return m_colourAttachments[index];
}
