#pragma once

// TODO: Put glm somewhere
#include <glm/glm.hpp>

// Stores raw shaders
struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
	std::string GeometrySource;
};

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniformInt(const std::string& name, int val);
	void SetUniformFloat(const std::string& name, float val);
	void SetUniformFloat3(const std::string& name, const glm::vec3& vec, unsigned int size = 1);
	void SetUniformFloat4(const std::string& name, const glm::vec4& vector);
	void SetUniformMat4(const std::string& name, const glm::mat4& matrix);

private:
	// Create shaders from source files
	ShaderSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader = "");

	// Get the location of a uniform in a shader
	int GetUniformLocation(const std::string& name);

private:
	unsigned int m_RendererID; // Shader ID
	std::string m_FilePath; // File path for debugging purposes

	std::unordered_map<std::string, int> m_UniformLocations; // Store uniform locations previously looked up


};