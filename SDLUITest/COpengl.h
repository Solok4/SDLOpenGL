#pragma once


#ifdef __EMSCRIPTEN__
#include <SDL2/SDL.h>
#else
#include "SDL.h"
#endif // __EMSCRIPTEN__
#include <vector>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "CCameraComponent.h"
#include "Shaders.h"
#include "CLightComponent.h"
//#include "CScene.h"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")

struct MyFrameBuffer
{
	std::string name;
	GLuint FBO=0;
	GLuint RBO=0;
	GLuint CBuffer=0;
	const char* ShaderName;
};

struct MyLightFramebuffer
{
	std::string name;
	GLuint FBO;
	GLuint DepthBuff;
};



class COpengl
{
public:
	COpengl();
	~COpengl();

	bool Create(SDL_Window* Window);
	void Delete();

	void PrepareToLoop();
	void PreLoop();
	void SetModelMatrix(glm::mat4 matrix);
	void SetModelMatrixLayout(glm::mat4 matrix);
	void SetColorMaskLayout(glm::vec3 ColorMask);
	void SetNormalMatrix(glm::mat4 matrix);

	void ProLoop(SDL_Window* Window);
	void PreLoopPerspective(std::shared_ptr<CCameraComponent> Camera);
	void PreLoopOrtho(SDL_Window* Window);

	void SetAspectRatio(SDL_Window* Window);
	float GetAspectRatio() { return this->AspectRatio; };

	void AddNewFramebuffer(std::string FBName, const char* ShaderName);
	void UseFramebuffer(std::string name);
	MyFrameBuffer GetFramebuffer(std::string name);
	void ClearFramebuffers();

	void AddNewLightFramebuffer(std::shared_ptr<CLightComponent> light, int size);
	void UseLightFramebuffer(std::string name);
	MyLightFramebuffer GetLightFrameBuffer(std::string name);
	void FinalDraw();
	void ProcessLight(std::shared_ptr<CLightComponent> light,int i);
	void PostProcessLight(std::shared_ptr<CLightComponent> light, int count);

	Shaders GetShadersClass();


private:
	SDL_GLContext _Context;
	Shaders Shaders;
	int WindowW;
	int WindowH;
	float AspectRatio;
	std::vector<MyFrameBuffer> Framebuffers;
	std::vector<MyLightFramebuffer> LightFramebuffers;

	GLuint FinalVao;
	GLuint FinalVbo;

	glm::mat4 ViewMatrix;


};

