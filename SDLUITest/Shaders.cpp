#include "pch.h"
#include "Shaders.h"
#include <fstream>


Shaders::Shaders()
{
	this->VertexShader = -1;
	this->FragmentShader = -1;
}


Shaders::~Shaders()
{
}

void Shaders::CreateShader(const char * File, bool IsVertex)
{
	std::string ShaderText = ReadShaderFromFile(File);
	if (!ShaderText.empty())
	{
		char logBuff[512];
		const char* ShaderAsChar = ShaderText.c_str();
		//CLog::MyLog(0, ShaderAsChar);
		GLint Result = GL_FALSE;
		if (IsVertex)
		{
			GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(VertexShaderID, 1, &ShaderAsChar, NULL);
			glCompileShader(VertexShaderID);

			glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
			CLog::MyLog(0, "Vertex Shader compile status: %d", Result);
			glGetShaderInfoLog(VertexShaderID, 512, NULL, logBuff);
			CLog::MyLog(0, logBuff);
			this->VertexShader = VertexShaderID;
		}
		else
		{
			GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(FragmentShaderID, 1, &ShaderAsChar, NULL);
			glCompileShader(FragmentShaderID);

			glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
			CLog::MyLog(0, "Fragment Shader compile status: %d", Result);
			glGetShaderInfoLog(FragmentShaderID, 512, NULL, logBuff);
			CLog::MyLog(0, "%s", logBuff);
			this->FragmentShader = FragmentShaderID;
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
		CLog::MyLog(1, "Failed to read shader from file: %s", ShaderName.c_str());
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

void Shaders::CreateShaderProgram(std::string name)
{
	if (this->VertexShader == -1 || this->FragmentShader ==-1)
	{
		if (this->VertexShader == -1)
		{
			CLog::MyLog(1, "You need to load Vertex Shader first\n");
		}
		if (this->FragmentShader == -1)
		{
			CLog::MyLog(1, "You need to load Fragment Shader first\n");
		}
		return;
	}
	else
	{
		ShadProgram Prog;
		Prog.name = name;
		Prog.Program = glCreateProgram();
		glAttachShader(Prog.Program, this->VertexShader);
		glAttachShader(Prog.Program, this->FragmentShader);
		glLinkProgram(Prog.Program);

		GLint result = GL_FALSE;
		glGetProgramiv(Prog.Program, GL_LINK_STATUS, &result);
		CLog::MyLog(0, "Shader Program link status: %d", result);
		glDetachShader(Prog.Program, this->VertexShader);
		glDetachShader(Prog.Program, this->FragmentShader);

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
	CLog::MyLog(1, "That uniform %s already exists in %s program \n", UnifromName.c_str(), ProgramName.c_str());

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

