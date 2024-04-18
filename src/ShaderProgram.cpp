#include "ShaderProgram.h"
#include "Utility.h"
#include <fstream>
#include <iostream>
#include <vector>
Shader::Shader(GLenum type, const std::string& file, const std::string& prefix)
{
	m_id = glCreateShader(type);
	std::vector<char> source;
	std::ifstream f(file, std::ifstream::in | std::ifstream::binary | std::ifstream::ate);
	ERR_CHECK_BOOL(f.is_open());
	GLint size = (GLint)f.tellg();
	f.seekg(0);
	source.resize(size);
	f.read(source.data(), size);
	f.close();
	if (prefix.size()) {
		const char* allSources[] = { prefix.data(), source.data() };
		GLint allSizes[] = { (int)prefix.size(), size };
		glShaderSource(m_id, 2, allSources, allSizes);
	}
	else {
		const char* allSources[] = {source.data() };
		GLint allSizes[] = {size };
		glShaderSource(m_id, 1, allSources, allSizes);
	}
	glCompileShader(m_id);
	int status;
	char infoLog[1024];
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		glGetShaderInfoLog(m_id, 1024, NULL, infoLog);
		ERR_LOG_BOOL(0, infoLog);
	}
}

ShaderProgram::ShaderProgram()
{
	m_id = glCreateProgram();
}

ShaderProgram& ShaderProgram::SetVertexShader(const std::string& file)
{
	return SetShader(0, GL_VERTEX_SHADER, file, "");
}

ShaderProgram& ShaderProgram::SetGeometryShader(const std::string& file)
{
	return SetShader(1, GL_GEOMETRY_SHADER, file, "");
}

ShaderProgram& ShaderProgram::SetFragmentShader(const std::string& file)
{
	return SetShader(2, GL_FRAGMENT_SHADER, file, "");
}

ShaderProgram& ShaderProgram::SetVertexShader(const std::string& file, const std::string& prefix)
{
	return SetShader(0, GL_VERTEX_SHADER, file, prefix);
}

ShaderProgram& ShaderProgram::SetGeometryShader(const std::string& file, const std::string& prefix)
{
	return SetShader(1, GL_GEOMETRY_SHADER, file, prefix);
}

ShaderProgram& ShaderProgram::SetFragmentShader(const std::string& file, const std::string& prefix)
{
	return SetShader(2, GL_FRAGMENT_SHADER, file, prefix);
}

GLuint ShaderProgram::GetProgramId()
{
	return m_id;
}

void ShaderProgram::Done()
{
	glLinkProgram(m_id);
}

void ShaderProgram::Use()
{
	glUseProgram(m_id);
}

ShaderProgram& ShaderProgram::SetShader(int id, GLuint type, const std::string& file, const std::string& prefix)
{
	if (m_shaders[id]) {
		glDetachShader(m_id, m_shaders[id].value().m_id);
		glDeleteShader(m_shaders[id].value().m_id);
	}
	m_shaders[id] = Shader(type, file, prefix);
	glAttachShader(m_id, m_shaders[id].value().m_id);
	return *this;
}
