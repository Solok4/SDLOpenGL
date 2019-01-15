#pragma once


#include "SDL.h"
#include <vector>
#include "GL/glew.h"
#include "glm.hpp"
#include "gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"
#include "CCameraComponent.h"
#include "Shaders.h"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")

struct MyFrameBuffer
{
	std::string name;
	GLuint FBO;
	GLuint RBO;
	GLuint CBuffer;
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

	void ProLoop(SDL_Window* Window);
	void PreLoopPerspective(std::shared_ptr<CCameraComponent> Camera);
	void PreLoopOrtho(SDL_Window* Window);

	void SetAspectRatio(SDL_Window* Window);
	float GetAspectRatio() { return this->AspectRatio; };

	void AddNewFramebuffer(std::string name);
	void UseFramebuffer(std::string name);
	MyFrameBuffer GetFramebuffer(std::string name);
	void ClearFramebuffers();
	void FinalDraw();


private:
	SDL_GLContext _Context;
	Shaders Shaders;
	int WindowW;
	int WindowH;
	float AspectRatio;
	std::vector<MyFrameBuffer> Framebuffers;

	GLuint FinalVao;
	GLuint FinalVbo;

};

