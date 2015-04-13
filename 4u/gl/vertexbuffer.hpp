#ifndef VERETXBUFFER_HPP
#define VERETXBUFFER_HPP

#include<GL/glew.h>

class VertexBuffer
{
private:
	GLuint id;
	GLuint length;
	GLuint typesize;
public:
	VertexBuffer() {
		length = 0;
		glGenBuffers(1, &id);
	}
	virtual ~VertexBuffer() {
		unbind();
		glDeleteBuffers(1, &id);
	}
	void bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}
	void unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	template <typename T>
	void buffer(T *varr, GLuint vlen)
	{
		bind();
		length = vlen;
		typesize = sizeof(T);
		glBufferData(GL_ARRAY_BUFFER, vlen*typesize, varr, GL_STATIC_DRAW);
		unbind();
	}
	GLuint getSize() const
	{
		return length*typesize;
	}
	GLuint getLength() const
	{
		return length;
	}
};

#endif // VERETXBUFFER_HPP
