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
#include "CRenderer.h"

#define MAX_LIGHTS 8
#define SHADOWMAP_SIZE 128
#define FARPLANE 50.0f

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")

struct MyFrameBuffer
{
	std::string name;
	bool isDepth = false;
	GLuint FBO=0;
	//GLuint DBO = 0;
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

	//Create context, initalize glew.
	bool Create(SDL_Window* Window);
	//Clear context and all framebuffers.
	void Delete();

	//Load shaders and shader uniforms.
	void PrepareToLoop();
	//Clear color and depth buffers.
	void PreLoop();
	//Set model matrix to scene rendering shader uniform.
	void SetModelMatrix(glm::mat4 matrix);
	////Set model matrix to layout rendering shader uniform.
	//void SetModelMatrixLayout(glm::mat4 matrix);
	//Set colour of layout element.
	void SetColorMaskLayout(glm::vec3 ColorMask);
	//Sends inverted and transposed normal matrix to scene rendering shader.
	void SetNormalMatrix(glm::mat4 matrix);

	//Being called after rendering all the things, swaps video buffers.
	void ProLoop(SDL_Window* Window);
	//Prepares view and perspective for scene rendering.
	void PreLoopPerspective(std::shared_ptr<CCameraComponent> Camera);
	//Prepares view and perspective for layout rendering.
	void PreLoopOrtho();

	//void SetAspectRatio(SDL_Window* Window);

	//Return screen aspect ratio.
	float GetAspectRatio() { return this->WndInfo->ScreenAspectRatio; };
	//Adds new framebuffer for drawing.
	void AddNewFramebuffer(std::string FBName, const char* ShaderName, bool isDepthbuffer = false);
	//Changes current drawing framebuffer.
	void UseFramebuffer(std::string name);
	//Returns framebuffer struct.
	MyFrameBuffer GetFramebuffer(std::string name);
	//Clears all framebuffers including the light ones.
	void ClearAllFramebuffers();

	void ClearFramebuffers();

	void ClearLightFramebuffers();

	void DrawDebugLights(std::vector<std::shared_ptr<CLightComponent>> list, std::shared_ptr<CCameraComponent> camera);

	//Creates new framebuffer that is used to determine distances from the light source to the nearby objects. Size means framebuffer size.
	void AddNewLightFramebuffer(std::shared_ptr<CLightComponent> light, int size);
	//Changes current drawing light framebuffer.
	void UseLightFramebuffer(std::string name);
	//Returns light framebuffer.
	MyLightFramebuffer GetLightFrameBuffer(std::string name);
	/*
	Final draw of the scene. The engine works that way it draws scene in separate framebuffer and in the final step it's drawn on a plane that covers whole screen area.
	It allows for postprocessing, dynamic resolution scaling and other stuff.
	*/
	void FinalDraw();
	//Takes light reference, fills all uniform needed for light calculations, binds proper light framebuffer.
	void ProcessLight(std::shared_ptr<CLightComponent> light,int i);
	//Takes light reference, light framebuffer and adds it to main drawing shader to create shadows.
	void PostProcessLight(std::shared_ptr<CLightComponent> light, int count);
	//returns shader class.
	Shaders GetShadersClass();


private:

	SDL_GLContext _Context;
	Shaders Shaders;
	std::shared_ptr<WindowInfo> WndInfo;
	std::vector<MyFrameBuffer> Framebuffers;
	std::vector<MyLightFramebuffer> LightFramebuffers;
	GLuint FinalVao;
	GLuint FinalVbo;
	//GLuint FinalEbo;
	//GLuint FinalVbo[2];
	glm::mat4 ViewMatrix;
};

extern std::unique_ptr<COpengl> OpenGL;

