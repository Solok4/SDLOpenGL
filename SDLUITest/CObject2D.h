#pragma once

#include <vector>
#include <map>
#include "glm.hpp"
#include "GL/glew.h"
using namespace glm;

class CObject2D
{
public:
	CObject2D();
	~CObject2D();

	virtual void Prepare();
	void RefreshModelMatrix();
	void SetPosition(vec2 vec);
	void SetRotation(vec3 vec);
	void SetSize(vec2 vec);

	virtual void PreDraw();
	virtual void Draw();
	virtual void PostDraw();

	void MoveObjectLayerDown();
	void MoveObjectLayerUp();
	void SetObjectLayer(int Layer);
	int GetObjectLayer();

	void LoadTexture(const char* str, std::string name = "Default");
	GLuint GetTexture(std::string name = "Default");
	void BindTexture(GLuint Tex);

	void SetName(std::string name);
	std::string GetName();
	void SetID(int id);
	int GetID();

	mat4 GetModelMatrix();


protected:

	int _ID;
	vec2 _Position;
	vec2 _Size;
	vec3 _Rotation;
	vec2 _Margin;

	int Layer;
	bool _IsActive = true;
	bool _IsVisible = true;

	std::string _Name;

	mat4 ModelMatrix;

	std::map<std::string, GLuint> Textures;
	GLuint TextureID;

	GLuint _VAO;
	std::vector<GLuint> _VBO;
};