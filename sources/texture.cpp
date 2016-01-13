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

void Texture::loadData(int dim, const void *data, const int bounds[], InternalFormat ifmt, Format fmt, Type type, Interpolation intp) throw(ErrorException) {
	GLenum error;
	
	_dim = dim;
	bind();
	
	for(int i = 0; i < dim; ++i)
		_bounds[i] = bounds[i];
	
	glTexParameteri(target(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	if(dim >= 2)
		glTexParameteri(target(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if(dim >= 3)
		glTexParameteri(target(), GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	setInterpolation(intp);
	
	_in_format = ifmt;
	
	if(dim == 1) {
		glTexImage1D(GL_TEXTURE_1D, 0, (GLuint)ifmt, bounds[0], 0, (GLuint)fmt, (GLuint)type, data);
	} else if(dim == 2) {
		glTexImage2D(GL_TEXTURE_2D, 0, (GLuint)ifmt, bounds[0], bounds[1], 0, (GLuint)fmt, (GLuint)type, data);
	} else if(dim == 3) {
		glTexImage3D(GL_TEXTURE_3D, 0, (GLuint)ifmt, bounds[0], bounds[1], bounds[2], 0, (GLuint)fmt, (GLuint)type, data);
	}
	
	error = glGetError();
	if(error != GL_NO_ERROR)
		throw ErrorException(error);
}

void Texture::loadSubData(const void *data, const int offset[], const int sub_size[], Format fmt, Type type) throw(ErrorException) {
	GLenum error;
	bind();
	
	if(_dim == 1) {
		glTexSubImage1D(GL_TEXTURE_1D, 0, offset[0], sub_size[0], (GLuint)fmt, (GLuint)type, data);
	} else if(_dim == 2) {
		glTexSubImage2D(GL_TEXTURE_2D, 0, offset[0], offset[1], sub_size[0], sub_size[1], (GLuint)fmt, (GLuint)type, data);
	} else if(_dim == 3) {
		glTexSubImage3D(GL_TEXTURE_3D, 0, offset[0], offset[1], offset[2], sub_size[0], sub_size[1], sub_size[2], (GLuint)fmt, (GLuint)type, data);
	}
	
	error = glGetError();
	if(error != GL_NO_ERROR)
		throw ErrorException(error);
}

void Texture::setInterpolation(Interpolation intp) const {
	bind();
	glTexParameteri(target(), GL_TEXTURE_MAG_FILTER, (GLuint)intp);
	glTexParameteri(target(), GL_TEXTURE_MIN_FILTER, (GLuint)intp);
}

GLuint Texture::id() const {
	return _id;
}
int Texture::dim() const {
	return _dim;
}
const int *Texture::bounds() const {
	return _bounds;
}
Texture::InternalFormat Texture::internal_format() const {
	return _in_format;
}

