#include "pch.h"
#include "Shaders.h"
#include <fstream>
#include <exception>


Shaders::Shaders()
{
	this->VertexShader = -1;
	this->FragmentShader = -1;
}


Shaders::~Shaders()
{
}

void Shaders::CreateShader(const char * File, ShaderType type)
{
	std::string ShaderText = ReadShaderFromFile(File);
	if (!ShaderText.empty())
	{
		char logBuff[512];
		const char* ShaderAsChar = ShaderText.c_str();
		//CLog::MyLog(0, ShaderAsChar);
		GLint Result = GL_FALSE;
		if (type == ShaderType::Vertex)
		{
			GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(VertexShaderID, 1, &ShaderAsChar, NULL);
			glCompileShader(VertexShaderID);

			glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
			CLog::MyLog(LogType::Debug, "Vertex Shader compile status: %d", Result);
			glGetShaderInfoLog(VertexShaderID, 512, NULL, logBuff);
			CLog::MyLog(LogType::Debug,"Info: %s", logBuff);
			this->VertexShader = VertexShaderID;
		}
		else if(type == ShaderType::Fragment)
		{
			GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(FragmentShaderID, 1, &ShaderAsChar, NULL);
			glCompileShader(FragmentShaderID);

			glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
			CLog::MyLog(LogType::Debug, "Fragment Shader compile status: %d", Result);
			glGetShaderInfoLog(FragmentShaderID, 512, NULL, logBuff);
			CLog::MyLog(LogType::Debug, "Info: %s", logBuff);
			this->FragmentShader = FragmentShaderID;
		}
		else if (type == ShaderType::Geometry)
		{
			GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(GeometryShaderID, 1, &ShaderAsChar, NULL);
			glCompileShader(GeometryShaderID);

			glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
			CLog::MyLog(LogType::Debug, "Geometry Shader compile status: %d", Result);
			glGetShaderInfoLog(GeometryShaderID, 512, NULL, logBuff);
			CLog::MyLog(LogType::Debug, "Info: %s", logBuff);
			this->GeometryShader = GeometryShaderID;
		}
		else
		{
			CLog::MyLog(LogType::Error, "Not recognized shaderType provided with %s file", File);
			return;
		}
	}

}

std::string Shaders::ReadShaderFromFile(const char * Filename)
{
	std::fstream ShaderFileHandle;
	std::string ShaderName(Filename);
	ShaderFileHandle.open(Filename, std::ios::in);
	if (ShaderFileHandle.good() == false)
	{
		CLog::MyLog(LogType::Error, "Failed to read shader from file: %s", ShaderName.c_str());
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

void Shaders::CreateShaderProgram(std::string name,bool UseGeometryShader)
{
	if (this->VertexShader == -1 || this->FragmentShader ==-1)
	{
		if (this->VertexShader == -1)
		{
			CLog::MyLog(LogType::Error, "You need to load Vertex Shader first\n");
		}
		if (this->FragmentShader == -1)
		{
			CLog::MyLog(LogType::Error, "You need to load Fragment Shader first\n");
		}
		return;
	}
	if (UseGeometryShader)
	{
		if (this->GeometryShader == -1)
		{
			CLog::MyLog(LogType::Error, "You need to load Geometry Shader first\n");
			return;
		}
	}

	{
		ShadProgram Prog;
		Prog.name = name;
		Prog.Program = glCreateProgram();
		glAttachShader(Prog.Program, this->VertexShader);
		glAttachShader(Prog.Program, this->FragmentShader);
		if (UseGeometryShader)
		{
			glAttachShader(Prog.Program, this->GeometryShader);
		}
		glLinkProgram(Prog.Program);

		GLint result = GL_FALSE;
		glGetProgramiv(Prog.Program, GL_LINK_STATUS, &result);
		CLog::MyLog(LogType::Debug, "%s> Shader Program link status: %d",name.c_str(), result);
		glDetachShader(Prog.Program, this->VertexShader);
		glDetachShader(Prog.Program, this->FragmentShader);
		if (UseGeometryShader)
		{
			glDetachShader(Prog.Program, this->GeometryShader);
			glDeleteShader(this->GeometryShader);
			this->GeometryShader = -1;
		}

		glDeleteShader(this->VertexShader);
		glDeleteShader(this->FragmentShader);
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



void Shaders::AddUniformToShaderStruct(std::string ProgramName,std::string UnifromName)
{
	if (Shaders::GetUniformByNameStruct(ProgramName, UnifromName) == -1)
	{
		Uniform temp;
		auto Program = this->GetShaderStruct(ProgramName);
		int CurrentProgram = this->GetCurrentShaderProgram();
		glUseProgram(this->GetShaderProgram(ProgramName));
		temp.Uni = glGetUniformLocation(Program->Program, UnifromName.c_str());
		temp.name = UnifromName;
		glUseProgram(CurrentProgram);
		Program->Uniforms.push_back(std::make_shared<Uniform>(temp));
		return;
	}
	CLog::MyLog(LogType::Error, "That uniform %s already exists in %s program \n", UnifromName.c_str(), ProgramName.c_str());

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
	if (strcmp(Shader, "") == 0)
	{
		glUniformMatrix4fv(GetUniformFromCurrentProgram(uniformName), 1, GL_FALSE, &matrix[0][0]);
	}
	else
	{
		auto PastProgram = this->CurrentShaderProgram;
		this->SetCurrentShaderProgram(Shader);
		glUniformMatrix4fv(GetUniformFromCurrentProgram(uniformName), 1, GL_FALSE, &matrix[0][0]);
		this->SetCurrentShaderProgram(PastProgram->name);
	}
}

void Shaders::Uniform1f(float value, const char* uniformName, const char* Shader)
{
	if (strcmp(Shader, "") == 0)
	{
		glUniform1f(GetUniformFromCurrentProgram(uniformName),value);
	}
	else
	{
		auto PastProgram = this->CurrentShaderProgram;
		this->SetCurrentShaderProgram(Shader);
		glUniform1f(GetUniformFromCurrentProgram(uniformName), value);
		this->SetCurrentShaderProgram(PastProgram->name);
	}
}

void Shaders::Uniform3f(glm::vec3 value, const char* uniformName, const char* Shader)
{
	if (strcmp(Shader, "") == 0)
	{
		glUniform3f(GetUniformFromCurrentProgram(uniformName), value.x, value.y, value.z);
	}
	else
	{
		auto PastProgram = this->CurrentShaderProgram;
		this->SetCurrentShaderProgram(Shader);
		glUniform3f(GetUniformFromCurrentProgram(uniformName), value.x, value.y, value.z);
		this->SetCurrentShaderProgram(PastProgram->name);
	}
}

void Shaders::Uniform1i(int value, const char* uniformName, const char* Shader)
{
	if (strcmp(Shader, "") == 0)
	{
		glUniform1i(GetUniformFromCurrentProgram(uniformName), value);
	}
	else
	{
		auto PastProgram = this->CurrentShaderProgram;
		this->SetCurrentShaderProgram(Shader);
		glUniform1i(GetUniformFromCurrentProgram(uniformName), value);
		this->SetCurrentShaderProgram(PastProgram->name);
	}
}

GLuint Shaders::GetUniformFromCurrentProgram(const char* name)
{
	for (auto a : this->CurrentShaderProgram->Uniforms)
	{
		if (strcmp(a->name.c_str(), name) == 0)
		{
			return a->Uni;
		}
	}
	return -1;
}

