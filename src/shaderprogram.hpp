#include <string>
#include <vector>
#include <iostream>

#include <GLFW/glfw3.h>
#include <glad/glad.h>


class ShaderProgram
{
public:
	ShaderProgram() = default;

	void addShaders(std::string vertSrc, std::string fragSrc);
	void createAndLinkShaderProgram();
	void bind();
	void unbind();

	int getProgramId() const { return _programId; }

private:
	class Shader
	{
	public:
		Shader(GLenum shaderType, const std::string& sourceString)
			: _shaderType{ shaderType }
		{
			const char* shaderSrc[] = { sourceString.c_str() };

			_shaderId = glCreateShader(_shaderType);
			glShaderSource(_shaderId, 1, shaderSrc, nullptr);

			glCompileShader(_shaderId);

			int  success;
			char infoLog[512];
			glGetShaderiv(_shaderId, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(_shaderId, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}

		int id() const { return _shaderId; }
	private:
		const GLenum _shaderType;
		int _shaderId = 0;
	};

	int _programId = 0;
	std::vector<Shader> _shaders;
};
