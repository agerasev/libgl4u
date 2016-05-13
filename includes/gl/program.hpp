#pragma once

#include <list>
#include <map>
#include <string>

#include <gl/type.hpp>
#include <gl/exception.hpp>
#include <gl/shader.hpp>
#include <gl/texture.hpp>
#include <gl/vertexbuffer.hpp>

namespace gl {
class Program {
public:
	struct Variable {
		enum Kind {
			SCALAR,
			VECTOR,
			MATRIX,
			SAMPLER
		};
		Kind kind;
		Type type;
		int dim = 1;
		GLint id = -2;
	};
	struct AttribVariable : public Variable {
		VertexBuffer *buffer = nullptr;
	};
	struct UniformVariable : public Variable {
		union {
			float   fdata[16];
			int     idata[16];
		};
		const Texture *tex = nullptr;
		int texno = 0;
	};
private:
	GLuint _id;
	std::string _name;
	std::list<Shader*> _shaders;
	std::map<std::string, AttribVariable> _attribs;
	std::map<std::string, UniformVariable> _uniforms;
public:
	Program();
	~Program();
	
	void attach(Shader *s) throw(Exception);
	void detach(Shader *s) throw(Exception);
	
	void link() throw(Exception);

	void enable();
	static void disable();
	
	GLuint id() const;

	void setName(const std::string &name);
	std::string name() const;
	
private:
	void _parseType(const Shader::Variable &src_var, Variable &dst_var);
	void _updateVariables() throw(Exception);
	void _getVariablesID() throw(Exception);

public:
	const std::map<std::string, Program::AttribVariable> &attributes() const;
	const std::map<std::string, Program::UniformVariable> &uniforms() const;
	
private:
	void _loadUniform(const UniformVariable &var);
	
public:
	void evaluate(GLenum mode, int first, int count) throw(ErrorException);
	
	void setAttribute(const std::string &name, VertexBuffer *buf);
	
	template <typename T>
	void setUniform(const std::string &name, const T *data, long len) throw(Exception) {
		auto iter = _uniforms.find(name);
		if(iter == _uniforms.end())
			throw Exception("No such uniform '" + name + "'");
		UniformVariable &var = iter->second;
		if(var.id == -1)
			return;//throw Exception("Uniform '" + name + "' location error");
		switch(var.kind) {
		case Variable::SCALAR:
			if(len != 1)
				throw Exception("Uniform '" + name + "' is scalar, but buffer size is " + std::to_string(len));
			break;
		case Variable::VECTOR:
			if(var.dim != len)
				throw Exception("Uniform '" + name + "' vector size mismatch");
			break;
		case Variable::MATRIX:
			if(var.dim*var.dim != len)
				throw Exception("Uniform '" + name + "' matrix size mismatch");
			break;
		case Variable::SAMPLER:
			throw Exception("Uniform '" + name + "' is sampler");
			break;
		}
		if(var.type != get_type<T>::value)
			throw Exception("Uniform '" + name + "' type mismatch");
		switch(var.type) {
		case INT:
			memcpy(var.idata, data, sizeof(int)*len);
			break;
		case FLOAT:
			memcpy(var.fdata, data, sizeof(float)*len);
			break;
		default:
			break;
		}
	}
	
	template <typename T>
	typename std::enable_if<std::is_arithmetic<T>::value, void>::type setUniform(const std::string &name, T data) {
		setUniform(name, &data, 1);
	}
	void setUniform(const std::string &name, const Texture *tex);
};
}
