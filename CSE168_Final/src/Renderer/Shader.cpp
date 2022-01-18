#include "oglpch.h"
#include "Shader.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>


Shader::Shader(const std::string& filepath)
	:m_RendererID(0), m_FilePath(filepath)
{
	ShaderSource source = ParseShader(filepath); // Get raw shaders
	if (source.GeometrySource.length() > 0)
		m_RendererID = CreateShader(source.VertexSource, source.FragmentSource, source.GeometrySource);
	else
		m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

ShaderSource Shader::ParseShader(const std::string& filepath)
{
	// Open file stream
	std::ifstream stream(filepath);

	// Shader types
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
	};

	// Go through each line of shader file
	std::string line;
	std::stringstream ss[3];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		// Check if the shader type has changed
		if (line.find("#SHADER") != std::string::npos)
		{
			if (line.find("VERTEX") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("FRAGMENT") != std::string::npos)
				type = ShaderType::FRAGMENT;
			else if (line.find("GEOMETRY") != std::string::npos)
				type = ShaderType::GEOMETRY;
		}
		else
		{
			// Print line to current shader type
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str(), ss[2].str() };
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader)
{
	// Compile shaders
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	unsigned int gs = 0;
	if (geometryShader != "")
		gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);

	// Attach shader to program
	glAttachShader(program, vs);
	if (gs != 0)
		glAttachShader(program, gs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	// Delete the shaders now that the program is done
	glDeleteShader(vs);
	if (gs != 0)
		glDeleteShader(gs);
	glDeleteShader(fs);

	return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	// Create shader
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Compile error checking
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		// Print errors to console
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char)); // Allocate char array on stack
		glGetShaderInfoLog(id, length, &length, message);
		if (type == GL_VERTEX_SHADER)
			LOG_ERROR("Failed to compile vertex shader!");
		else if (type == GL_FRAGMENT_SHADER)
			LOG_ERROR("Failed to compile fragment shader!");
		else if (type == GL_GEOMETRY_SHADER)
			LOG_ERROR("Failed to compile geometry shader!");
		LOG_ERROR(message);
		glDeleteShader(id);
		return 0;
	}

	return id;
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniformInt(const std::string& name, int val)
{
	glUniform1i(GetUniformLocation(name), val);
}

void Shader::SetUniformFloat(const std::string& name, float val)
{
	glUniform1f(GetUniformLocation(name), val);
}

void Shader::SetUniformFloat3(const std::string& name, const glm::vec3& vec, unsigned int size)
{
	glUniform3fv(GetUniformLocation(name), size, glm::value_ptr(vec));
}

void Shader::SetUniformFloat4(const std::string& name, const glm::vec4& vec)
{
	glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(vec));
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::GetUniformLocation(const std::string& name)
{
	// Check if location was already found
	if (m_UniformLocations.find(name) != m_UniformLocations.end())
		return m_UniformLocations[name];

	// Find and store the location of the uniform
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
		LOG_WARN("Uniform '{0}' does not exist!", name);

	m_UniformLocations[name] = location;
	return location;
}
