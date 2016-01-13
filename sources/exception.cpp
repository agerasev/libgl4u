#include <gl/exception.hpp>

using namespace gl;

Exception::Exception(const std::string &message) {
	msg = message;
}

const char *Exception::what() const noexcept {
	return msg.c_str();
}

std::string Exception::getMessage() const {
	return msg;
}

std::string ErrorException::code_to_string(GLenum code) {
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

ErrorException::ErrorException(GLenum error_code, const std::string &error_info)
  : Exception(code_to_string(error_code) + " : " + error_info), 
	code(error_code), info(error_info) 
{
	
}

GLenum ErrorException::getCode() const {
	return code;
}

std::string ErrorException::getInfo() const {
	return info;
}


FileNotFoundException::FileNotFoundException(const std::string &filename)
	: Exception("File '" + filename + "' not found")
{
	fn = filename;
}

std::string FileNotFoundException::getFileName() const {
	return fn;
}

