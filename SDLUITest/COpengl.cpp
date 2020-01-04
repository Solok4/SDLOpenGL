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
#include "CSceneManager.h"


std::unique_ptr<COpengl> OpenGL;

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
	CLog::MyLog(LogType::Log, "OpenglDestructor");
}

/*
Creates context and initalize glew
*/
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
		CLog::MyLog(LogType::Error, "Failed to create Context");
		return false;
	}

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		CLog::MyLog(LogType::Error, "Failed to init GLEW");
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

/*
Creates shaders and uniforms. Also creates quad for final drawing.
*/
void COpengl::PrepareToLoop()
{
#ifdef __EMSCRIPTEN__
	Shaders.CreateShader("Assets/Shaders/Guie.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/Guie.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("Gui",false);
	Shaders.CreateShader("Assets/Shaders/Scenee.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/Scenee.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("Default",false);
	Shaders.CreateShader("Assets/Shaders/finale.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/finale.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("Final",false);
	Shaders.CreateShader("Assets/Shaders/Shadowse.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/Shadowse.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("Shadows", false);
#else
	Shaders.CreateShader("Assets/Shaders/Gui.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/Gui.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("Gui",false);
	Shaders.CreateShader("Assets/Shaders/Scene.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/Scene.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("Default", false);
	Shaders.CreateShader("Assets/Shaders/final.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/final.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("Final", false);
	Shaders.CreateShader("Assets/Shaders/Shadows.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/Shadows.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("Shadows", false);
	Shaders.CreateShader("Assets/Shaders/PointShad.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/PointShad.geom", ShaderType::Geometry);
	Shaders.CreateShader("Assets/Shaders/PointShad.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("PointShad",true);
#endif // __EMSCRIPTEN__

	glClearColor(0, 0, 0, 1);

	// Panel vertices
	GLfloat panelVertices[] = {
		-1.0f,  -1.0f,  0.0f, 0.0f,
		 1.0f,  -1.0f,  1.0f, 0.0f,
		 1.0f, 1.0f,  1.0f, 1.0f,

		 1.0f, 1.0f,  1.0f, 1.0f,
		-1.0f, 1.0f,  0.0f, 1.0f,
		-1.0f,  -1.0f,  0.0f, 0.0f
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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shaders.SetCurrentShaderProgram("Default");
	Shaders.AddUniformToShaderStruct("Default", "View");
	Shaders.AddUniformToShaderStruct("Default", "Projection");
	Shaders.AddUniformToShaderStruct("Default", "Model");
	Shaders.AddUniformToShaderStruct("Default", "CameraPos");
	Shaders.AddUniformToShaderStruct("Default", "NormalMatrix");

	Shaders.AddUniformToShaderStruct("Default", "Base");
	Shaders.AddUniformToShaderStruct("Default", "Normal");
	Shaders.AddUniformToShaderStruct("Default", "Specular");

	Shaders.AddUniformToShaderStruct("Default", "ShadowMap");
#ifndef __EMSCRIPTEN__
	Shaders.AddUniformToShaderStruct("Default", "ShadowCube");
#endif
	Shaders.AddUniformToShaderStruct("Default", "LightCount");
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
			Uniform = "depthMVP[" + std::to_string(i);
			Uniform += "]";
			Shaders.AddUniformToShaderStruct("Default", Uniform);
		}
	}
	Shaders.AddUniformToShaderStruct("Default", "Mat.Ambient");
	Shaders.AddUniformToShaderStruct("Default", "Mat.Diffuse");
	Shaders.AddUniformToShaderStruct("Default", "Mat.Specular");
	Shaders.AddUniformToShaderStruct("Default", "Mat.Shininess");
	Shaders.AddUniformToShaderStruct("Default", "FarPlane");

	Shaders.AddUniformToShaderStruct("Gui", "View");
	Shaders.AddUniformToShaderStruct("Gui", "Projection");
	Shaders.AddUniformToShaderStruct("Gui", "Model");
	Shaders.AddUniformToShaderStruct("Gui", "ColorMask");

	Shaders.AddUniformToShaderStruct("Shadows", "depthMVP");
	Shaders.AddUniformToShaderStruct("Shadows", "Model");

#ifndef __EMSCRIPTEN__
	Shaders.AddUniformToShaderStruct("PointShad", "Model");
	Shaders.AddUniformToShaderStruct("PointShad", "LightPos");
	Shaders.AddUniformToShaderStruct("PointShad", "FarPlane");
	for (int i = 0; i < 6; i++)
	{
		Shaders.AddUniformToShaderStruct("PointShad", "ShadowMatrices[" + std::to_string(i) + "]");
	}
#endif
	
}


/*
Clear Buffers
*/
void COpengl::PreLoop()
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void COpengl::SetModelMatrix(glm::mat4 matrix)
{
	glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Default","Model"), 1, GL_FALSE, &matrix[0][0]);
	glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Shadows", "Model"), 1, GL_FALSE, &matrix[0][0]);
#ifndef __EMSCRIPTEN__
	glUniformMatrix4fv(Shaders.GetUniformByNameStruct("PointShad", "Model"), 1, GL_FALSE, &matrix[0][0]);
#endif
}

/*
Set position, scale and rotation to gui element
*/
void COpengl::SetModelMatrixLayout(glm::mat4 matrix)
{
	glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Gui", "Model"), 1, GL_FALSE, &matrix[0][0]);
}

/*
Sets colour of gui element
*/
void COpengl::SetColorMaskLayout(glm::vec3 ColorMask)
{
	glUniform3f(Shaders.GetUniformByNameStruct("Gui", "ColorMask"), ColorMask.x, ColorMask.y, ColorMask.z);
}

/*
Calculates inversed and transposed normal matrix to base drawing struct
*/
void COpengl::SetNormalMatrix(glm::mat4 matrix)
{
	glm::mat4 NormalMatrix = glm::transpose(glm::inverse(matrix));
	glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Default", "NormalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
}


/*
Use default program and swap buffers
*/
void COpengl::ProLoop(SDL_Window* Window)
{
	glUseProgram(0);
	//SDL_GL_SetSwapInterval(1);
	SDL_GL_SwapWindow(Window);
}

/*
Sets viewport based on camera settings
*/
void COpengl::PreLoopPerspective(std::shared_ptr<CCameraComponent> Camera)
{
	if (Camera != nullptr)
	{
		ViewMatrix = glm::lookAt(Camera->GetPosition(), Camera->GetPosition()+ Camera->GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 Projection = glm::perspective(glm::radians(Camera->GetFov()), this->AspectRatio, 0.1f, 100.0f);
		glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Default", "View"), 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Default", "Projection"), 1, GL_FALSE, &Projection[0][0]);
		glUniform1f(Shaders.GetUniformByNameStruct("Default", "FarPlane"), FARPLANE);
		glCullFace(GL_BACK);
	}
	else
	{
		CLog::MyLog(LogType::Warning, "Passed camera doesn't exist");
	}

}

/*
Sets viewport for gui elements
*/
void COpengl::PreLoopOrtho()
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

/*
Creates new framebuffer for drawing. Creates framebuffer and renderbuffer
*/
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
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FboStruct.CBuffer, 0);

	glGenRenderbuffers(1, &FboStruct.RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, FboStruct.RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->WindowW, this->WindowH);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, this->WindowW, this->WindowH);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, FboStruct.RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		CLog::MyLog(LogType::Error, "Framebuffer %s is not complete", FBName.c_str());
	}
	FboStruct.ShaderName = ShaderName;
	this->Framebuffers.push_back(FboStruct);
}

/*
Creates framebuffer that are used for lighting. Those saves only depth from the scene.
*/
void COpengl::AddNewLightFramebuffer(std::shared_ptr<CLightComponent> light,int size)
{
	for (auto x : this->LightFramebuffers)
	{
		if (strcmp(light->GetName().c_str(), x.name.c_str()) == 0)
		{
			return;
		}
	}
	if (size < 0)
	{
		CLog::MyLog(LogType::Error, "Light Framebuffer has to be bigger than 0 %s", light->GetName().c_str());
		return;
	}
	else if (size % 2 != 0)
	{
		CLog::MyLog(LogType::Warning, "It would be better if light framebuffer will be divisible by 2 %s", light->GetName().c_str());
	}
	MyLightFramebuffer FboStruct;
	FboStruct.name = light->GetName();
	glGenFramebuffers(1, &FboStruct.FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FboStruct.FBO);

	glGenTextures(1, &FboStruct.DepthBuff);
	if (light->GetLightStruct().LightType == LightType::Point)
	{
#ifndef __EMSCRIPTEN__
		glBindTexture(GL_TEXTURE_CUBE_MAP, FboStruct.DepthBuff);
		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, FboStruct.DepthBuff, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
#endif
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, FboStruct.DepthBuff);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, FboStruct.DepthBuff, 0);
#ifndef __EMSCRIPTEN__
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
#endif

	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLuint result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result != GL_FRAMEBUFFER_COMPLETE)
	{
		do {
			result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		
			CLog::MyLog(LogType::Error, "Light Framebuffer isn't complete %s :%d", light->GetName().c_str(),glGetError());

		} while (result != GL_FRAMEBUFFER_COMPLETE);
	}
	this->LightFramebuffers.push_back(FboStruct);

}

/*
Binds light framebuffer to be drawn to
*/
void COpengl::UseLightFramebuffer(std::string name)
{
	if (name == "")
	{
		CLog::MyLog(LogType::Error, "LightFramebuffer name can't be empty");
		return;
	}
	else
	{
		for (auto o : this->LightFramebuffers)
		{
			if (strcmp(o.name.c_str(), name.c_str()) == 0)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, o.FBO);
				glClearColor(0.f, 0.f, 0.f, 1.0f);
				glClear(GL_DEPTH_BUFFER_BIT);
				glCullFace(GL_FRONT);
				return;
			}
		}
	}
}

/*
Returns LightFramebuffer object
*/
MyLightFramebuffer COpengl::GetLightFrameBuffer(std::string name)
{
	for (auto o : this->LightFramebuffers)
	{
		if (strcmp(o.name.c_str(), name.c_str()) == 0)
		{
			return o;
		}
	}
	CLog::MyLog(LogType::Error, "LightFramebuffer named %s not found", name.c_str());
	return {};
}

/*
Binds normal framebuffer for drawing. If name == "0" default framebuffer will be bound
*/
void COpengl::UseFramebuffer(std::string name)
{
	if (name == "0")
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		Shaders.SetCurrentShaderProgram("Final");
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_BACK);
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
			glCullFace(GL_BACK);
			return;
		}
	}
	CLog::MyLog(LogType::Error, "Framebuffer named %s not found", name.c_str());
}

/*
Returns normal framebuffer object
*/
MyFrameBuffer COpengl::GetFramebuffer(std::string name)
{
	for (auto o : this->Framebuffers)
	{
		if (o.name == name)
			return o;
	}
	CLog::MyLog(LogType::Error, "Framebuffer named %s not found", name.c_str());
	return {};
}

/*
Clears all framebuffers including those used for lighting
*/
void COpengl::ClearFramebuffers()
{
	for (auto o : this->Framebuffers)
	{
		glDeleteRenderbuffers(1,&o.RBO);
		glDeleteTextures(1, &o.CBuffer);
		glDeleteFramebuffers(1,&o.FBO);
	}
	for (auto o : this->LightFramebuffers)
	{
		for (int i = 0; i < 6; i++)
		{
			glDeleteTextures(1, &o.DepthBuff);
			glDeleteFramebuffers(1, &o.FBO);
		}
	}
}

/*
Renders content of Default framebuffer on to the quad. Last part of rendering
*/
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

/*
Calculates data to process light on to the scene. Binds light framebuffer, sends uniforms and changes shader program depending on shadow type
*/
void COpengl::ProcessLight(std::shared_ptr<CLightComponent> lights,int index)
{
	Light LightStruct;
	std::string LightNumber;
	std::string Uniform;

	glm::vec3 LightDir;
	glm::mat4 depthProjectionMatrix;
	glm::mat4 depthViewMatrix;
	glm::mat4 depthMVP;
	glm::mat4 BiasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	glm::mat4 depthBiasMatrix;

	COpengl::AddNewLightFramebuffer(lights, SHADOWMAP_SIZE);
	if (lights->GetLightStruct().LightType == LightType::Point)
	{
#ifndef __EMSCRIPTEN__
		depthProjectionMatrix = glm::perspective<float>(glm::radians(90.f), 1, 0.01f, 100.f);
		COpengl::UseLightFramebuffer(lights->GetName());
		depthViewMatrix = glm::lookAt(lights->GetPosition(), lights->GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));
		std::vector<glm::mat4> ShadowTransforms;
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(lights->GetPosition(), lights->GetPosition() + glm::vec3( 1.0f, 0.0f,  0.0f), glm::vec3(0.0f, -1.0f, 0.0f )));
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(lights->GetPosition(), lights->GetPosition() + glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec3(0.0f, -1.0f, 0.0f )));
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(lights->GetPosition(), lights->GetPosition() + glm::vec3( 0.0f, 1.0f,  0.0f), glm::vec3(0.0f,  0.0f, 1.0f )));
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(lights->GetPosition(), lights->GetPosition() + glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(lights->GetPosition(), lights->GetPosition() + glm::vec3( 0.0f, 0.0f,  1.0f), glm::vec3(0.0f, -1.0f, 0.0f )));
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(lights->GetPosition(), lights->GetPosition() + glm::vec3( 0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f )));

		Shaders.SetCurrentShaderProgram("PointShad");
		for (int i = 0; i < 6; i++)
		{
			glUniformMatrix4fv(Shaders.GetUniformByNameStruct("PointShad", "ShadowMatrices[" + std::to_string(i) + "]"),1,GL_FALSE, &ShadowTransforms[i][0][0]);
		}

		glUniform3f(Shaders.GetUniformByNameStruct("PointShad", "LightPos"), lights->GetPosition().x, lights->GetPosition().y, lights->GetPosition().z);
		glUniform1f(Shaders.GetUniformByNameStruct("PointShad", "FarPlane"), FARPLANE);
#endif
	}
	else if(lights->GetLightStruct().LightType == LightType::Directional)
	{
		depthProjectionMatrix = glm::ortho<float>(-20.f, 20.f, -20.f,20.f, 0.0f, 50.f);
		COpengl::UseLightFramebuffer(lights->GetName());

		auto camera =SceneManager->GetCurrentScene()->GetCamera();
		glm::vec3 LightPostition = camera->GetPosition();

		glm::vec3 FinalVec = lights->GetForwardVector();

		glm::vec3 FVectorCamera = LightPostition + (FinalVec*20.f);
		lights->SetPosition(FVectorCamera);

		depthViewMatrix = glm::lookAt(FVectorCamera, LightPostition, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else // spotlight
	{

	}

	depthMVP = depthProjectionMatrix * depthViewMatrix;
	depthBiasMatrix = BiasMatrix * depthMVP;

	LightStruct = lights->GetLightStruct();
	LightNumber = "Lights[";
	LightNumber += std::to_string(index);
	LightNumber += "]";
	Uniform = LightNumber + ".Position";

	Shaders.SetCurrentShaderProgram("Default");
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
	Uniform = "depthMVP[" + std::to_string(index);
	Uniform += "]";
	glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Default", Uniform), 1, GL_FALSE, &depthMVP[0][0]); //&depthBiasMatrix[0][0]);

	if (lights->GetLightStruct().LightType == LightType::Point)
	{
#ifndef __EMSCRIPTEN__
		Shaders.SetCurrentShaderProgram("PointShad");
		//glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Shadows", "depthMVP"), 1, GL_FALSE, &depthMVP[0][0]);
#endif
	}
	else
	{
		Shaders.SetCurrentShaderProgram("Shadows");
		glUniformMatrix4fv(Shaders.GetUniformByNameStruct("Shadows", "depthMVP"), 1, GL_FALSE, &depthMVP[0][0]);
	}
	glViewport(0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE);
}

/*
Sends shadowmaps as uniforms to default shader program.
*/
void COpengl::PostProcessLight(std::shared_ptr<CLightComponent> light, int count)
{
	std::string Uniform;
	Shaders.SetCurrentShaderProgram("Default");
	glActiveTexture(GL_TEXTURE3 + count);

	if (light->GetLightStruct().LightType == LightType::Point)
	{
#ifndef __EMSCRIPTEN__
		glBindTexture(GL_TEXTURE_CUBE_MAP, COpengl::GetLightFrameBuffer(light->GetName()).DepthBuff);
		Uniform = "ShadowCube[" + std::to_string(count);
		Uniform += "]";
		glUniform1i(Shaders.GetUniformByNameStruct("Default", "ShadowCube"), 3+count);
#endif
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, COpengl::GetLightFrameBuffer(light->GetName()).DepthBuff);
		Uniform = "ShadowMap[" + std::to_string(count);
		Uniform += "]";
		glUniform1i(Shaders.GetUniformByNameStruct("Default", "ShadowMap"), 3+count);
	}
}

Shaders COpengl::GetShadersClass()
{
	return this->Shaders;
}


