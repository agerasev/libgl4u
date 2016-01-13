#include <gl/shader.hpp>

#include <gl/filereader.hpp>

#include <regex>

using namespace gl;

Shader::Shader(Shader::Type type) {
	_type = type;
	_id = glCreateShader((GLuint)type);
}
Shader::~Shader() {
	glDeleteShader(_id);
}

void Shader::loadSource(char *source, long size) {
	glShaderSource(_id, 1, &source, nullptr);
	_findVariables(source, size);
}
void Shader::loadSourceFromFile(const std::string &filename) throw(FileNotFoundException) {
	FileReader fr(filename);
	_name = filename;
	loadSource(fr.getData(), fr.getSize());
}


/* returned array of chars must be deleted */
char *Shader::_getCompilationLog() {
	int len = 0;
	int chars_written = 0;
	char *message = nullptr;
	glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &len);
	if(len > 1) {
		message = new char[len];
		glGetShaderInfoLog(_id, len, &chars_written, message);
	}
	return message;
}


void Shader::compile() throw(Exception) {
	glCompileShader(_id);
	char *msg = _getCompilationLog();
	if(msg != nullptr) {
		fprintf(stderr, "Shader '%s' compile log:\n%s\n", _name.c_str(), msg);
		delete[] msg;
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

