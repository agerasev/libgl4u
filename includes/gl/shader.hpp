#pragma once

#include <list>
#include <string>

#include <GL/glew.h>
#include <gl/exception.hpp>

namespace gl {
class Shader {
public:
	enum Type {
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER
	};
	struct Variable {
		std::string type;
		std::string name;
	};
	
private:
	GLuint _id = 0;
	Type _type;
	std::string _name = "";
	std::list<Variable> _attribs, _uniforms;
	
	void *inc = nullptr;
	void free_inc();
	
public:
	Shader(Shader::Type type);
	~Shader();
	
	void loadSource(char *source, long size);
	void loadSourceFromFile(const std::string &filename, const std::string &incdir="") throw(FileNotFoundException);
	
private:
	std::string _getCompilationLog();
	
public:
	void compile() throw(Exception);
	
	GLuint id() const;
	const std::list<Shader::Variable> &attributes() const;
	const std::list<Shader::Variable> &uniforms() const;
	
	void setName(const std::string &name);
	std::string name() const;
	
private:
	void _findVariables(char *source, long);
};
}
