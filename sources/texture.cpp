#include <gl/texture.hpp>

using namespace gl;

void Texture::_gen_tex() {
	_del_tex();
	glGenTextures(1, &_id);
	own = true;
}

void Texture::_del_tex() {
	if(own) {
		glDeleteTextures(1, &_id);
		own = false;
	}
}

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
	
}

Texture::~Texture() {
	_del_tex();
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
	_del_tex();
	_gen_tex();
	
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
	
	std::pair<Format, Type> ft = split_itype(ifmt);
	if(dim == 1) {
		glTexImage1D(GL_TEXTURE_1D, level, (GLuint)ifmt, size[0], 0, (GLenum)ft.first, (GLenum)ft.second, NULL);
	} else if(dim == 2) {
		glTexImage2D(GL_TEXTURE_2D, level, (GLuint)ifmt, size[0], size[1], 0, (GLenum)ft.first, (GLenum)ft.second, NULL);
	} else if(dim == 3) {
		glTexImage3D(GL_TEXTURE_3D, level, (GLuint)ifmt, size[0], size[1], size[2], 0, (GLenum)ft.first, (GLenum)ft.second, NULL);
	}
	
	if(max_lod < level) {
		max_lod = level;
		glTexParameteri(target(), GL_TEXTURE_MAX_LEVEL, max_lod);
	}
	
	setInterpolation(Interpolation::NEAREST, Interpolation::NEAREST);
	
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

void Texture::read(void *data, Texture::Format fmt, Type type, int size, int level) throw(ErrorException) {
	bind();
	
	if(size != 0) {
		glGetnTexImage(target(), level, (GLuint)fmt, (GLuint)type, size, data);
	} else {
		glGetTexImage(target(), level, (GLuint)fmt, (GLuint)type, data);
	}
	
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		throw ErrorException(error);
}

void Texture::wrap(GLuint id, int dim, const int size[], InternalFormat ifmt) {
	_del_tex();
	
	_id = id;
	_dim = dim;
	for(int i = 0; i < dim; ++i) {
		_size[i] = size[i];
	}
	
	_format = ifmt;
}

void Texture::setInterpolation(Interpolation min, Interpolation mag) {
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

std::pair<Texture::Format, gl::Type> Texture::split_itype(InternalFormat ifmt) {
	Texture::Format fmt;
	gl::Type type;
	switch(ifmt) {
	case R8:
	case R16:
	case R16F:
	case R32F:
		fmt = RED;
		break;
		
	case R8I:
	case R8UI:
	case R16I:
	case R16UI:
	case R32I:
	case R32UI:
		fmt = RED_INT;
		break;
	
	case RG8:
	case RG16:
	case RG16F:
	case RG32F:
		fmt = RG;
		break;
		
	case RG8I:
	case RG8UI:
	case RG16I:
	case RG16UI:
	case RG32I:
	case RG32UI:
		fmt = RG_INT;
		break;
		
	case RGB8:
	case RGB16:
	case RGB16F:
	case RGB32F:
		fmt = RGB;
		break;
		
	case RGB8I:
	case RGB8UI:
	case RGB16I:
	case RGB16UI:
	case RGB32I:
	case RGB32UI:
		fmt = RGB_INT;
		break;
		
	case RGBA8:
	case RGBA16:
	case RGBA16F:
	case RGBA32F:
		fmt = RGBA;
		break;
		
	case RGBA8I:
	case RGBA8UI:
	case RGBA16I:
	case RGBA16UI:
	case RGBA32I:
	case RGBA32UI:
		fmt = RGBA_INT;
		break;
	}
	
	switch(ifmt) {
	case R8:
	case R8I:
	case R8UI:
	case RG8:
	case RG8I:
	case RG8UI:
	case RGB8:
	case RGB8I:
	case RGB8UI:
	case RGBA8:
	case RGBA8I:
	case RGBA8UI:
		type = BYTE;
		break;
		
	case R16:
	case R16I:
	case R16UI:
	case RG16:
	case RG16I:
	case RG16UI:
	case RGB16:
	case RGB16I:
	case RGB16UI:
	case RGBA16:
	case RGBA16I:
	case RGBA16UI:
		type = SHORT;
		break;
		
	case R16F:
	case R32F:
	case RG16F:
	case RG32F:
	case RGB16F:
	case RGB32F:
	case RGBA16F:
	case RGBA32F:
		type = FLOAT;
		break;
		
	case R32I:
	case R32UI:
	case RG32I:
	case RG32UI:
	case RGB32I:
	case RGB32UI:
	case RGBA32I:
	case RGBA32UI:
		type = INT;
		break;
	}
	
	return std::pair<Format, Type>(fmt, type);
}
