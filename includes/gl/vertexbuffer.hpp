#pragma once

#include <GL/glew.h>

#include <gl/exception.hpp>
#include <gl/type.hpp>

namespace gl {
class VertexBuffer {
public:
private:
	GLuint _id;
	long _size;
	Type _type;
public:
	VertexBuffer();
	~VertexBuffer();
	
	void bind();
	static void unbind();
	
	template <typename T>
	void loadData(const T *data, long size) throw(ErrorException) {
		GLenum error;
		bind();
		_size = size;
		_type = get_type<T>::value;
		glBufferData(GL_ARRAY_BUFFER, size*sizeof(T), data, GL_STATIC_DRAW);
		unbind();
		if((error = glGetError()) != GL_NO_ERROR)
			throw ErrorException(error);
	}
	
	void draw();
	
	GLuint id() const;
	long size() const;
	Type type();
};
}
