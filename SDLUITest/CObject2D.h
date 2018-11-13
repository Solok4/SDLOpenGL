#pragma once

#include <vector>
#include "glm.hpp"
#include "GL/glew.h"
using namespace glm;

class CObject2D
{
public:
	CObject2D();
	~CObject2D();

	virtual void Prepare();
	void PreDraw();
	void SetPosition(vec2 vec);
	void SetRotation(vec3 vec);
	void SetSize(vec2 vec);
	virtual void Draw();
	virtual void Free();

	mat4 GetModelMatrix();


protected:

	unsigned int _ID;
	vec2 _Position;
	vec2 _Size;
	vec3 _Rotation;

	vec2 _Margin;
	bool _IsActive = true;
	bool _IsVisible = true;

	mat4 ModelMatrix;
	unsigned int VertexCount;

	GLuint _VAO;
	std::vector<GLuint> _VBO;
};

