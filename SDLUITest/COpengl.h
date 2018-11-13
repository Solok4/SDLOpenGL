#pragma once


#include "SDL.h"
#include <vector>
#include "GL/glew.h"
#include "glm.hpp"
#include "gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")


class COpengl
{
public:
	COpengl();
	~COpengl();

	bool Create(SDL_Window* Window);
	void Delete();

	void CreateShader(const char* File,bool IsVertex);

	std::string ReadShaderFromFile(const char* Filename);

	void CreateShaderProgram(int VertexID, int FragmentID);

	void PrepareToLoop();
	void PreLoop();
	void SetModelMatrix(glm::mat4 matrix);

	void ProLoop(SDL_Window* Window);
	void PreLoopPerspective();
	void PreLoopOrtho(SDL_Window* Window);

	void SetCameraPosition(glm::vec3 Pos);

	void SelectShaderProgram(int number);
	

private:
	SDL_GLContext _Context;

	GLuint CurrentShaderProgram;

	std::vector<unsigned int> VertexShaders;
	std::vector<unsigned int> FragmentShaders;
	std::vector<unsigned int> ShaderProgram;

	glm::mat4 Projection = glm::mat4(1.0f);
	glm::mat4 ViewMatrix = glm::mat4(1.0f);

};

