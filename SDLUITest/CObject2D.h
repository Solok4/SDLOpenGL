#pragma once

#include <vector>
#include <map>
#include <memory>
#include "glm/glm.hpp"
#include "GL/glew.h"
#include <functional>
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

	vec2 GetPosition() { return this->_Position; };
	vec2 GetRotaton() { return this->_Rotation; };
	vec2 GetSize() { return this->_Size; };

	virtual void PreDraw();
	virtual void Draw();
	virtual void PostDraw();

	void BindParrentObject(std::shared_ptr<CObject2D> obj);
	std::shared_ptr<CObject2D> GetParrentObject() const;
	//FIX ME
	void AddToParrentOfTable(std::shared_ptr<CObject2D> obj);
	std::vector<std::shared_ptr<CObject2D>> GetParentOfTable() const;
	//
	vec3 GetForwardVector() const;

	void MoveObjectLayerDown();
	void MoveObjectLayerUp();
	void SetObjectLayer(int Layer);
	int GetObjectLayer();

	void SetVisibility(bool visibility);
	bool GetVisibility() const;

	void SetIsActive(bool active);
	bool GetIsActive() const;

	void SetColor(vec3 color);
	void SetAlpha(float alpha);

	void SetColorMask(vec4 ColorMask);

	vec4 GetColorMask() const;

	virtual void Tick(double delta);

	void BindTexture(GLuint Tex);

	void SetName(const char* name);
	const char* GetName() const;

	void SetID(int id);
	int GetID() const;

	mat4 GetModelMatrix();

	void BindTickFunction(std::function<void(double)> func);


protected:

	int _ID;
	vec2 _Position;
	vec2 _Size;
	vec3 _Rotation;
	vec2 _Margin;
	vec2 _LocalOffset;

	std::shared_ptr<CObject2D> ParrentObject;
	std::vector<std::shared_ptr<CObject2D>> ParrentOf;

	int Layer;
	bool _IsActive = true;
	bool _IsVisible = true;

	std::function<void(double)> TickFunc = nullptr;

	vec4 ColorMask;

	const char* _Name;

	mat4 ModelMatrix;

	GLuint TextureID;

	GLuint _VAO;
	std::vector<GLuint> _VBO;
};