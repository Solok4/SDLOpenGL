#include "pch.h"
#include "GL/glew.h"
#include "glm.hpp"
#include <GL/GL.h>
#include <GL/GLU.h>
#include "CLog.h"
#include "COpengl.h"
#include <fstream>


COpengl::COpengl()
{
}


COpengl::~COpengl()
{
	Delete();
}

bool COpengl::Create(SDL_Window* Window)
{
	_Context = SDL_GL_CreateContext(Window);
	if (_Context == NULL)
	{
		CLog::MyLog(1, "Failed to create Context");
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		CLog::MyLog(1, "Failed to init GLEW");
		return false;
	}
	return true;
}

void COpengl::Delete()
{
	if (_Context !=NULL)
	{
		SDL_GL_DeleteContext(_Context);
	}
}

void COpengl::CreateShader(const char * File, bool IsVertex)
{
	std::string ShaderText = ReadShaderFromFile(File);
	if (!ShaderText.empty())
	{
		char logBuff[512];
		const char* ShaderAsChar = ShaderText.c_str();
		CLog::MyLog(0, ShaderAsChar);
		GLint Result = GL_FALSE;
		if (IsVertex)
		{
			GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(VertexShaderID, 1, &ShaderAsChar, NULL);
			glCompileShader(VertexShaderID);

			glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
			CLog::MyLog(0, "Vertex Shader compile status: " + std::to_string((int)Result));
			glGetShaderInfoLog(VertexShaderID, 512, NULL, logBuff);
			CLog::MyLog(0, logBuff);
			VertexShaders.push_back(VertexShaderID);
		}
		else
		{
			GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(FragmentShaderID, 1, &ShaderAsChar, NULL);
			glCompileShader(FragmentShaderID);

			glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
			CLog::MyLog(0, "Fragment Shader compile status: " + std::to_string((int)Result));
			glGetShaderInfoLog(FragmentShaderID, 512, NULL, logBuff);
			CLog::MyLog(0, logBuff);
			FragmentShaders.push_back(FragmentShaderID);
		}
	}

}

std::string COpengl::ReadShaderFromFile(const char *Filename)
{
	std::fstream ShaderFileHandle;
	std::string ShaderName(Filename);
	ShaderFileHandle.open(Filename, std::ios::in);
	if(ShaderFileHandle.good() == false)
	{
		CLog::MyLog(1, "Failed to read shader from file: " + ShaderName);
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

void COpengl::CreateShaderProgram(int VertexID, int FragmentID)
{
	if (VertexShaders.empty() && FragmentShaders.empty())
	{
	}
	else if(VertexShaders.size() < VertexID || FragmentShaders.size() < FragmentID)
	{
	}
	else
	{
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaders[VertexID]);
		glAttachShader(ProgramID, FragmentShaders[FragmentID]);
		glLinkProgram(ProgramID);
		
		GLint result = GL_FALSE;
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
		CLog::MyLog(0, "Shader Program link status: " + std::to_string(result));
		glDetachShader(ProgramID, VertexShaders[VertexID]);
		glDetachShader(ProgramID, FragmentShaders[FragmentID]);

		glDeleteShader(FragmentShaders[FragmentID]);
		glDeleteShader(VertexShaders[VertexID]);
		ShaderProgram.push_back(ProgramID);
	}
}

void COpengl::PrepareToLoop()
{
	CreateShader("Shaders/vs.vs", true);
	CreateShader("Shaders/fs.fs", false);
	CreateShaderProgram(0, 0);
	glClearColor(0, 0, 0, 1);

}

void COpengl::PreLoop()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SelectShaderProgram(0);
	glUseProgram(CurrentShaderProgram);
	
}

void COpengl::SetModelMatrix(glm::mat4 matrix)
{
	GLuint ModelID = glGetUniformLocation(CurrentShaderProgram, "Model");
	glUniformMatrix4fv(ModelID, 1, GL_FALSE, &matrix[0][0]);
}

void COpengl::ProLoop(SDL_Window* Window)
{
	glUseProgram(0);
	SDL_GL_SetSwapInterval(1);
	SDL_GL_SwapWindow(Window);
}

void COpengl::PreLoopPerspective()
{
	ViewMatrix = glm::lookAt(glm::vec3(0.f,0.f,1.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	GLuint ViewID = glGetUniformLocation(CurrentShaderProgram, "View");
	glUniformMatrix4fv(ViewID, 1, GL_FALSE, &ViewMatrix[0][0]);
	GLuint ProjectionID = glGetUniformLocation(CurrentShaderProgram, "Projection");
	glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, &Projection[0][0]);
	Projection = glm::perspective(glm::radians(80.0f), 16.0f / 9.0f, 0.1f, 100.0f);


}

void COpengl::PreLoopOrtho(SDL_Window* Window)
{
	ViewMatrix = glm::lookAt(glm::vec3(0.f, 0.f, -50.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	GLuint ViewID = glGetUniformLocation(CurrentShaderProgram, "View");
	glUniformMatrix4fv(ViewID, 1, GL_FALSE, &ViewMatrix[0][0]);
	int w = 0;
	int h = 0;
	SDL_GL_GetDrawableSize(Window, &w, &h);
	GLuint ProjectionID = glGetUniformLocation(CurrentShaderProgram, "Projection");
	glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, &Projection[0][0]);
	Projection = glm::ortho(0.0f, (float)w, (float)h, 0.0f, -0.1f, 1000.0f);

}

void COpengl::SetCameraPosition(glm::vec3 Pos)
{
	glm::mat4 ViewMatrix = glm::lookAt(Pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	GLuint ViewID = glGetUniformLocation(CurrentShaderProgram, "View");
	glUniformMatrix4fv(ViewID, 1, GL_FALSE, &ViewMatrix[0][0]);
}


void COpengl::SelectShaderProgram(int number)
{
	if (!ShaderProgram.empty()) 
	{
		CurrentShaderProgram = ShaderProgram[number];

	}
}


