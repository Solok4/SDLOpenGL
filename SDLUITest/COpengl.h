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
	void FinalDraw();

	void ProcessLight(std::vector<std::shared_ptr<CLightComponent>> lights);

	Shaders GetShadersClass();


private:
	SDL_GLContext _Context;
	Shaders Shaders;
	int WindowW;
	int WindowH;
	float AspectRatio;
	std::vector<MyFrameBuffer> Framebuffers;

	GLuint FinalVao;
	GLuint FinalVbo;

	glm::mat4 ViewMatrix;


};

