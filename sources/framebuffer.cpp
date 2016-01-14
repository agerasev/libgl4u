#include <gl/framebuffer.hpp>

using namespace gl;

FrameBuffer::FrameBuffer() throw(Exception) {
	glGenFramebuffers(1, &_id);
}
FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &_id);
}

void FrameBuffer::setSize(int width, int height) throw(Exception) {
	_width = width;
	_height = height;
	
	bind();
	
	int s[2] = {width, height};
	_tex.init(2, s, Texture::RGBA32F);
	_tex.setInterpolation(Texture::LINEAR);
	
	if(GLEW_VERSION_3_2)
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _tex.id(), 0);
	else if(GLEW_ARB_framebuffer_object)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tex.id(), 0);
	else
		throw Exception("No framebuffer support : (GLEW_VERSION_3_2 || GLEW_ARB_framebuffer_object) == 0");
	
	GLenum bufs[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, bufs);
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw Exception("FrameBuffer create error");
}

void FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	glViewport(0, 0, _width, _height);
}
void FrameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FrameBuffer::id() const {
	return _id;
}
const Texture *FrameBuffer::getTexture() const {
	return &_tex;
}

