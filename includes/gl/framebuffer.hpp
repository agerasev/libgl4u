#pragma once

#include <GL/glew.h>

#include <gl/texture.hpp>
#include <gl/exception.hpp>

namespace gl {
class FrameBuffer {
private:
	GLuint _id;
	Texture _tex;
	int _width, _height;
	
public:
	FrameBuffer() throw(Exception);
	virtual ~FrameBuffer();
	
	void setSize(int width, int height) throw(Exception);
	
	void bind();
	static void unbind();
	
	GLuint id() const;
	const Texture *getTexture() const;
};
}
