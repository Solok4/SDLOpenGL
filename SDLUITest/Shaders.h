#pragma once

#include "CLog.h"
#include "GL/glew.h"
#include <vector>

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

class Shaders
{
public:
	Shaders();
	~Shaders();

	void CreateShader(const char* File, bool IsVertex);
	std::string ReadShaderFromFile(const char* Filename);
	void CreateShaderProgram(int VertexID, int FragmentID,std::string name);
	void SetCurrentShaderProgram(std::string name);
	std::shared_ptr<ShadProgram> GetShaderStruct(std::string name);
	GLuint GetShaderProgram(std::string name);
	void AddUniformToShaderStruct(std::string ProgramName, std::string UnifromName);
	GLuint GetUniformByNameStruct(std::string ProgramName, std::string UniformName);
	


private:

	std::vector<unsigned int> VertexShader;
	std::vector<unsigned int> FragmentShader;
	std::vector< std::shared_ptr<ShadProgram>> ShaderProgram;

	std::shared_ptr<ShadProgram> CurrentShaderProgram;
};

