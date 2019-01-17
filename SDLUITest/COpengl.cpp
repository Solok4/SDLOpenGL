#include "pch.h"
#include "GL/glew.h"
#include "glm/glm.hpp"
#ifndef __EMSCRIPTEN__
#include <GL/GL.h>
#include <GL/GLU.h>
#endif // !__EMSCRIPTEN__
#include "CLog.h"
#include "COpengl.h"
#include <fstream>


COpengl::COpengl()
{
}


COpengl::~COpengl()
{
	CLog::MyLog(0, "OpenglDestructor");
}

bool COpengl::Create(SDL_Window* Window)
{
#ifdef __EMSCRIPTEN__
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetSwapInterval(0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
#else
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif // __EMSCRIPTEN__


	_Context = SDL_GL_CreateContext(Window);
	if (_Context == NULL)
	{
		CLog::MyLog(1, "Failed to create Context");
		return false;
	}

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		CLog::MyLog(1, "Failed to init GLEW");
		return false;
	}
	this->SetAspectRatio(Window);
	return true;
}

void COpengl::Delete()
{
	this->ClearFramebuffers();
	if (_Context !=NULL)
	{
		SDL_GL_DeleteContext(_Context);
	}
}


void COpengl::PrepareToLoop()
{
#ifdef __EMSCRIPTEN__
	Shaders.CreateShader("Assets/Shaders/vs.vse", true);
	Shaders.CreateShader("Assets/Shaders/fs.fse", false);
	Shaders.CreateShader("Assets/Shaders/final.vse", true);
	Shaders.CreateShader("Assets/Shaders/final.fse", false);
#else
	Shaders.CreateShader("Assets/Shaders/vs.vs", true);
	Shaders.CreateShader("Assets/Shaders/fs.fs", false);
	Shaders.CreateShader("Assets/Shaders/final.vs", true);
	Shaders.CreateShader("Assets/shaders/final.fs", false);
#endif // __EMSCRIPTEN__
	Shaders.CreateShaderProgram(0, 0,"Default");
	Shaders.CreateShaderProgram(1, 1, "Final");
	glClearColor(0, 0, 0, 1);

	// Panel vertices
	GLfloat panelVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f
	};

	glGenVertexArrays(1, &this->FinalVao);
	glBindVertexArray(this->FinalVao);
	glGenBuffers(1, &this->FinalVbo);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->FinalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(panelVertices), panelVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void COpengl::PreLoop()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shaders.SetCurrentShaderProgram("Default");
	Shaders.AddUniformToShaderStruct("Default","View");
	Shaders.AddUniformToShaderStruct("Default","Projection");
	Shaders.AddUniformToShaderStruct("Default","Model");

	Shaders.AddUniformToShaderStruct("Final", "Tex");
	

}

void COpengl::SetModelMatrix(glm::mat4 matrix)
{
	glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Default","Model"), 1, GL_FALSE, &matrix[0][0]);
}

void COpengl::ProLoop(SDL_Window* Window)
{
	glUseProgram(0);
	SDL_GL_SetSwapInterval(1);
	SDL_GL_SwapWindow(Window);
}

void COpengl::PreLoopPerspective(std::shared_ptr<CCameraComponent> Camera)
{
	glm::mat4 ViewMatrix = glm::lookAt(Camera->GetPosition(), Camera->GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Projection = glm::perspective(glm::radians(Camera->GetFov()), this->AspectRatio, 0.1f, 100.0f);
	glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Default", "View"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Default", "Projection"), 1, GL_FALSE, &Projection[0][0]);


}

void COpengl::PreLoopOrtho(SDL_Window* Window)
{
	glm::mat4 ViewMatrix = glm::lookAt(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Projection = glm::ortho(0.0f, (float)this->WindowW, (float)this->WindowH, 0.0f, -0.1f, 1000.0f);
	glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Default", "View"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Default", "Projection"), 1, GL_FALSE, &Projection[0][0]);


}

void COpengl::SetAspectRatio(SDL_Window * Window)
{
	int w, h;
	SDL_GetWindowSize(Window, &w, &h);
	this->WindowW = w;
	this->WindowH = h;
	this->AspectRatio = w / h;
}

void COpengl::AddNewFramebuffer(std::string name)
{
	MyFrameBuffer FboStruct;
	FboStruct.name = name;
	glGenFramebuffers(1, &FboStruct.FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FboStruct.FBO);

	glGenTextures(1, &FboStruct.CBuffer);
	glBindTexture(GL_TEXTURE_2D, FboStruct.CBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->WindowW, this->WindowH, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FboStruct.CBuffer, 0);

	glGenRenderbuffers(1, &FboStruct.RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, FboStruct.RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->WindowW, this->WindowH);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, FboStruct.RBO);
	this->Framebuffers.push_back(FboStruct);
}

void COpengl::UseFramebuffer(std::string name)
{
	if (name == "0")
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return;
	}
	for (auto o : this->Framebuffers)
	{
		if (name == o.name)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, o.FBO);
			glClearColor(0.f, 0.f, 0.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			return;
		}
	}
	CLog::MyLog(1, "Framebuffer named %s not found", name.c_str());
}

MyFrameBuffer COpengl::GetFramebuffer(std::string name)
{
	for (auto o : this->Framebuffers)
	{
		if (o.name == name)
			return o;
	}
	CLog::MyLog(1, "Framebuffer named %s not found", name.c_str());
	return {};
}

void COpengl::ClearFramebuffers()
{
	for (auto o : this->Framebuffers)
	{
		glDeleteRenderbuffers(1,&o.RBO);
		glDeleteTextures(1, &o.CBuffer);
		glDeleteFramebuffers(1,&o.FBO);
	}
}

void COpengl::FinalDraw()
{
	glBindVertexArray(this->FinalVao);
	Shaders.SetCurrentShaderProgram("Final");
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->GetFramebuffer("Default").CBuffer);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->GetFramebuffer("GUI").CBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}


