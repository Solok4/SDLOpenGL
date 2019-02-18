#pragma once

#include <vector>
#include <map>
#include <memory>
#include "glm/glm.hpp"
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

	void BindRootObject(std::shared_ptr<CObject2D> obj);
	vec3 GetForwardVector();

	void MoveObjectLayerDown();
	void MoveObjectLayerUp();
	void SetObjectLayer(int Layer);
	int GetObjectLayer();

	void Tick(uint32_t delta);

	void LoadTexture(const char* str, const char* name = "Default");
	GLuint GetTexture(const char* name = "Default");
	void BindTexture(GLuint Tex);

	void SetName(const char* name);
	const char* GetName();
	void SetID(int id);
	int GetID();

	mat4 GetModelMatrix();


protected:

	int _ID;
	vec2 _Position;
	vec2 _Size;
	vec3 _Rotation;
	vec2 _Margin;

	std::shared_ptr<CObject2D> RootObject;

	int Layer;
	bool _IsActive = true;
	bool _IsVisible = true;

	const char* _Name;

	mat4 ModelMatrix;

	std::map<const char*, GLuint> Textures;
	GLuint TextureID;

	GLuint _VAO;
	std::vector<GLuint> _VBO;
};