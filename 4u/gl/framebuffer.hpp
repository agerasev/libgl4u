#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include<GL/glew.h>
#include<iostream>

class FrameBuffer
{
private:
	int width, height;
	GLuint id = 0;
	GLuint texture = 0;
public:
	struct Size {
		int w, h;
		Size() {
			w = 0;
			h = 0;
		}
		Size(int aw, int ah) {
			w = aw;
			h = ah;
		}
		Size(const Size &as) {
			w = as.w;
			h = as.h;
		}
	};

	FrameBuffer(int w, int h) {
		width = w;
		height = h;

		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

		// Set the list of draw buffers.
		GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

		// Always check that our framebuffer is ok
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "[ ok ] Framebuffer was succesfully created" << std::endl;
		} else {
			std::cout << "[error!] Framebuffer failed!" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
	virtual ~FrameBuffer() {
		glDeleteFramebuffers(1,&id);
	}

	void bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glViewport(0,0,width,height);
	}
	static void unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Size getSize() const {
		return Size(width,height);
	}
	GLuint getTexture() const {
		return texture;
	}
};

#endif // FRAMEBUFFER_HPP
