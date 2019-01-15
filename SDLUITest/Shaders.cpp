#include "pch.h"
#include "Shaders.h"
#include <fstream>


Shaders::Shaders()
{
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
			VertexShader.push_back(VertexShaderID);
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
			FragmentShader.push_back(FragmentShaderID);
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

void Shaders::CreateShaderProgram(int VertexID, int FragmentID, std::string name)
{
	if (VertexShader.empty() && FragmentShader.empty())
	{
	}
	else if (VertexShader.size() < VertexID || FragmentShader.size() < FragmentID)
	{
	}
	else
	{
		ShadProgram Prog;
		Prog.name = name;
		Prog.Program = glCreateProgram();
		glAttachShader(Prog.Program, VertexShader[VertexID]);
		glAttachShader(Prog.Program, FragmentShader[FragmentID]);
		glLinkProgram(Prog.Program);

		GLint result = GL_FALSE;
		glGetProgramiv(Prog.Program, GL_LINK_STATUS, &result);
		CLog::MyLog(0, "Shader Program link status: %d", result);
		glDetachShader(Prog.Program, VertexShader[VertexID]);
		glDetachShader(Prog.Program, FragmentShader[FragmentID]);

		glDeleteShader(FragmentShader[FragmentID]);
		glDeleteShader(VertexShader[VertexID]);
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
			for (auto a : o->Uniforms)
			{
				a->Uni = glGetUniformLocation(o->Program, a->name.c_str());
			}
		}
	}
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
	return NULL;
}



void Shaders::AddUniformToShaderStruct(std::string ProgramName,std::string UnifromName)
{
	Uniform temp;
	auto Program = this->GetShaderStruct(ProgramName);
	temp.Uni = glGetUniformLocation(Program->Program, UnifromName.c_str());
	temp.name = UnifromName;
	Program->Uniforms.push_back(std::make_shared<Uniform>(temp));

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
	CLog::MyLog(1, "Can't find uniform named %s in % program", UniformName.c_str(), ProgramName.c_str());
	return NULL;
}

