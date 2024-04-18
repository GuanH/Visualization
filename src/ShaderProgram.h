#pragma once
#include <glad/glad.h>
#include <string>
#include <array>
#include <optional>

struct Shader {
	Shader(GLenum type, const std::string& file, const std::string& prefix);
	GLuint m_id;
};

class ShaderProgram {
public:
	ShaderProgram();
	ShaderProgram& SetVertexShader(const std::string& file);
	ShaderProgram& SetGeometryShader(const std::string& file);
	ShaderProgram& SetFragmentShader(const std::string& file);
	ShaderProgram& SetVertexShader(const std::string& file, const std::string& prefix);
	ShaderProgram& SetGeometryShader(const std::string& file, const std::string& prefix);
	ShaderProgram& SetFragmentShader(const std::string& file, const std::string& prefix);
	GLuint GetProgramId();
	void Done();
	void Use();
private:
	ShaderProgram& SetShader(int id, GLuint type, const std::string& file, const std::string& prefix);
	GLuint m_id;
	std::array<std::optional<Shader>, 3> m_shaders;
};