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

#define MAX_LIGHTS 32


COpengl::COpengl()
{
	this->AspectRatio = 0;
	this->FinalVao = 0;
	this->FinalVbo = 0;
	this->WindowH = 0;
	this->WindowW = 0;
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
	Shaders.CreateShader("Assets/Shaders/Gui.vse", true);
	Shaders.CreateShader("Assets/Shaders/Gui.fse", false);
	Shaders.CreateShaderProgram("Gui");
	Shaders.CreateShader("Assets/Shaders/Scene.vse", true);
	Shaders.CreateShader("Assets/Shaders/Scene.fse", false);
	Shaders.CreateShaderProgram("Default");
	Shaders.CreateShader("Assets/Shaders/final.vse", true);
	Shaders.CreateShader("Assets/Shaders/final.fse", false);
	Shaders.CreateShaderProgram("Final");
#else
	Shaders.CreateShader("Assets/Shaders/Gui.vs", true);
	Shaders.CreateShader("Assets/Shaders/Gui.fs", false);
	Shaders.CreateShaderProgram("Gui");
	Shaders.CreateShader("Assets/Shaders/Scene.vs", true);
	Shaders.CreateShader("Assets/Shaders/Scene.fs", false);
	Shaders.CreateShaderProgram("Default");
	Shaders.CreateShader("Assets/Shaders/final.vs", true);
	Shaders.CreateShader("Assets/Shaders/final.fs", false);
	Shaders.CreateShaderProgram("Final");
#endif // __EMSCRIPTEN__

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

	Shaders.SetCurrentShaderProgram("Default");
	Shaders.AddUniformToShaderStruct("Default", "View");
	Shaders.AddUniformToShaderStruct("Default", "Projection");
	Shaders.AddUniformToShaderStruct("Default", "Model");
	Shaders.AddUniformToShaderStruct("Default", "CameraPos");
	Shaders.AddUniformToShaderStruct("Default", "NormalMatrix");
	Shaders.AddUniformToShaderStruct("Default", "Base");
	Shaders.AddUniformToShaderStruct("Default", "Normal");
	Shaders.AddUniformToShaderStruct("Default", "Specular");
	{
		std::string LightNumber;
		std::string Uniform;
		for (int i = 0; i < MAX_LIGHTS; i++)
		{
			LightNumber = "Lights[";
			LightNumber += std::to_string(i);
			LightNumber += "]";
			Uniform = LightNumber + ".Position";
			Shaders.AddUniformToShaderStruct("Default", Uniform);
			Uniform = LightNumber + ".Rotation";
			Shaders.AddUniformToShaderStruct("Default", Uniform);
			Uniform = LightNumber + ".Ambient";
			Shaders.AddUniformToShaderStruct("Default", Uniform);
			Uniform = LightNumber + ".Diffuse";
			Shaders.AddUniformToShaderStruct("Default", Uniform);
			Uniform = LightNumber + ".Specular";
			Shaders.AddUniformToShaderStruct("Default", Uniform);
			Uniform = LightNumber + ".Color";
			Shaders.AddUniformToShaderStruct("Default", Uniform);
			Uniform = LightNumber + ".Constant";
			Shaders.AddUniformToShaderStruct("Default", Uniform);
			Uniform = LightNumber + ".Linear";
			Shaders.AddUniformToShaderStruct("Default", Uniform);
			Uniform = LightNumber + ".Quadratic";
			Shaders.AddUniformToShaderStruct("Default", Uniform);
			Uniform = LightNumber + ".CutoutDist";
			Shaders.AddUniformToShaderStruct("Default", Uniform);
			Uniform = LightNumber + ".LightType";
			Shaders.AddUniformToShaderStruct("Default", Uniform);
		}
	}
	Shaders.AddUniformToShaderStruct("Default", "Mat.Ambient");
	Shaders.AddUniformToShaderStruct("Default", "Mat.Diffuse");
	Shaders.AddUniformToShaderStruct("Default", "Mat.Specular");
	Shaders.AddUniformToShaderStruct("Default", "Mat.Shininess");

	Shaders.AddUniformToShaderStruct("Gui", "View");
	Shaders.AddUniformToShaderStruct("Gui", "Projection");
	Shaders.AddUniformToShaderStruct("Gui", "Model");
	
}

void COpengl::PreLoop()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

}

void COpengl::SetModelMatrix(glm::mat4 matrix)
{
	glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Default","Model"), 1, GL_FALSE, &matrix[0][0]);
}

void COpengl::SetModelMatrixLayout(glm::mat4 matrix)
{
	glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Gui", "Model"), 1, GL_FALSE, &matrix[0][0]);
}

void COpengl::SetNormalMatrix(glm::mat4 matrix)
{
	glm::mat4 ViewModel = this->ViewMatrix*matrix ;
	glm::mat4 NormalMatrix =glm::transpose(glm::inverse(matrix));
	glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Default", "NormalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
}

void COpengl::ProLoop(SDL_Window* Window)
{
	glUseProgram(0);
	//SDL_GL_SetSwapInterval(1);
	SDL_GL_SwapWindow(Window);
}

void COpengl::PreLoopPerspective(std::shared_ptr<CCameraComponent> Camera)
{
	if (Camera != nullptr)
	{
		ViewMatrix = glm::lookAt(Camera->GetPosition(), Camera->GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 Projection = glm::perspective(glm::radians(Camera->GetFov()), this->AspectRatio, 0.1f, 100.0f);
		glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Default", "View"), 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Default", "Projection"), 1, GL_FALSE, &Projection[0][0]);
	}


}

void COpengl::PreLoopOrtho(SDL_Window* Window)
{
	ViewMatrix = glm::lookAt(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Projection = glm::ortho(0.0f, (float)this->WindowW, (float)this->WindowH, 0.0f, -0.1f, 1000.0f);
	glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Gui", "View"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Gui", "Projection"), 1, GL_FALSE, &Projection[0][0]);


}

void COpengl::SetAspectRatio(SDL_Window * Window)
{
	int w, h;
	SDL_GetWindowSize(Window, &w, &h);
	this->WindowW = w;
	this->WindowH = h;
	this->AspectRatio = (float)w / (float)h;
}

void COpengl::AddNewFramebuffer(std::string FBName, const char* ShaderName)
{
	MyFrameBuffer FboStruct;
	FboStruct.name = FBName;
	glGenFramebuffers(1, &FboStruct.FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FboStruct.FBO);

	glGenTextures(1, &FboStruct.CBuffer);
	glBindTexture(GL_TEXTURE_2D, FboStruct.CBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->WindowW, this->WindowH, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FboStruct.CBuffer, 0);

	glGenRenderbuffers(1, &FboStruct.RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, FboStruct.RBO);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->WindowW, this->WindowH);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, this->WindowW, this->WindowH);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, FboStruct.RBO);
	FboStruct.ShaderName = ShaderName;
	this->Framebuffers.push_back(FboStruct);
}

void COpengl::UseFramebuffer(std::string name)
{
	if (name == "0")
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		Shaders.SetCurrentShaderProgram("Final");
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return;
	}
	for (auto o : this->Framebuffers)
	{
		if (name == o.name)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, o.FBO);
			Shaders.SetCurrentShaderProgram(o.ShaderName);
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
	glUniform1i(glGetUniformLocation(Shaders.GetCurrentShaderProgram(), "Base"),0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void COpengl::ProcessLight(std::vector<std::shared_ptr<CLightComponent>> lights)
{
	Light LightStruct;
	std::string LightNumber;
	std::string Uniform;
	for (int i=0;i<lights.size();i++)
	{
		LightStruct = lights[i]->GetLightStruct();
		LightNumber = "Lights[";
		LightNumber+= std::to_string(i);
		LightNumber += "]";
		Uniform = LightNumber + ".Position";
		glUniform3f(Shaders.GetUniformByNameStruct("Default", Uniform), LightStruct.Position.x, LightStruct.Position.y, LightStruct.Position.z);
		Uniform = LightNumber + ".Rotation";
		glUniform3f(Shaders.GetUniformByNameStruct("Default", Uniform), LightStruct.Rotation.x, LightStruct.Rotation.y, LightStruct.Rotation.z);
		Uniform = LightNumber + ".Ambient";
		glUniform3f(Shaders.GetUniformByNameStruct("Default", Uniform), LightStruct.Ambient.x, LightStruct.Ambient.y, LightStruct.Ambient.z);
		Uniform = LightNumber + ".Diffuse";
		glUniform3f(Shaders.GetUniformByNameStruct("Default", Uniform), LightStruct.Diffuse.x, LightStruct.Diffuse.y, LightStruct.Diffuse.z);
		Uniform = LightNumber + ".Specular";
		glUniform3f(Shaders.GetUniformByNameStruct("Default", Uniform), LightStruct.Specular.x, LightStruct.Specular.y, LightStruct.Specular.z);
		Uniform = LightNumber + ".Color";
		glUniform3f(Shaders.GetUniformByNameStruct("Default", Uniform), LightStruct.Color.x, LightStruct.Color.y, LightStruct.Color.z);
		Uniform = LightNumber + ".Constant";
		glUniform1f(Shaders.GetUniformByNameStruct("Default", Uniform), LightStruct.Constant);
		Uniform = LightNumber + ".Linear";
		glUniform1f(Shaders.GetUniformByNameStruct("Default", Uniform), LightStruct.Linear);
		Uniform = LightNumber + ".Quadratic";
		glUniform1f(Shaders.GetUniformByNameStruct("Default", Uniform), LightStruct.Quadratic);
		Uniform = LightNumber + ".CutoutDist";
		glUniform1f(Shaders.GetUniformByNameStruct("Default", Uniform), LightStruct.CutoutDist);
		Uniform = LightNumber + ".LightType";
		glUniform1i(Shaders.GetUniformByNameStruct("Default", Uniform), LightStruct.LightType);
	}
}

Shaders COpengl::GetShadersClass()
{
	return this->Shaders;
}


