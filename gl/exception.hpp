#pragma once

#include <string>
#include <exception>

#include <GL/glew.h>

namespace gl {
class Exception : public std::exception {
private:
	std::string msg;
	
public:
	Exception(const std::string &message) {
		msg = message;
	}
	virtual ~Exception() = default;
	virtual const char *what() const noexcept {
		return msg.c_str();
	}
	std::string getMessage() const {
		return msg;
	}
};

class ErrorException : public Exception {
private:
	GLenum code;
	std::string info;
public:
	static std::string code_to_string(GLenum code) {
		switch(code) {
		case GL_NO_ERROR:
			return "GL_NO_ERROR";
		case GL_INVALID_ENUM:
			return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE:
			return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:
			return "GL_INVALID_OPERATION";
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "GL_INVALID_FRAMEBUFFER_OPERATION";
		case GL_OUT_OF_MEMORY:
			return "GL_OUT_OF_MEMORY";
		default:
			return "UNKNOWN_ERROR";
		}
	}
	ErrorException(GLenum error_code, const std::string &error_info = "")
	  : Exception(code_to_string(error_code) + " : " + error_info), 
	    code(error_code), info(error_info) {}
	GLenum getCode() const {
		return code;
	}
	std::string getInfo() const {
		return info;
	}
};

class FileNotFoundException : public Exception {
private:
	std::string fn;
public:
	FileNotFoundException(const std::string &filename)
	    : Exception("File '" + filename + "' not found")
	{
		fn = filename;
	}
	std::string getFileName() const {
		return fn;
	}
};
}
