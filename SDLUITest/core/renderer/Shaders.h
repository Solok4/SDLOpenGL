#pragma once

#include "../../CLog.h"
#include "GL/glew.h"
#include "../../glm/glm.hpp"
#include <vector>
#include <memory>
#include <string>
#include "IShader.h"

struct Uniform
{
	unsigned int Uni;
	std::string name;
};

struct ShadProgram
{
	GLuint Program;
	std::string name;
	std::vector< std::shared_ptr<Uniform>> Uniforms;
};

enum ShaderType
{
	Vertex = 0,
	Fragment,
	Geometry,
};

class Shaders
{
public:
	Shaders();
	~Shaders();

	void CreateShader(const char* File, ShaderType type);
	std::string ReadShaderFromFile(const char* Filename);
	void CreateShaderProgram(std::string name, bool useGeometryShader);
	void SetCurrentShaderProgram(std::string name);
	GLuint GetCurrentShaderProgram();
	std::shared_ptr<ShadProgram> GetShaderStruct(std::string name);
	GLuint GetShaderProgram(std::string name);
	void AddUniformToShaderStruct(std::string ProgramName, std::string UnifromName);
	GLuint GetUniformByNameStruct(std::string ProgramName, std::string UniformName);

	void UniformMat4f(glm::mat4 matrix, const char* uniformName, const char* Shader = "");
	void Uniform1f(float value, const char* uniformName, const char* Shader = "");
	void Uniform3f(glm::vec3 value, const char* uniformName, const char* Shader = "");
	void Uniform1i(int value, const char* uniformName, const char* Shader = "");

	GLuint GetUniformFromCurrentProgram(const char* name);

private:

	unsigned int VertexShader;
	unsigned int FragmentShader;
	unsigned int GeometryShader;
	std::vector< std::shared_ptr<ShadProgram>> ShaderProgram;
	std::shared_ptr<ShadProgram> CurrentShaderProgram;
	std::shared_ptr<IShader> shaderWrapper;
};
