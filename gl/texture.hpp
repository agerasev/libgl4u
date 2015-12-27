#pragma once

#include <GL/glew.h>

#include <cstdio>

namespace gl {
class Texture {
public:
	enum Format {
		RGB,
		RGBA
	};
	enum Type {
		UBYTE,
		FLOAT
	};
	enum Interpolation {
		LINEAR,
		NEAREST
	};

private:
	GLuint _id = 0;
	int _bounds[3] = {0, 0, 0};
	Format _format = RGB;
	Type _type = UBYTE;
	int _dim = 2;
	
	void gl_formats(GLuint &ifmt, GLuint &fmt, GLuint &type) const {
		switch(_format) {
		case RGB:
			fmt = GL_RGB;
			break;
		case RGBA:
			fmt = GL_RGBA;
			break;
		}
		switch(_type) {
		case UBYTE:
			type = GL_UNSIGNED_BYTE;
			ifmt = fmt;
			break;
		case FLOAT:
			type = GL_FLOAT;
			ifmt = (_format == RGB) ? GL_RGB32F : GL_RGBA32F;
			break;
		}
	}
	
	static GLuint target(int dim) {
		switch(dim) {
		case 1:
			return GL_TEXTURE_1D;
		case 2:
			return GL_TEXTURE_2D;
		case 3:
			return GL_TEXTURE_3D;
		default:
			return 0;
		}
	}
	
	GLuint target() const {
		return target(_dim);
	}
	
public:
	Texture() {
		glGenTextures(1, &_id);
		
	}
	virtual ~Texture() {
		glDeleteTextures(1, &_id);
	}
	
	void bind() const {
		glBindTexture(target(), _id);
	}
	void unbind() const {
		glBindTexture(target(), 0);
	}
	static void unbind(int dim) {
		glBindTexture(target(dim), 0);
	}
	
	void loadData(int dim, const void *data, int bounds[], Format format, Type type, Interpolation inp = LINEAR) {
		_dim = dim;
		bind();
		
		for(int i = 0; i < dim; ++i)
			_bounds[i] = bounds[i];
		
		glTexParameteri(target(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		if(dim >= 2)
			glTexParameteri(target(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if(dim >= 3)
			glTexParameteri(target(), GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		
		glTexParameteri(target(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		setInterpolation(inp);
		
		_format = format;
		_type = type;
		GLuint ifmt, fmt, t;
		gl_formats(ifmt, fmt, t);
		
		if(dim == 1) {
			glTexImage1D(GL_TEXTURE_1D, 0, ifmt, bounds[0], 0, fmt, t, data);
		} else if(dim == 2) {
			glTexImage2D(GL_TEXTURE_2D, 0, ifmt, bounds[0], bounds[1], 0, fmt, t, data);
		} else if(dim == 3) {
			glTexImage3D(GL_TEXTURE_3D, 0, ifmt, bounds[0], bounds[1], bounds[2], 0, fmt, t, data);
		}
	}
	
	void setInterpolation(Interpolation inp) const {
		bind();
		switch(inp) {
		case LINEAR:
			glTexParameteri(target(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case NEAREST:
			glTexParameteri(target(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		}
	}

	GLuint id() const {
		return _id;
	}
	int dim() const {
		return _dim;
	}
	const int *bounds() const {
		return _bounds;
	}
	Type type() const {
		return _type;
	}
	Format format() const {
		return _format;
	}
};
}
