#ifndef SHADER_HPP
#define SHADER_HPP

#include<GL/glew.h>
#include<iostream>
#include<fstream>
#include<string>

#include<4u/exception/glexception.hpp>
#include<4u/exception/filenotfoundexception.hpp>

class Shader
{
private:

	GLuint id;

	std::string logCompilation()
	{
		std::string message;

		int   infologLen   = 0;
		int   charsWritten = 0;
		char *infoLog;

		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infologLen);

		if(infologLen > 1)
		{
			infoLog = new char[infologLen];
			glGetShaderInfoLog(id, infologLen, &charsWritten, infoLog);
			message = infoLog;
			delete[] infoLog;
		}

		return message;
	}

public:

	Shader(GLenum type)
	{
		id = glCreateShader(type);
	}

	virtual ~Shader()
	{
		glDeleteShader(id);
	}

	void loadSourceFromFile(const char *addr) throw(FileNotFoundException)
	{

		int length;

		std::ifstream is;
		is.open(addr, std::ios::binary);
		if(!is)
		{
			throw FileNotFoundException(addr);
		}

		is.seekg (0, std::ios::end);
		length = is.tellg();
		is.seekg (0, std::ios::beg);

		char *src;
		src = new char[length+1];
		src[length] = '\0';

		is.read(src,length);
		is.close();

		glShaderSource(id, 1, const_cast<const GLchar**>(&src), nullptr);
	}

	void compile() throw(GLException)
	{
		glCompileShader(id);
		std::string log = logCompilation();

		GLint st;
		glGetShaderiv(id,GL_COMPILE_STATUS,&st);
		if(st == GL_TRUE)
		{
			/* to log file */
			std::cout << log;
			std::cout.flush();
		}
		else
		{
			throw GLException(log);
		}
	}

	GLuint getID() const
	{
		return id;
	}
};

#endif // SHADER_HPP
