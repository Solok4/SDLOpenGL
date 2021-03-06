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
#include "Primitives.h"

std::unique_ptr<COpengl> OpenGL;

COpengl::COpengl()
{
	this->FinalVao = 0;
	//this->FinalVbo = 0;
	this->OglRenderMode = RenderMode::RenderModeForward;
}

COpengl::~COpengl()
{
	CLog::debug("OpenglDestructor");
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
		CLog::error("Failed to create Context");
		return false;
	}

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		CLog::error("Failed to init GLEW");
		return false;
	}
	this->WndInfo = Renderer->GetWindowInfo();
	return true;
}

void COpengl::Delete()
{
	this->ClearFramebuffers();
	if (_Context != NULL)
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
	Shaders.CreateShader("Assets/Shaders/gles/Gui.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/gles/Gui.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("Gui", false);
	Shaders.CreateShader("Assets/Shaders/gles/Scene.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/gles/Scene.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("Default", false);
	Shaders.CreateShader("Assets/Shaders/gles/final.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/gles/final.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("Final", false);
	Shaders.CreateShader("Assets/Shaders/gles/Shadows.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/gles/Shadows.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("Shadows", false);
	Shaders.CreateShader("Assets/Shaders/gles/DebugLights.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/gles/DebugLights.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("DebugLights", false);
#else
	Shaders.CreateShader("Assets/Shaders/Gui.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/Gui.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("Gui", false);
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
	Shaders.CreateShaderProgram("PointShad", true);
	Shaders.CreateShader("Assets/Shaders/DebugLights.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/DebugLights.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("DebugLights", false);

	Shaders.CreateShader("Assets/Shaders/BaseMap.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/BaseMap.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("BaseMap", false);

	Shaders.CreateShader("Assets/Shaders/LightPass.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/LightPass.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("LightPass", false);

	Shaders.CreateShader("Assets/Shaders/HeightMap.vert", ShaderType::Vertex);
	Shaders.CreateShader("Assets/Shaders/HeightMap.frag", ShaderType::Fragment);
	Shaders.CreateShaderProgram("HeightMap", false);
#endif // __EMSCRIPTEN__

	glClearColor(0, 0, 0, 1);

	//Panel vertices
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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Forward
	this->AddNewFramebuffer("Default", "Default");
	//Deferred
#ifndef __EMSCRIPTEN__
	this->AddNewFramebuffer("DeferredShading", "BaseMap", true);
	this->AddNewFramebuffer("LightPass", "LightPass");
#endif

	Shaders.SetCurrentShaderProgram("Default");
#ifndef __EMSCRIPTEN__
	//Deferred rendering light uniforms
	{
		Shaders.AddUniformToShaderStruct("LightPass", "BaseMap");
		Shaders.AddUniformToShaderStruct("LightPass", "NormalMap");
		Shaders.AddUniformToShaderStruct("LightPass", "PositionMap");
		Shaders.AddUniformToShaderStruct("LightPass", "View");
		Shaders.AddUniformToShaderStruct("LightPass", "Projection");
		Shaders.AddUniformToShaderStruct("LightPass", "Model");
		Shaders.AddUniformToShaderStruct("LightPass", "Camera");
		Shaders.AddUniformToShaderStruct("LightPass", "NormalMatrix");

		Shaders.AddUniformToShaderStruct("LightPass", "gDiffuse");
		Shaders.AddUniformToShaderStruct("LightPass", "gNormal");
		Shaders.AddUniformToShaderStruct("LightPass", "gSpecular");

		Shaders.AddUniformToShaderStruct("LightPass", "ShadowMap");

		Shaders.AddUniformToShaderStruct("LightPass", "ShadowCube");
		Shaders.AddUniformToShaderStruct("LightPass", "LightCount");
		{
			std::string LightNumber;
			std::string Uniform;
			for (int i = 0; i < MAX_LIGHTS; i++)
			{
				LightNumber = "Lights[";
				LightNumber += std::to_string(i);
				LightNumber += "]";
				Uniform = LightNumber + ".Position";
				Shaders.AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".Rotation";
				Shaders.AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".Ambient";
				Shaders.AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".Diffuse";
				Shaders.AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".Specular";
				Shaders.AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".Color";
				Shaders.AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".Constant";
				Shaders.AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".Linear";
				Shaders.AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".Quadratic";
				Shaders.AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".CutoutDist";
				Shaders.AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".LightType";
				Shaders.AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = "depthMVP[" + std::to_string(i);
				Uniform += "]";
				Shaders.AddUniformToShaderStruct("LightPass", Uniform);
			}
		}
		Shaders.AddUniformToShaderStruct("LightPass", "Mat.Ambient");
		Shaders.AddUniformToShaderStruct("LightPass", "Mat.Diffuse");
		Shaders.AddUniformToShaderStruct("LightPass", "Mat.Specular");
		Shaders.AddUniformToShaderStruct("LightPass", "Mat.Shininess");
		Shaders.AddUniformToShaderStruct("LightPass", "FarPlane");
	}
#endif
	//Forward rendering uniforms
	{
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
	}
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

	Shaders.AddUniformToShaderStruct("DebugLights", "Projection");
	Shaders.AddUniformToShaderStruct("DebugLights", "View");
	Shaders.AddUniformToShaderStruct("DebugLights", "Model");
	Shaders.AddUniformToShaderStruct("DebugLights", "Color");

	Shaders.AddUniformToShaderStruct("HeightMap", "Projection");
	Shaders.AddUniformToShaderStruct("HeightMap", "View");
	Shaders.AddUniformToShaderStruct("HeightMap", "Model");
	Shaders.AddUniformToShaderStruct("HeightMap", "Color");

	//Deferred rendering geometry uniforms
#ifndef __EMSCRIPTEN__
	Shaders.AddUniformToShaderStruct("BaseMap", "Projection");
	Shaders.AddUniformToShaderStruct("BaseMap", "View");
	Shaders.AddUniformToShaderStruct("BaseMap", "Model");
	Shaders.AddUniformToShaderStruct("BaseMap", "Base");
	Shaders.AddUniformToShaderStruct("BaseMap", "Normal");
	Shaders.AddUniformToShaderStruct("BaseMap", "Specular");
#endif
	Shaders.AddUniformToShaderStruct("Final", "Base");
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
	Shaders.UniformMat4f(matrix, "Model");
}

/*
Sets colour of gui element
*/
void COpengl::SetColorMaskLayout(glm::vec3 ColorMask)
{
	Shaders.Uniform3f(ColorMask, "ColorMask");
}

/*
Calculates inversed and transposed normal matrix to base drawing struct
*/
void COpengl::SetNormalMatrix(glm::mat4 matrix)
{
	glm::mat4 NormalMatrix = glm::transpose(glm::inverse(matrix));
	Shaders.UniformMat4f(NormalMatrix, "NormalMatrix");
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
		Shaders.UniformMat4f(Camera->GetViewMatrix(), "View");
		Shaders.UniformMat4f(Camera->GetPerspectiveMatrix(), "Projection");
		Shaders.Uniform1f(FARPLANE, "FarPlane");
		glCullFace(GL_BACK);
	}
	else
	{
		CLog::error("Passed camera doesn't exist");
	}
}

/*
Sets viewport for gui elements
*/
void COpengl::PreLoopOrtho()
{
	Shaders.SetCurrentShaderProgram("Gui");
	ViewMatrix = glm::lookAt(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Projection = glm::ortho(0.f, (float)this->WndInfo->ScreenWidth, (float)this->WndInfo->ScreenHeight, 0.0f, 0.1f, 1000.0f);
	Shaders.UniformMat4f(ViewMatrix, "View");
	Shaders.UniformMat4f(Projection, "Projection");
	glCullFace(GL_BACK);
}
/*
Creates new framebuffer for drawing. Creates framebuffer and renderbuffer
*/
void COpengl::AddNewFramebuffer(std::string FBName, const char* ShaderName, bool Deferred)
{
	MyFrameBuffer FboStruct;
	FboStruct.name = FBName;
	glGenFramebuffers(1, &FboStruct.FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FboStruct.FBO);

	if (Deferred)
	{
		FboStruct.DeferredShading = true;
		// Diffuse
		glGenTextures(1, &FboStruct.Deferred[0]);
		glBindTexture(GL_TEXTURE_2D, FboStruct.Deferred[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->WndInfo->ScreenWidth, this->WndInfo->ScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FboStruct.Deferred[0], 0);

		// - normal color buffer
		glGenTextures(1, &FboStruct.Deferred[1]);
		glBindTexture(GL_TEXTURE_2D, FboStruct.Deferred[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->WndInfo->ScreenWidth, this->WndInfo->ScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, FboStruct.Deferred[1], 0);

		// - color + specular color buffer
		glGenTextures(1, &FboStruct.Deferred[2]);
		glBindTexture(GL_TEXTURE_2D, FboStruct.Deferred[2]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->WndInfo->ScreenWidth, this->WndInfo->ScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, FboStruct.Deferred[2], 0);

		glGenRenderbuffers(1, &FboStruct.RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, FboStruct.RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->WndInfo->ScreenWidth, this->WndInfo->ScreenHeight);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, this->WndInfo->ScreenWidth, this->WndInfo->ScreenHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, FboStruct.RBO);

		// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering

		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);
	}
	else
	{
		glGenTextures(1, &FboStruct.CBuffer);
		glBindTexture(GL_TEXTURE_2D, FboStruct.CBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->WndInfo->ScreenWidth, this->WndInfo->ScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FboStruct.CBuffer, 0);

		glGenRenderbuffers(1, &FboStruct.RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, FboStruct.RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->WndInfo->ScreenWidth, this->WndInfo->ScreenHeight);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, this->WndInfo->ScreenWidth, this->WndInfo->ScreenHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, FboStruct.RBO);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		CLog::error("Framebuffer %s is not complete", FBName.c_str());
	}
	FboStruct.ShaderName = ShaderName;
	this->Framebuffers.push_back(FboStruct);
}

/*
Creates framebuffer that are used for lighting. Those saves only depth from the scene.
*/
void COpengl::AddNewLightFramebuffer(std::shared_ptr<CLightComponent> light, int size)
{
	std::string name = light->GetPossesingObject()->GetName() + "_" + light->GetName();
	for (auto x : this->LightFramebuffers)
	{
		if (strcmp(name.c_str(), x.name.c_str()) == 0)
		{
			return;
		}
	}
	if (size < 0)
	{
		CLog::error("Light Framebuffer has to be bigger than 0 %s", name.c_str());
		return;
	}
	else if (size % 2 != 0)
	{
		CLog::error("It would be better if light framebuffer will be divisible by 2 %s", name.c_str());
	}
	MyLightFramebuffer FboStruct;
	FboStruct.name = name;
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

			CLog::error("Light Framebuffer isn't complete %s :%d", light->GetName().c_str(), glGetError());
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
		CLog::error("LightFramebuffer name can't be empty");
		return;
	}
	else
	{
		for (auto o : this->LightFramebuffers)
		{
			if (strcmp(o.name.c_str(), name.c_str()) == 0)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, o.FBO);
				glClearColor(1.f, 1.f, 1.f, 1.0f);
				glClear(GL_DEPTH_BUFFER_BIT);
				//glCullFace(GL_FRONT);
				glCullFace(GL_BACK);
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
	CLog::error("LightFramebuffer named %s not found", name.c_str());
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
	CLog::error("Framebuffer named %s not found", name.c_str());
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
	CLog::error("Framebuffer named %s not found", name.c_str());
	return {};
}

/*
Clears all framebuffers including those used for lighting
*/
void COpengl::ClearAllFramebuffers()
{
	this->ClearFramebuffers();
	this->ClearLightFramebuffers();
}
void COpengl::ClearFramebuffers()
{
	for (auto o : this->Framebuffers)
	{
		if (o.DeferredShading)
		{
			glDeleteTextures(3, &o.CBuffer);
		}
		else
		{
			glDeleteTextures(1, &o.CBuffer);
		}
		glDeleteRenderbuffers(1, &o.RBO);
		glDeleteFramebuffers(1, &o.FBO);
	}
}
void COpengl::ClearLightFramebuffers()
{
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
Draws on the scene positions of the current active lights as a box colored in lights colour
*/
void COpengl::DrawDebugLights(std::vector<std::shared_ptr<CLightComponent>> list, std::shared_ptr<CCameraComponent> camera)
{
	Shaders.SetCurrentShaderProgram("DebugLights");
	if (camera != nullptr)
	{
		ViewMatrix = glm::lookAt(camera->GetPosition(), camera->GetPosition() + camera->GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 Projection = glm::perspective(glm::radians(camera->GetFov()), this->WndInfo->ScreenAspectRatio, 0.1f, 100.0f);
		Shaders.UniformMat4f(ViewMatrix, "View");
		Shaders.UniformMat4f(Projection, "Projection");
	}
	else
	{
		CLog::error("Passed camera doesn't exist");
	}
	auto model = ModelManager->GetModelByName("Cube.obj");
	glBindVertexArray(model->VAO);
	glEnableVertexAttribArray(MODEL_MESHBUFFER);
	glm::vec3 Color;
	for (auto a : list)
	{
		if (a->IsActive())
		{
			Shaders.UniformMat4f(a->GetModelMatrix(), "Model");
			Color = glm::vec3(a->GetLightStruct().Color.r, a->GetLightStruct().Color.g, a->GetLightStruct().Color.b);
			Shaders.Uniform3f(Color, "Color");
			glDrawArrays(GL_TRIANGLES, 0, model->IndicesCount);
		}
	}
	glDisableVertexAttribArray(MODEL_MESHBUFFER);
	glBindVertexArray(0);
}

/*
Renders content of Default framebuffer on to the quad. Last part of rendering
*/
void COpengl::FinalDraw()
{
	this->UseFramebuffer("0");
	glBindVertexArray(this->FinalVao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	if (this->OglRenderMode == RenderMode::RenderModeDeferred)
	{
		Shaders.SetCurrentShaderProgram("LightPass");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->GetFramebuffer("DeferredShading").Deferred[0]);
		Shaders.Uniform1i(0, "PositionMap");

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->GetFramebuffer("DeferredShading").Deferred[1]);
		Shaders.Uniform1i(1, "NormalMap");

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->GetFramebuffer("DeferredShading").Deferred[2]);
		Shaders.Uniform1i(2, "BaseMap");
	}
	else
	{
		Shaders.SetCurrentShaderProgram("Final");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->GetFramebuffer("Default").CBuffer);
		Shaders.Uniform1i(0, "Base");
	}
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

/*
Calculates data to process light on to the scene. Binds light framebuffer, sends uniforms and changes shader program depending on shadow type
*/
void COpengl::ProcessLight(std::shared_ptr<CLightComponent> lights, int index)
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
	std::string name = lights->GetPossesingObject()->GetName() + "_" + lights->GetName();

	COpengl::AddNewLightFramebuffer(lights, SHADOWMAP_SIZE);
	if (lights->GetLightStruct().LightType == LightType::Point)
	{
#ifndef __EMSCRIPTEN__
		depthProjectionMatrix = glm::perspective<float>(glm::radians(90.f), 1, 0.01f, 100.f);
		COpengl::UseLightFramebuffer(name);
		depthViewMatrix = glm::lookAt(lights->GetPosition(), lights->GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));
		std::vector<glm::mat4> ShadowTransforms;
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(lights->GetPosition(), lights->GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(lights->GetPosition(), lights->GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(lights->GetPosition(), lights->GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(lights->GetPosition(), lights->GetPosition() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(lights->GetPosition(), lights->GetPosition() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(lights->GetPosition(), lights->GetPosition() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

		Shaders.SetCurrentShaderProgram("PointShad");
		for (int i = 0; i < 6; i++)
		{
			glUniformMatrix4fv(Shaders.GetUniformByNameStruct("PointShad", "ShadowMatrices[" + std::to_string(i) + "]"), 1, GL_FALSE, &ShadowTransforms[i][0][0]);
		}

		Shaders.Uniform3f(lights->GetPosition(), "LightPos");
		Shaders.Uniform1f(FARPLANE, "FarPlane");
#endif
	}
	else if (lights->GetLightStruct().LightType == LightType::Directional)
	{
		depthProjectionMatrix = glm::ortho<float>(-30.f, 30.f, -30.f, 30.f, 0.0f, 30.f);
		COpengl::UseLightFramebuffer(name);

		auto camera = SceneManager->GetCurrentScene()->GetCamera();
		glm::vec3 LightPostition = camera->GetPosition();

		glm::vec3 FinalVec = lights->GetForwardVector();

		glm::vec3 FVectorCamera = LightPostition + (FinalVec * 10.f);
		lights->SetPosition(FVectorCamera);
		lights->SetRotation(lights->GetForwardVector());

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
	if (this->OglRenderMode == RenderMode::RenderModeDeferred)
	{
		Shaders.SetCurrentShaderProgram("LightPass");
	}
	else
	{
		Shaders.SetCurrentShaderProgram("Default");
	}
	Shaders.Uniform3f(LightStruct.Position, Uniform.c_str());
	Uniform = LightNumber + ".Rotation";
	Shaders.Uniform3f(LightStruct.Rotation, Uniform.c_str());
	Uniform = LightNumber + ".Ambient";
	Shaders.Uniform3f(LightStruct.Ambient, Uniform.c_str());
	Uniform = LightNumber + ".Diffuse";
	Shaders.Uniform3f(LightStruct.Diffuse, Uniform.c_str());
	Uniform = LightNumber + ".Specular";
	Shaders.Uniform3f(LightStruct.Specular, Uniform.c_str());
	Uniform = LightNumber + ".Color";
	Shaders.Uniform3f(LightStruct.Color, Uniform.c_str());
	Uniform = LightNumber + ".Constant";
	Shaders.Uniform1f(LightStruct.Constant, Uniform.c_str());
	Uniform = LightNumber + ".Linear";
	Shaders.Uniform1f(LightStruct.Linear, Uniform.c_str());
	Uniform = LightNumber + ".Quadratic";
	Shaders.Uniform1f(LightStruct.Quadratic, Uniform.c_str());
	Uniform = LightNumber + ".CutoutDist";
	Shaders.Uniform1f(LightStruct.CutoutDist, Uniform.c_str());
	Uniform = LightNumber + ".LightType";
	Shaders.Uniform1i(LightStruct.LightType, Uniform.c_str());
	Uniform = "depthMVP[" + std::to_string(index);
	Uniform += "]";
	Shaders.UniformMat4f(depthMVP, Uniform.c_str());
	Shaders.UniformMat4f(depthMVP, "depthMVP", "Shadows");

	if (lights->GetLightStruct().LightType == LightType::Point)
	{
#ifndef __EMSCRIPTEN__
		Shaders.SetCurrentShaderProgram("PointShad");
#endif
	}
	else
	{
		Shaders.SetCurrentShaderProgram("Shadows");
	}
	glViewport(0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE);
}

/*
Sends shadowmaps as uniforms to default shader program.
*/
void COpengl::PostProcessLight(std::shared_ptr<CLightComponent> light, int count)
{
	std::string name = light->GetPossesingObject()->GetName() + "_" + light->GetName();
	std::string Uniform;
	if (this->OglRenderMode == RenderMode::RenderModeDeferred)
	{
		Shaders.SetCurrentShaderProgram("LightPass");
	}
	else
	{
		Shaders.SetCurrentShaderProgram("Default");
	}
	glActiveTexture(GL_TEXTURE3 + count);

	if (light->GetLightStruct().LightType == LightType::Point)
	{
#ifndef __EMSCRIPTEN__
		glBindTexture(GL_TEXTURE_CUBE_MAP, COpengl::GetLightFrameBuffer(name).DepthBuff);
		Uniform = "ShadowCube[" + std::to_string(count);
		Uniform += "]";
		this->Shaders.Uniform1i(3 + count, "ShadowCube");
		//glUniform1i(Shaders.GetUniformByNameStruct("Default", "ShadowCube"), 3+count);
#endif
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, COpengl::GetLightFrameBuffer(name).DepthBuff);
		Uniform = "ShadowMap[" + std::to_string(count);
		Uniform += "]";
		this->Shaders.Uniform1i(3 + count, "ShadowMap");
		//glUniform1i(Shaders.GetUniformByNameStruct("Default", "ShadowMap"), 3+count);
	}
}

Shaders COpengl::GetShadersClass()
{
	return this->Shaders;
}

void COpengl::SetCurrentShaderProgram(std::string name)
{
	this->Shaders.SetCurrentShaderProgram(name);
}