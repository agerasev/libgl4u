#include <gl/texture.hpp>

using namespace gl;
	
GLuint Texture::target(int dim) {
	static const GLuint trg[3] = {
	GL_TEXTURE_1D,
	GL_TEXTURE_2D,
	GL_TEXTURE_3D
	};
	return (dim > 0 && dim < 4) ? trg[dim - 1] : 0;
}

GLuint Texture::target() const {
	return target(_dim);
}

Texture::Texture() {
	glGenTextures(1, &_id);
}

Texture::~Texture() {
	glDeleteTextures(1, &_id);
}

void Texture::bind() const {
	glBindTexture(target(), _id);
	if(!glIsTexture(_id))
		fprintf(stderr, "%d is not a texture\n", _id);
}
void Texture::unbind() const {
	glBindTexture(target(), 0);
}
void Texture::unbind(int dim) {
	glBindTexture(target(dim), 0);
}

void Texture::init(int dim, const int size[], InternalFormat ifmt, int level) throw(ErrorException) {
	_dim = dim;
	bind();
	
	for(int i = 0; i < dim; ++i)
		_size[i] = size[i];
	
	glTexParameteri(target(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	if(dim >= 2)
		glTexParameteri(target(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if(dim >= 3)
		glTexParameteri(target(), GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	_format = ifmt;
	
	if(dim == 1) {
		glTexImage1D(GL_TEXTURE_1D, level, (GLuint)ifmt, size[0], 0, (GLenum)RGBA, (GLenum)UBYTE, NULL);
	} else if(dim == 2) {
		glTexImage2D(GL_TEXTURE_2D, level, (GLuint)ifmt, size[0], size[1], 0, (GLenum)RGBA, (GLenum)UBYTE, NULL);
	} else if(dim == 3) {
		glTexImage3D(GL_TEXTURE_3D, level, (GLuint)ifmt, size[0], size[1], size[2], 0, (GLenum)RGBA, (GLenum)UBYTE, NULL);
	}
	
	if(max_lod < level) {
		max_lod = level;
		glTexParameteri(target(), GL_TEXTURE_MAX_LEVEL, max_lod);
	}
	
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		throw ErrorException(error);
}

void Texture::write(const void *data, const int offset[], const int size[], Format fmt, Type type, int level) throw(ErrorException) {
	bind();
	
	if(_dim == 1) {
		glTexSubImage1D(GL_TEXTURE_1D, level, offset[0], size[0], (GLuint)fmt, (GLuint)type, data);
	} else if(_dim == 2) {
		glTexSubImage2D(GL_TEXTURE_2D, level, offset[0], offset[1], size[0], size[1], (GLuint)fmt, (GLuint)type, data);
	} else if(_dim == 3) {
		glTexSubImage3D(GL_TEXTURE_3D, level, offset[0], offset[1], offset[2], size[0], size[1], size[2], (GLuint)fmt, (GLuint)type, data);
	}
	
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		throw ErrorException(error);
}

void Texture::setInterpolation(Interpolation min, Interpolation mag) const {
	bind();
	glTexParameteri(target(), GL_TEXTURE_MIN_FILTER, (GLuint)min);
	glTexParameteri(target(), GL_TEXTURE_MAG_FILTER, (GLuint)mag);
}

GLuint Texture::id() const {
	return _id;
}
int Texture::dim() const {
	return _dim;
}
const int *Texture::size() const {
	return _size;
}
Texture::InternalFormat Texture::internal_format() const {
	return _format;
}

