#include "Shaders.h"
#include <fstream>
#include <exception>
#include "GLSLShader.h"
#include "../shared/Shared.h"

Shaders::Shaders()
{
	this->VertexShader = -1;
	this->FragmentShader = -1;
	this->shaderWrapper = std::make_shared<GLSLShader>();
}

Shaders::~Shaders()
{
}

void Shaders::CreateShader(const char* File, ShaderType type)
{
	std::string ShaderText = ReadShaderFromFile(File);
	if (!ShaderText.empty())
	{
		char logBuff[512];
		const char* ShaderAsChar = ShaderText.c_str();
		GLint Result = GL_FALSE;
		if (type == ShaderType::Vertex)
		{
			GLuint VertexShaderID = shaderWrapper->createShader(GL_VERTEX_SHADER);
			shaderWrapper->shaderSource(VertexShaderID, 1, &ShaderAsChar, NULL);
			shaderWrapper->compileShader(VertexShaderID);

			shaderWrapper->getShaderIntValue(VertexShaderID, GL_COMPILE_STATUS, &Result);
			CLog::debug("Vertex Shader compile status: %d", Result);
			shaderWrapper->getShaderInfoLog(VertexShaderID, sizeof(logBuff)/sizeof(logBuff[0]), NULL, logBuff);
			CLog::debug("Info: %s", logBuff);
			this->VertexShader = VertexShaderID;
		}
		else if (type == ShaderType::Fragment)
		{
			GLuint FragmentShaderID = shaderWrapper->createShader(GL_FRAGMENT_SHADER);
			shaderWrapper->shaderSource(FragmentShaderID, 1, &ShaderAsChar, NULL);
			shaderWrapper->compileShader(FragmentShaderID);

			shaderWrapper->getShaderIntValue(FragmentShaderID, GL_COMPILE_STATUS, &Result);
			CLog::debug("Fragment Shader compile status: %d", Result);
			shaderWrapper->getShaderInfoLog(FragmentShaderID, sizeof(logBuff)/sizeof(logBuff[0]), NULL, logBuff);
			CLog::debug("Info: %s", logBuff);
			this->FragmentShader = FragmentShaderID;
		}
		else if (type == ShaderType::Geometry)
		{
			GLuint GeometryShaderID = shaderWrapper->createShader(GL_GEOMETRY_SHADER);
			shaderWrapper->shaderSource(GeometryShaderID, 1, &ShaderAsChar, NULL);
			shaderWrapper->compileShader(GeometryShaderID);

			shaderWrapper->getShaderIntValue(GeometryShaderID, GL_COMPILE_STATUS, &Result);
			CLog::debug("Geometry Shader compile status: %d", Result);
			shaderWrapper->getShaderInfoLog(GeometryShaderID, sizeof(logBuff)/sizeof(logBuff[0]), NULL, logBuff);
			CLog::debug("Info: %s", logBuff);
			this->GeometryShader = GeometryShaderID;
		}
		else
		{
			CLog::error("Not recognized shaderType provided with %s file", File);
			return;
		}
	}
}

std::string Shaders::ReadShaderFromFile(const char* Filename)
{
	std::fstream ShaderFileHandle;
	std::string ShaderName(Filename);
	ShaderFileHandle.open(Filename, std::ios::in);
	if (ShaderFileHandle.good() == false)
	{
		CLog::error("Failed to read shader from file: %s", ShaderName.c_str());
		return "";
	}

	char NextLine[256];
	std::string FileContent;
	while (!ShaderFileHandle.eof())
	{
		ShaderFileHandle.getline(NextLine, 256);
		FileContent.append(NextLine);
		FileContent.append("\n");
	}
	ShaderFileHandle.close();
	return FileContent;
}

void Shaders::CreateShaderProgram(std::string name, bool UseGeometryShader)
{
	if (this->VertexShader == -1 || this->FragmentShader == -1)
	{
		if (this->VertexShader == -1)
		{
			CLog::error("You need to load Vertex Shader first\n");
		}
		if (this->FragmentShader == -1)
		{
			CLog::error("You need to load Fragment Shader first\n");
		}
		return;
	}
	if (UseGeometryShader)
	{
		if (this->GeometryShader == -1)
		{
			CLog::error("You need to load Geometry Shader first\n");
			return;
		}
	}

	{
		ShadProgram Prog;
		Prog.name = name;
		Prog.Program = shaderWrapper->createProgram();
		shaderWrapper->attachShader(Prog.Program, this->VertexShader);
		shaderWrapper->attachShader(Prog.Program, this->FragmentShader);
		if (UseGeometryShader)
		{
			shaderWrapper->attachShader(Prog.Program, this->GeometryShader);
		}
		shaderWrapper->linkProgram(Prog.Program);

		GLint result = GL_FALSE;
		shaderWrapper->getProgramiv(Prog.Program, GL_LINK_STATUS, &result);
		CLog::debug("%s> Shader Program link status: %d", name.c_str(), result);
		shaderWrapper->detachShader(Prog.Program, this->VertexShader);
		shaderWrapper->detachShader(Prog.Program, this->FragmentShader);
		if (UseGeometryShader)
		{
			shaderWrapper->detachShader(Prog.Program, this->GeometryShader);
			shaderWrapper->deleteShader(this->GeometryShader);
			this->GeometryShader = -1;
		}

		shaderWrapper->deleteShader(this->VertexShader);
		shaderWrapper->deleteShader(this->FragmentShader);
		this->VertexShader = -1;
		this->FragmentShader = -1;
		ShaderProgram.push_back(std::make_shared<ShadProgram>(Prog));
	}
}

void Shaders::SetCurrentShaderProgram(std::string name)
{
	for (auto o : this->ShaderProgram)
	{
		if (o->name == name)
		{
			this->CurrentShaderProgram = o;
			glUseProgram(o->Program);
			return;
		}
	}
}

GLuint Shaders::GetCurrentShaderProgram()
{
	return this->CurrentShaderProgram->Program;
}

std::shared_ptr<ShadProgram> Shaders::GetShaderStruct(std::string name)
{
	for (auto o : ShaderProgram)
	{
		if (o->name == name)
		{
			return o;
		}
	}
	return nullptr;
}

GLuint Shaders::GetShaderProgram(std::string name)
{
	for (auto o : this->ShaderProgram)
	{
		if (o->name == name)
		{
			return o->Program;
		}
	}
	return -1;
}

void Shaders::AddUniformToShaderStruct(std::string ProgramName, std::string UnifromName)
{
	if (Shaders::GetUniformByNameStruct(ProgramName, UnifromName) == -1)
	{
		Uniform temp;
		auto Program = this->GetShaderStruct(ProgramName);
		int CurrentProgram = this->GetCurrentShaderProgram();
		shaderWrapper->useProgram(this->GetShaderProgram(ProgramName));
		temp.Uni = shaderWrapper->getUniformLocation(Program->Program, UnifromName.c_str());
		temp.name = UnifromName;
		shaderWrapper->useProgram(CurrentProgram);
		Program->Uniforms.push_back(std::make_shared<Uniform>(temp));
		return;
	}
	CLog::error("That uniform %s already exists in %s program \n", UnifromName.c_str(), ProgramName.c_str());
}

GLuint Shaders::GetUniformByNameStruct(std::string ProgramName, std::string UniformName)
{
	auto s = this->GetShaderStruct(ProgramName);
	for (auto o : s->Uniforms)
	{
		if (o->name == UniformName)
		{
			return o->Uni;
		}
	}
	return -1;
}

void Shaders::UniformMat4f(glm::mat4 matrix, const char* uniformName, const char* Shader)
{
	if (StdLibWrapper::Sstrcmp(Shader, "") == 0)
	{
		shaderWrapper->uniformMatrix4fv(GetUniformFromCurrentProgram(uniformName), 1, GL_FALSE, &matrix[0][0]);
	}
	else
	{
		auto PastProgram = this->CurrentShaderProgram;
		this->SetCurrentShaderProgram(Shader);
		shaderWrapper->uniformMatrix4fv(GetUniformFromCurrentProgram(uniformName), 1, GL_FALSE, &matrix[0][0]);
		this->SetCurrentShaderProgram(PastProgram->name);
	}
}

void Shaders::Uniform1f(float value, const char* uniformName, const char* Shader)
{
	if (StdLibWrapper::Sstrcmp(Shader, "") == 0)
	{
		shaderWrapper->uniform1f(GetUniformFromCurrentProgram(uniformName), value);
	}
	else
	{
		auto PastProgram = this->CurrentShaderProgram;
		this->SetCurrentShaderProgram(Shader);
		shaderWrapper->uniform1f(GetUniformFromCurrentProgram(uniformName), value);
		this->SetCurrentShaderProgram(PastProgram->name);
	}
}

void Shaders::Uniform3f(glm::vec3 value, const char* uniformName, const char* Shader)
{
	if (StdLibWrapper::Sstrcmp(Shader, "") == 0)
	{
		shaderWrapper->uniform3f(GetUniformFromCurrentProgram(uniformName), value.x, value.y, value.z);
	}
	else
	{
		auto PastProgram = this->CurrentShaderProgram;
		this->SetCurrentShaderProgram(Shader);
		shaderWrapper->uniform3f(GetUniformFromCurrentProgram(uniformName), value.x, value.y, value.z);
		this->SetCurrentShaderProgram(PastProgram->name);
	}
}

void Shaders::Uniform1i(int value, const char* uniformName, const char* Shader)
{
	if (StdLibWrapper::Sstrcmp(Shader, "") == 0)
	{
		shaderWrapper->uniform1i(GetUniformFromCurrentProgram(uniformName), value);
	}
	else
	{
		auto PastProgram = this->CurrentShaderProgram;
		this->SetCurrentShaderProgram(Shader);
		shaderWrapper->uniform1i(GetUniformFromCurrentProgram(uniformName), value);
		this->SetCurrentShaderProgram(PastProgram->name);
	}
}

GLuint Shaders::GetUniformFromCurrentProgram(const char* name)
{
	for (auto a : this->CurrentShaderProgram->Uniforms)
	{
		if (StdLibWrapper::Sstrcmp(a->name.c_str(), name) == 0)
		{
			return a->Uni;
		}
	}
	return -1;
}