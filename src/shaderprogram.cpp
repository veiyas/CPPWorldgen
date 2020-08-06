#include "shaderprogram.hpp"

void ShaderProgram::addShaders(std::string vertSrc, std::string fragSrc)
{
	_shaders.emplace_back(GL_VERTEX_SHADER, std::move(vertSrc));
	_shaders.emplace_back(GL_FRAGMENT_SHADER, std::move(fragSrc));
}

void ShaderProgram::createAndLinkShaderProgram()
{
	_programId = glCreateProgram();

	for (const Shader& shader : _shaders)
	{
		glAttachShader(_programId, shader.id());
	}

	glLinkProgram(_programId);

	int  success;
	char infoLog[512];
	glGetProgramiv(_programId, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(_programId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
}

void ShaderProgram::bind()
{
	glUseProgram(_programId);
}

void ShaderProgram::unbind()
{
	glUseProgram(0);
}
