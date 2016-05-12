#include <gl/vertexbuffer.hpp>

using namespace gl;

VertexBuffer::VertexBuffer() {
	glGenBuffers(1, &_id);
}
VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &_id);
}

void VertexBuffer::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, _id);
}
void VertexBuffer::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint VertexBuffer::id() const {
	return _id;
}
long VertexBuffer::size() const {
	return _size;
}
Type VertexBuffer::type() {
	return _type;
}
