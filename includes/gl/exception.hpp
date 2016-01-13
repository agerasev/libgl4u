#pragma once

#include <string>
#include <exception>

#include <GL/glew.h>

namespace gl {
class Exception : public std::exception {
private:
	std::string msg;
	
public:
	Exception(const std::string &message);
	virtual ~Exception() = default;
	virtual const char *what() const noexcept;
	std::string getMessage() const;
};

class ErrorException : public Exception {
private:
	GLenum code;
	std::string info;
public:
	static std::string code_to_string(GLenum code);
	ErrorException(GLenum error_code, const std::string &error_info = "");
	GLenum getCode() const;
	std::string getInfo() const;
};

class FileNotFoundException : public Exception {
private:
	std::string fn;
public:
	FileNotFoundException(const std::string &filename);
	std::string getFileName() const;
};
}
