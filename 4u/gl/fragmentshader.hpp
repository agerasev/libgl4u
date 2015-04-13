#ifndef FRAGMENTSHADER_HPP
#define FRAGMENTSHADER_HPP

#include"shader.hpp"

class FragmentShader : public Shader
{
public:
	FragmentShader() :
		Shader(GL_FRAGMENT_SHADER)
	{

	}

	virtual ~FragmentShader()
	{

	}
};

#endif // FRAGMENTSHADER_HPP
