#include "glm.hpp"
#include "../../CLog.h"
#include "COpengl.h"
#include "../managers/CSceneManager.h"
#include "../shared/Primitives.h"
#include "../shared/Shared.h"

std::unique_ptr<COpengl> OpenGL;

COpengl::COpengl()
{
	this->_FinalVao = 0;
	this->_OglRenderMode = RenderMode::RenderModeForward;
	this->_Shaders = std::make_shared<Shaders>();
}

COpengl::~COpengl()
{
	CLog::debug("OpenglDestructor");
}

/*
Creates context and initalize glew
*/
void COpengl::Create(SDL_Window *Window)
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
		std::string throwMessage = std::string("Failed to create Context. Error: %s",SDL_GetError());
		CLog::error(throwMessage.c_str());
		throw std::runtime_error(throwMessage);
	}

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::string throwMessage = std::string("Failed to init GLEW. Error: %s",SDL_GetError());
		CLog::error(throwMessage.c_str());
		throw std::runtime_error(throwMessage);
	}
	this->_WndInfo = WindowManager->GetWindowInfo();
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
Creates this->_Shaders and uniforms. Also creates quad for final drawing.
*/
void COpengl::PrepareToLoop()
{
#ifdef __EMSCRIPTEN__
	this->_Shaders->CreateShader("Assets/Shaders/gles/Gui.vert", ShaderType::Vertex);
	this->_Shaders->CreateShader("Assets/Shaders/gles/Gui.frag", ShaderType::Fragment);
	this->_Shaders->CreateShaderProgram("Gui", false);
	this->_Shaders->CreateShader("Assets/Shaders/gles/Scene.vert", ShaderType::Vertex);
	this->_Shaders->CreateShader("Assets/Shaders/gles/Scene.frag", ShaderType::Fragment);
	this->_Shaders->CreateShaderProgram("Default", false);
	this->_Shaders->CreateShader("Assets/Shaders/gles/final.vert", ShaderType::Vertex);
	this->_Shaders->CreateShader("Assets/Shaders/gles/final.frag", ShaderType::Fragment);
	this->_Shaders->CreateShaderProgram("Final", false);
	this->_Shaders->CreateShader("Assets/Shaders/gles/Shadows.vert", ShaderType::Vertex);
	this->_Shaders->CreateShader("Assets/Shaders/gles/Shadows.frag", ShaderType::Fragment);
	this->_Shaders->CreateShaderProgram("Shadows", false);
	this->_Shaders->CreateShader("Assets/Shaders/gles/DebugLights.vert", ShaderType::Vertex);
	this->_Shaders->CreateShader("Assets/Shaders/gles/DebugLights.frag", ShaderType::Fragment);
	this->_Shaders->CreateShaderProgram("DebugLights", false);
#else
	this->_Shaders->CreateShader("Assets/Shaders/Gui.vert", ShaderType::Vertex);
	this->_Shaders->CreateShader("Assets/Shaders/Gui.frag", ShaderType::Fragment);
	this->_Shaders->CreateShaderProgram("Gui", false);
	this->_Shaders->CreateShader("Assets/Shaders/Scene.vert", ShaderType::Vertex);
	this->_Shaders->CreateShader("Assets/Shaders/Scene.frag", ShaderType::Fragment);
	this->_Shaders->CreateShaderProgram("Default", false);
	this->_Shaders->CreateShader("Assets/Shaders/final.vert", ShaderType::Vertex);
	this->_Shaders->CreateShader("Assets/Shaders/final.frag", ShaderType::Fragment);
	this->_Shaders->CreateShaderProgram("Final", false);
	this->_Shaders->CreateShader("Assets/Shaders/Shadows.vert", ShaderType::Vertex);
	this->_Shaders->CreateShader("Assets/Shaders/Shadows.frag", ShaderType::Fragment);
	this->_Shaders->CreateShaderProgram("Shadows", false);
	this->_Shaders->CreateShader("Assets/Shaders/PointShad.vert", ShaderType::Vertex);
	this->_Shaders->CreateShader("Assets/Shaders/PointShad.geom", ShaderType::Geometry);
	this->_Shaders->CreateShader("Assets/Shaders/PointShad.frag", ShaderType::Fragment);
	this->_Shaders->CreateShaderProgram("PointShad", true);
	this->_Shaders->CreateShader("Assets/Shaders/DebugLights.vert", ShaderType::Vertex);
	this->_Shaders->CreateShader("Assets/Shaders/DebugLights.frag", ShaderType::Fragment);
	this->_Shaders->CreateShaderProgram("DebugLights", false);

	this->_Shaders->CreateShader("Assets/Shaders/BaseMap.vert", ShaderType::Vertex);
	this->_Shaders->CreateShader("Assets/Shaders/BaseMap.frag", ShaderType::Fragment);
	this->_Shaders->CreateShaderProgram("BaseMap", false);

	this->_Shaders->CreateShader("Assets/Shaders/LightPass.vert", ShaderType::Vertex);
	this->_Shaders->CreateShader("Assets/Shaders/LightPass.frag", ShaderType::Fragment);
	this->_Shaders->CreateShaderProgram("LightPass", false);

	this->_Shaders->CreateShader("Assets/Shaders/HeightMap.vert", ShaderType::Vertex);
	this->_Shaders->CreateShader("Assets/Shaders/HeightMap.frag", ShaderType::Fragment);
	this->_Shaders->CreateShaderProgram("HeightMap", false);
#endif // __EMSCRIPTEN__

	glClearColor(0, 0, 0, 1);

	// Panel vertices
	GLfloat panelVertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f};

	glGenVertexArrays(1, &this->_FinalVao);
	glBindVertexArray(this->_FinalVao);
	glGenBuffers(1, &this->_FinalVbo);
	glEnableVertexAttribArray(MODEL_TEXCORDBUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, this->_FinalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(panelVertices), panelVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(MODEL_TEXCORDBUFFER, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(MODEL_MESHBUFFER);
	glVertexAttribPointer(MODEL_MESHBUFFER, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
	glDisableVertexAttribArray(MODEL_MESHBUFFER);
	glDisableVertexAttribArray(MODEL_TEXCORDBUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Forward
	this->AddNewFramebuffer("Default", "Default");
	// Deferred
#ifndef __EMSCRIPTEN__
	this->AddNewFramebuffer("DeferredShading", "BaseMap", true);
	this->AddNewFramebuffer("LightPass", "LightPass");
#endif

	this->_Shaders->SetCurrentShaderProgram("Default");
#ifndef __EMSCRIPTEN__
	// Deferred rendering light uniforms
	{
		this->_Shaders->AddUniformToShaderStruct("LightPass", "BaseMap");
		this->_Shaders->AddUniformToShaderStruct("LightPass", "NormalMap");
		this->_Shaders->AddUniformToShaderStruct("LightPass", "PositionMap");
		this->_Shaders->AddUniformToShaderStruct("LightPass", "View");
		this->_Shaders->AddUniformToShaderStruct("LightPass", "Projection");
		this->_Shaders->AddUniformToShaderStruct("LightPass", "Model");
		this->_Shaders->AddUniformToShaderStruct("LightPass", "Camera");
		this->_Shaders->AddUniformToShaderStruct("LightPass", "NormalMatrix");

		this->_Shaders->AddUniformToShaderStruct("LightPass", "gDiffuse");
		this->_Shaders->AddUniformToShaderStruct("LightPass", "gNormal");
		this->_Shaders->AddUniformToShaderStruct("LightPass", "gSpecular");

		this->_Shaders->AddUniformToShaderStruct("LightPass", "ShadowMap");

		this->_Shaders->AddUniformToShaderStruct("LightPass", "ShadowCube");
		this->_Shaders->AddUniformToShaderStruct("LightPass", "LightCount");
		{
			std::string LightNumber;
			std::string Uniform;
			for (int i = 0; i < MAX_LIGHTS; i++)
			{
				LightNumber = "Lights[";
				LightNumber += std::to_string(i);
				LightNumber += "]";
				Uniform = LightNumber + ".Position";
				this->_Shaders->AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".Rotation";
				this->_Shaders->AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".Ambient";
				this->_Shaders->AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".Diffuse";
				this->_Shaders->AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".Specular";
				this->_Shaders->AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".Color";
				this->_Shaders->AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".Constant";
				this->_Shaders->AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".Linear";
				this->_Shaders->AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".Quadratic";
				this->_Shaders->AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".CutoutDist";
				this->_Shaders->AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = LightNumber + ".LightType";
				this->_Shaders->AddUniformToShaderStruct("LightPass", Uniform);
				Uniform = "depthMVP[" + std::to_string(i);
				Uniform += "]";
				this->_Shaders->AddUniformToShaderStruct("LightPass", Uniform);
			}
		}
		this->_Shaders->AddUniformToShaderStruct("LightPass", "Mat.Ambient");
		this->_Shaders->AddUniformToShaderStruct("LightPass", "Mat.Diffuse");
		this->_Shaders->AddUniformToShaderStruct("LightPass", "Mat.Specular");
		this->_Shaders->AddUniformToShaderStruct("LightPass", "Mat.Shininess");
		this->_Shaders->AddUniformToShaderStruct("LightPass", "FarPlane");
	}
#endif
	// Forward rendering uniforms
	{
		this->_Shaders->AddUniformToShaderStruct("Default", "View");
		this->_Shaders->AddUniformToShaderStruct("Default", "Projection");
		this->_Shaders->AddUniformToShaderStruct("Default", "Model");
		this->_Shaders->AddUniformToShaderStruct("Default", "CameraPos");
		this->_Shaders->AddUniformToShaderStruct("Default", "NormalMatrix");

		this->_Shaders->AddUniformToShaderStruct("Default", "Base");
		this->_Shaders->AddUniformToShaderStruct("Default", "Normal");
		this->_Shaders->AddUniformToShaderStruct("Default", "Specular");
		this->_Shaders->AddUniformToShaderStruct("Default", "ShadowMap");
#ifndef __EMSCRIPTEN__
		this->_Shaders->AddUniformToShaderStruct("Default", "ShadowCube");
#endif
		this->_Shaders->AddUniformToShaderStruct("Default", "LightCount");
		{
			std::string LightNumber;
			std::string Uniform;
			for (int i = 0; i < MAX_LIGHTS; i++)
			{
				LightNumber = "Lights[";
				LightNumber += std::to_string(i);
				LightNumber += "]";
				Uniform = LightNumber + ".Position";
				this->_Shaders->AddUniformToShaderStruct("Default", Uniform);
				Uniform = LightNumber + ".Rotation";
				this->_Shaders->AddUniformToShaderStruct("Default", Uniform);
				Uniform = LightNumber + ".Ambient";
				this->_Shaders->AddUniformToShaderStruct("Default", Uniform);
				Uniform = LightNumber + ".Diffuse";
				this->_Shaders->AddUniformToShaderStruct("Default", Uniform);
				Uniform = LightNumber + ".Specular";
				this->_Shaders->AddUniformToShaderStruct("Default", Uniform);
				Uniform = LightNumber + ".Color";
				this->_Shaders->AddUniformToShaderStruct("Default", Uniform);
				Uniform = LightNumber + ".Constant";
				this->_Shaders->AddUniformToShaderStruct("Default", Uniform);
				Uniform = LightNumber + ".Linear";
				this->_Shaders->AddUniformToShaderStruct("Default", Uniform);
				Uniform = LightNumber + ".Quadratic";
				this->_Shaders->AddUniformToShaderStruct("Default", Uniform);
				Uniform = LightNumber + ".CutoutDist";
				this->_Shaders->AddUniformToShaderStruct("Default", Uniform);
				Uniform = LightNumber + ".LightType";
				this->_Shaders->AddUniformToShaderStruct("Default", Uniform);
				Uniform = "depthMVP[" + std::to_string(i);
				Uniform += "]";
				this->_Shaders->AddUniformToShaderStruct("Default", Uniform);
			}
		}
		this->_Shaders->AddUniformToShaderStruct("Default", "Mat.Ambient");
		this->_Shaders->AddUniformToShaderStruct("Default", "Mat.Diffuse");
		this->_Shaders->AddUniformToShaderStruct("Default", "Mat.Specular");
		this->_Shaders->AddUniformToShaderStruct("Default", "Mat.Shininess");
		this->_Shaders->AddUniformToShaderStruct("Default", "FarPlane");
	}
	this->_Shaders->AddUniformToShaderStruct("Gui", "View");
	this->_Shaders->AddUniformToShaderStruct("Gui", "Projection");
	this->_Shaders->AddUniformToShaderStruct("Gui", "Model");
	this->_Shaders->AddUniformToShaderStruct("Gui", "ColorMask");

	this->_Shaders->AddUniformToShaderStruct("Shadows", "depthMVP");
	this->_Shaders->AddUniformToShaderStruct("Shadows", "Model");

#ifndef __EMSCRIPTEN__
	this->_Shaders->AddUniformToShaderStruct("PointShad", "Model");
	this->_Shaders->AddUniformToShaderStruct("PointShad", "LightPos");
	this->_Shaders->AddUniformToShaderStruct("PointShad", "FarPlane");
	for (int i = 0; i < 6; i++)
	{
		this->_Shaders->AddUniformToShaderStruct("PointShad", "ShadowMatrices[" + std::to_string(i) + "]");
	}
#endif

	this->_Shaders->AddUniformToShaderStruct("DebugLights", "Projection");
	this->_Shaders->AddUniformToShaderStruct("DebugLights", "View");
	this->_Shaders->AddUniformToShaderStruct("DebugLights", "Model");
	this->_Shaders->AddUniformToShaderStruct("DebugLights", "Color");

	this->_Shaders->AddUniformToShaderStruct("HeightMap", "Projection");
	this->_Shaders->AddUniformToShaderStruct("HeightMap", "View");
	this->_Shaders->AddUniformToShaderStruct("HeightMap", "Model");
	this->_Shaders->AddUniformToShaderStruct("HeightMap", "Color");

	// Deferred rendering geometry uniforms
#ifndef __EMSCRIPTEN__
	this->_Shaders->AddUniformToShaderStruct("BaseMap", "Projection");
	this->_Shaders->AddUniformToShaderStruct("BaseMap", "View");
	this->_Shaders->AddUniformToShaderStruct("BaseMap", "Model");
	this->_Shaders->AddUniformToShaderStruct("BaseMap", "Base");
	this->_Shaders->AddUniformToShaderStruct("BaseMap", "Normal");
	this->_Shaders->AddUniformToShaderStruct("BaseMap", "Specular");
#endif
	this->_Shaders->AddUniformToShaderStruct("Final", "Base");
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
	this->_Shaders->UniformMat4f(matrix, "Model");
}

/*
Sets colour of gui element
*/
void COpengl::SetColorMaskLayout(glm::vec3 ColorMask)
{
	this->_Shaders->Uniform3f(ColorMask, "ColorMask");
}

/*
Calculates inversed and transposed normal matrix to base drawing struct
*/
void COpengl::SetNormalMatrix(glm::mat4 matrix)
{
	glm::mat4 NormalMatrix = glm::transpose(glm::inverse(matrix));
	this->_Shaders->UniformMat4f(NormalMatrix, "NormalMatrix");
}

/*
Use default program and swap buffers
*/
void COpengl::ProLoop(SDL_Window *Window)
{
	glUseProgram(0);
	SDL_GL_SwapWindow(Window);
}

/*
Sets viewport based on camera settings
*/
void COpengl::PreLoopPerspective(std::shared_ptr<CCameraComponent> Camera)
{
	if (Camera != nullptr)
	{
		this->_Shaders->UniformMat4f(Camera->GetViewMatrix(), "View");
		this->_Shaders->UniformMat4f(Camera->GetPerspectiveMatrix(), "Projection");
		this->_Shaders->Uniform1f(FARPLANE, "FarPlane");
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
	this->_Shaders->SetCurrentShaderProgram("Gui");
	this->_ViewMatrix = glm::lookAt(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Projection = glm::ortho(0.f, (float)this->_WndInfo->ScreenWidth, (float)this->_WndInfo->ScreenHeight, 0.0f, 0.1f, 1000.0f);
	this->_Shaders->UniformMat4f(this->_ViewMatrix, "View");
	this->_Shaders->UniformMat4f(Projection, "Projection");
	glCullFace(GL_BACK);
}
/*
Creates new framebuffer for drawing. Creates framebuffer and renderbuffer
*/
void COpengl::AddNewFramebuffer(std::string FBName, const char *ShaderName, bool Deferred)
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->_WndInfo->ScreenWidth, this->_WndInfo->ScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FboStruct.Deferred[0], 0);

		// - normal color buffer
		glGenTextures(1, &FboStruct.Deferred[1]);
		glBindTexture(GL_TEXTURE_2D, FboStruct.Deferred[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->_WndInfo->ScreenWidth, this->_WndInfo->ScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, FboStruct.Deferred[1], 0);

		// - color + specular color buffer
		glGenTextures(1, &FboStruct.Deferred[2]);
		glBindTexture(GL_TEXTURE_2D, FboStruct.Deferred[2]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->_WndInfo->ScreenWidth, this->_WndInfo->ScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, FboStruct.Deferred[2], 0);

		glGenRenderbuffers(1, &FboStruct.RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, FboStruct.RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->_WndInfo->ScreenWidth, this->_WndInfo->ScreenHeight);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, this->_WndInfo->ScreenWidth, this->_WndInfo->ScreenHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, FboStruct.RBO);

		// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering

		unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
		glDrawBuffers(3, attachments);
	}
	else
	{
		glGenTextures(1, &FboStruct.CBuffer);
		glBindTexture(GL_TEXTURE_2D, FboStruct.CBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->_WndInfo->ScreenWidth, this->_WndInfo->ScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FboStruct.CBuffer, 0);

		glGenRenderbuffers(1, &FboStruct.RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, FboStruct.RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->_WndInfo->ScreenWidth, this->_WndInfo->ScreenHeight);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, this->_WndInfo->ScreenWidth, this->_WndInfo->ScreenHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, FboStruct.RBO);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		CLog::error("Framebuffer %s is not complete", FBName.c_str());
	}
	FboStruct.ShaderName = ShaderName;
	this->_Framebuffers.push_back(FboStruct);
}

/*
Creates framebuffer that are used for lighting. Those saves only depth from the scene.
*/
void COpengl::AddNewLightFramebuffer(std::shared_ptr<CLightComponent> light, int size)
{
	std::string name = light->GetPossesingObject()->GetName() + "_" + light->GetName();
	for (auto x : this->_LightFramebuffers)
	{
		if (name.compare(x.name) == 0)
		{
			return;
		}
	}
	if (size < 0)
	{
		CLog::error("Light Framebuffer has to be bigger than 0 %s", name.c_str());
		return;
	}
	MyLightFramebuffer FboStruct;
	FboStruct.name = name;
	glGenFramebuffers(1, &FboStruct.FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FboStruct.FBO);

	glGenTextures(1, &FboStruct.DepthBuff);
	if (light->GetLightType() == LightType::Point)
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
		float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
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
		do
		{
			result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			CLog::error("Light Framebuffer isn't complete %s :%d", light->GetName().c_str(), glGetError());
		} while (result != GL_FRAMEBUFFER_COMPLETE);
	}
	this->_LightFramebuffers.push_back(FboStruct);
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

	for (auto o : this->_LightFramebuffers)
	{
		if (o.name.compare(name) == 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, o.FBO);
			glClearColor(1.f, 1.f, 1.f, 1.0f);
			glClear(GL_DEPTH_BUFFER_BIT);
			glCullFace(GL_BACK);
			return;
		}
	}
}

/*
Returns LightFramebuffer object
*/
MyLightFramebuffer COpengl::GetLightFrameBuffer(std::string name)
{
	for (auto o : this->_LightFramebuffers)
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
	if (name.compare("0") == 0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		this->_Shaders->SetCurrentShaderProgram("Final");
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_BACK);
		return;
	}
	for (auto o : this->_Framebuffers)
	{
		if (name.compare(o.name) == 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, o.FBO);
			this->_Shaders->SetCurrentShaderProgram(o.ShaderName);
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
	for (auto o : this->_Framebuffers)
	{
		if (o.name.compare(name) == 0)
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
	for (auto o : this->_Framebuffers)
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
	for (auto o : this->_LightFramebuffers)
	{
		for (int i = 0; i < 6; i++)
		{
			glDeleteTextures(1, &o.DepthBuff);
			glDeleteFramebuffers(1, &o.FBO);
		}
	}
}
/*
Draws on the scene positions of the current active light as a box colored in light colour
*/
void COpengl::DrawDebugLights(std::vector<std::shared_ptr<CLightComponent>> list, std::shared_ptr<CCameraComponent> camera)
{
	this->_Shaders->SetCurrentShaderProgram("DebugLights");
	if (camera != nullptr)
	{
		this->_ViewMatrix = glm::lookAt(camera->GetPosition(), camera->GetPosition() + camera->GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 Projection = glm::perspective(glm::radians(camera->GetFov()), this->_WndInfo->ScreenAspectRatio, 0.1f, 100.0f);
		this->_Shaders->UniformMat4f(this->_ViewMatrix, "View");
		this->_Shaders->UniformMat4f(Projection, "Projection");
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
			this->_Shaders->UniformMat4f(a->GetModelMatrix(), "Model");
			Color = glm::vec3(a->GetColor().r, a->GetColor().g, a->GetColor().b);
			this->_Shaders->Uniform3f(Color, "Color");
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
	glBindVertexArray(this->_FinalVao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	if (this->_OglRenderMode == RenderMode::RenderModeDeferred)
	{
		this->_Shaders->SetCurrentShaderProgram("LightPass");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->GetFramebuffer("DeferredShading").Deferred[0]);
		this->_Shaders->Uniform1i(0, "PositionMap");

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->GetFramebuffer("DeferredShading").Deferred[1]);
		this->_Shaders->Uniform1i(1, "NormalMap");

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->GetFramebuffer("DeferredShading").Deferred[2]);
		this->_Shaders->Uniform1i(2, "BaseMap");
	}
	else
	{
		this->_Shaders->SetCurrentShaderProgram("Final");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->GetFramebuffer("Default").CBuffer);
		this->_Shaders->Uniform1i(0, "Base");
	}
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

/*
Calculates data to process light on to the scene. Binds light framebuffer, sends uniforms and changes shader program depending on shadow type
*/
void COpengl::ProcessLight(std::shared_ptr<CLightComponent> light, int index)
{
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
		0.5, 0.5, 0.5, 1.0);
	glm::mat4 depthBiasMatrix;
	std::string name = light->GetPossesingObject()->GetName() + "_" + light->GetName();

	COpengl::AddNewLightFramebuffer(light, SHADOWMAP_SIZE);
	if (light->GetLightType() == LightType::Point)
	{
#ifndef __EMSCRIPTEN__
		depthProjectionMatrix = glm::perspective<float>(glm::radians(90.f), 1, 0.01f, 100.f);
		COpengl::UseLightFramebuffer(name);
		depthViewMatrix = glm::lookAt(light->GetPosition(), light->GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));
		std::vector<glm::mat4> ShadowTransforms;
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(light->GetPosition(), light->GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(light->GetPosition(), light->GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(light->GetPosition(), light->GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(light->GetPosition(), light->GetPosition() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(light->GetPosition(), light->GetPosition() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		ShadowTransforms.push_back(depthProjectionMatrix * glm::lookAt(light->GetPosition(), light->GetPosition() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

		this->_Shaders->SetCurrentShaderProgram("PointShad");
		for (int i = 0; i < 6; i++)
		{
			glUniformMatrix4fv(this->_Shaders->GetUniformByNameStruct("PointShad", "ShadowMatrices[" + std::to_string(i) + "]"), 1, GL_FALSE, &ShadowTransforms[i][0][0]);
		}

		this->_Shaders->Uniform3f(light->GetPosition(), "LightPos");
		this->_Shaders->Uniform1f(FARPLANE, "FarPlane");
#endif
	}
	else if (light->GetLightType() == LightType::Directional)
	{
		depthProjectionMatrix = glm::ortho<float>(-30.f, 30.f, -30.f, 30.f, 0.0f, 30.f);
		COpengl::UseLightFramebuffer(name);

		auto camera = SceneManager->GetCurrentScene()->GetCamera();
		glm::vec3 LightPostition = camera->GetPosition();

		glm::vec3 FinalVec = light->GetForwardVector();

		glm::vec3 FVectorCamera = LightPostition + (FinalVec * 10.f);
		light->SetPosition(FVectorCamera);
		light->SetRotation(light->GetForwardVector());

		depthViewMatrix = glm::lookAt(FVectorCamera, LightPostition, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else // spotlight
	{
	}

	depthMVP = depthProjectionMatrix * depthViewMatrix;
	depthBiasMatrix = BiasMatrix * depthMVP;

	LightNumber = "Lights[";
	LightNumber += std::to_string(index);
	LightNumber += "]";

	Uniform = LightNumber + ".Position";
	if (this->_OglRenderMode == RenderMode::RenderModeDeferred)
	{
		this->_Shaders->SetCurrentShaderProgram("LightPass");
	}
	else
	{
		this->_Shaders->SetCurrentShaderProgram("Default");
	}
	this->_Shaders->Uniform3f(light->GetPosition(), Uniform.c_str());
	Uniform = LightNumber + ".Rotation";
	this->_Shaders->Uniform3f(light->GetRotation(), Uniform.c_str());
	Uniform = LightNumber + ".Ambient";
	this->_Shaders->Uniform3f(light->GetAmbient(), Uniform.c_str());
	Uniform = LightNumber + ".Diffuse";
	this->_Shaders->Uniform3f(light->GetDiffuse(), Uniform.c_str());
	Uniform = LightNumber + ".Specular";
	this->_Shaders->Uniform3f(light->GetSpecular(), Uniform.c_str());
	Uniform = LightNumber + ".Color";
	this->_Shaders->Uniform3f(light->GetColor(), Uniform.c_str());
	Uniform = LightNumber + ".Constant";
	this->_Shaders->Uniform1f(light->GetConstant(), Uniform.c_str());
	Uniform = LightNumber + ".Linear";
	this->_Shaders->Uniform1f(light->GetLinear(), Uniform.c_str());
	Uniform = LightNumber + ".Quadratic";
	this->_Shaders->Uniform1f(light->GetQuadratic(), Uniform.c_str());
	Uniform = LightNumber + ".CutoutDist";
	this->_Shaders->Uniform1f(light->GetCutoutDist(), Uniform.c_str());
	Uniform = LightNumber + ".LightType";
	this->_Shaders->Uniform1i(light->GetLightType(), Uniform.c_str());
	Uniform = "depthMVP[" + std::to_string(index);
	Uniform += "]";
	this->_Shaders->UniformMat4f(depthMVP, Uniform.c_str());
	this->_Shaders->UniformMat4f(depthMVP, "depthMVP", "Shadows");

	if (light->GetLightType() == LightType::Point)
	{
#ifndef __EMSCRIPTEN__
		this->_Shaders->SetCurrentShaderProgram("PointShad");
#endif
	}
	else
	{
		this->_Shaders->SetCurrentShaderProgram("Shadows");
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
	if (this->_OglRenderMode == RenderMode::RenderModeDeferred)
	{
		this->_Shaders->SetCurrentShaderProgram("LightPass");
	}
	else
	{
		this->_Shaders->SetCurrentShaderProgram("Default");
	}
	glActiveTexture(GL_TEXTURE3 + count);

	if (light->GetLightType() == LightType::Point)
	{
#ifndef __EMSCRIPTEN__
		glBindTexture(GL_TEXTURE_CUBE_MAP, COpengl::GetLightFrameBuffer(name).DepthBuff);
		Uniform = "ShadowCube[" + std::to_string(count);
		Uniform += "]";
		this->_Shaders->Uniform1i(3 + count, "ShadowCube");
		// glUniform1i(Shaders->GetUniformByNameStruct("Default", "ShadowCube"), 3+count);
#endif
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, COpengl::GetLightFrameBuffer(name).DepthBuff);
		Uniform = "ShadowMap[" + std::to_string(count);
		Uniform += "]";
		this->_Shaders->Uniform1i(3 + count, "ShadowMap");
		// glUniform1i(Shaders->GetUniformByNameStruct("Default", "ShadowMap"), 3+count);
	}
}

std::shared_ptr<Shaders> COpengl::GetShadersClass()
{
	return this->_Shaders;
}

void COpengl::SetCurrentShaderProgram(std::string name)
{
	this->_Shaders->SetCurrentShaderProgram(name);
}