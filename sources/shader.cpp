#include <gl/shader.hpp>

#include <regex>

#include "includer.hpp"

using namespace gl;

void Shader::free_inc() {
	if(inc != nullptr) {
		delete static_cast<includer*>(inc);
		inc = nullptr;
	}
}

Shader::Shader(Shader::Type type) {
	_type = type;
	_id = glCreateShader((GLuint)type);
}
Shader::~Shader() {
	free_inc();
	glDeleteShader(_id);
}

void Shader::loadSource(char *source, long size) {
	free_inc();
	glShaderSource(_id, 1, &source, nullptr);
	_findVariables(source, size);
}
void Shader::loadSourceFromFile(const std::string &filename, const std::string &incdir) throw(FileNotFoundException) {
	free_inc();
	includer *_inc = new includer(filename, std::list<std::string>({incdir}));
	loadSource(const_cast<char*>(_inc->data().c_str()), _inc->data().size());
	inc = static_cast<void*>(_inc);
}


/* returned array of chars must be deleted */
std::string Shader::_getCompilationLog() {
	int len = 0;
	int chars_written = 0;
	glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &len);
	std::string message("");
	if(len > 1) {
		char *data = new char[len];
		glGetShaderInfoLog(_id, len, &chars_written, data);
		message = std::string(data);
		delete[] data;
	}
	return message;
}


void Shader::compile() throw(Exception) {
	glCompileShader(_id);
	std::string msg = _getCompilationLog();
	if(msg.size() > 0) {
		if(inc != nullptr) {
			includer *_inc = static_cast<includer*>(inc);
			std::string result;
			std::string string(msg);
			// some of GLSL output formats
			std::regex expr("(^|\n)[a-zA-Z:\\(\\) ]*(\\d+)[:\\(\\) ]+(\\d+)[:\\)]*"); 
			std::smatch match;
			while(std::regex_search(string,match,expr)) {
				int gpos = std::stoi(std::string(match[3]));
				std::string name;
				int lpos;
				if(_inc->locate(gpos, name, lpos)) {
					result += match.prefix().str() + std::string(match[1]) + name + ": " + std::string(match[2]) + ":" + std::to_string(lpos);
				} else {
					fprintf(stderr, "includer: cannot locate line %d\n", gpos);
					result += match.prefix().str() + std::string(match[0]);
				}
				string = match.suffix().str();
			}
			result += string;
			msg = result;
		}
		fprintf(stderr, "Shader '%s' compile log:\n%s\n", _name.c_str(), msg.c_str());
	}
	
	GLint st;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &st);
	if(st != GL_TRUE) {
		throw Exception("Shader '" + _name + "' compile error");
	}
}

GLuint Shader::id() const {
	return _id;
}
const std::list<Shader::Variable> &Shader::attributes() const {
	return _attribs;
}
const std::list<Shader::Variable> &Shader::uniforms() const {
	return _uniforms;
}

void Shader::setName(const std::string &name) {
	_name = name;
}
std::string Shader::name() const {
	return _name;
}


void Shader::_findVariables(char *source, long) {
	std::string string;
	std::smatch match;
	std::regex expr;
	
	if(_type == VERTEX) {
		string = std::string(source);
		expr = "(^|\n)[ \t]*in[ \t\n]*([^ \t\n]*)[ \t\n]*([^ \t\n;]*)[ \t\n]*;";
		while(std::regex_search(string, match, expr))
		{
			Variable var;
			var.name = match[3];
			var.type = match[2];
			_attribs.push_back(var);
			string = match.suffix().str();
		}
	}
	
	string = std::string(source);
	expr = "(^|\n)[ \t]*uniform[ \t\n]*([^ \t\n]*)[ \t\n]*([^ \t\n;]*)[ \t\n]*;";
	while(std::regex_search(string, match, expr))
	{
		Variable var;
		var.name = match[3];
		var.type = match[2];
		_uniforms.push_back(var);
		string = match.suffix().str();
	}
}

