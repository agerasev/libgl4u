#pragma once

#include <GL/glew.h>

#include <gl/exception.hpp>

namespace gl {
class Texture {
public:
	enum Format {
		RED  = GL_RED,
		RG   = GL_RG,
		RGB  = GL_RGB,
		RGBA = GL_RGBA
	};
	
	enum Type {
		BYTE   = GL_BYTE,
		UBYTE  = GL_UNSIGNED_BYTE,
		SHORT  = GL_SHORT,
		USHORT = GL_UNSIGNED_SHORT,
		INT    = GL_INT,
		UINT   = GL_UNSIGNED_INT,
		FLOAT  = GL_FLOAT
	};
	
	enum InternalFormat {
		R8    = GL_R8,
		R8I   = GL_R8I,
		R8UI  = GL_R8UI,
		R16   = GL_R16,
		R16F  = GL_R16F,
		R16I  = GL_R16I,
		R16UI = GL_R16UI,
		R32F  = GL_R32F,
		R32I  = GL_R32I,
		R32UI = GL_R32UI,
		
		RG8    = GL_RG8,
		RG8I   = GL_RG8I,
		RG8UI  = GL_RG8UI,
		RG16   = GL_RG16,
		RG16F  = GL_RG16F,
		RG16I  = GL_RG16I,
		RG16UI = GL_RG16UI,
		RG32F  = GL_RG32F,
		RG32I  = GL_RG32I,
		RG32UI = GL_RG32UI,
		
		RGB8    = GL_RGB8,
		RGB8I   = GL_RGB8I,
		RGB8UI  = GL_RGB8UI,
		RGB16   = GL_RGB16,
		RGB16F  = GL_RGB16F,
		RGB16I  = GL_RGB16I,
		RGB16UI = GL_RGB16UI,
		RGB32F  = GL_RGB32F,
		RGB32I  = GL_RGB32I,
		RGB32UI = GL_RGB32UI,
		
		RGBA8    = GL_RGBA8,
		RGBA8I   = GL_RGBA8I,
		RGBA8UI  = GL_RGBA8UI,
		RGBA16   = GL_RGBA16,
		RGBA16F  = GL_RGBA16F,
		RGBA16I  = GL_RGBA16I,
		RGBA16UI = GL_RGBA16UI,
		RGBA32F  = GL_RGBA32F,
		RGBA32I  = GL_RGBA32I,
		RGBA32UI = GL_RGBA32UI
	};
	
	enum Interpolation {
		LINEAR  = GL_LINEAR,
		NEAREST = GL_NEAREST
	};

private:
	GLuint _id = 0;
	int _bounds[3] = {0, 0, 0};
	InternalFormat _in_format = RGB8;
	int _dim = 2;
	
	static GLuint target(int dim);
	GLuint target() const;
	
public:
	Texture();
	~Texture();
	
	void bind() const;
	void unbind() const;
	static void unbind(int dim);
	
	void loadData(int dim, const void *data, const int bounds[], InternalFormat ifmt, Format fmt, Type type, Interpolation intp = LINEAR) throw(ErrorException);
	void loadSubData(const void *data, const int offset[], const int sub_size[], Format fmt, Type type) throw(ErrorException);
	
	void setInterpolation(Interpolation intp) const;

	GLuint id() const;
	int dim() const;
	const int *bounds() const;
	Texture::InternalFormat internal_format() const;
};
}
