#pragma once

#include "CLog.h"
#include "GL/glew.h"
#include <vector>
#include <memory>

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
	


private:

	unsigned int VertexShader;
	unsigned int FragmentShader;
	unsigned int GeometryShader;
	std::vector< std::shared_ptr<ShadProgram>> ShaderProgram;

	std::shared_ptr<ShadProgram> CurrentShaderProgram;
};

