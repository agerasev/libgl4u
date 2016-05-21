#include <gl/framebuffer.hpp>

using namespace gl;

FrameBuffer::FrameBuffer() throw(Exception) {
	glGenFramebuffers(1, &_id);
}
FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &_id);
}

void FrameBuffer::init(Texture::InternalFormat ifmt, int width, int height) throw(Exception) {
	int s[2] = {width, height};
	init(2, ifmt, s);
}

void FrameBuffer::init(int dim, Texture::InternalFormat ifmt, const int size[]) throw(Exception) {
	if(dim == 1) {
		_width = size[0];
		_height = 1;
	} else if(dim == 2) {
		_width = size[0];
		_height = size[1];
	}
	
	bind();
	
	_tex.init(dim, size, ifmt);
	
	if(GLEW_VERSION_3_2) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _tex.id(), 0);
	} else if(GLEW_ARB_framebuffer_object) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _tex.target(), _tex.id(), 0);
	} else {
		throw Exception("No framebuffer support : (GLEW_VERSION_3_2 || GLEW_ARB_framebuffer_object) == 0");
	}
	
	GLenum bufs[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, bufs);
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw Exception("FrameBuffer create error");
	
	unbind();
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
Texture *FrameBuffer::getTexture() {
	return &_tex;
}

