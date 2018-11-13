#pragma once

#include "glm.hpp"
#include <vector>
#include "CBaseComponent.h"
using namespace glm;
using namespace std;

class CBaseComponent;

class CObject3D
{
public:
	CObject3D(unsigned int ID);
	~CObject3D();

	virtual void Prepare();
	void PreDraw();
	void SetPosition(vec3 vec);
	void SetRotation(vec3 vec);
	void SetScale(vec3 vec);
	virtual void Draw();
	virtual void Free();


protected:

	unsigned int _ID;
	vec3 _Position;
	vec3 _Scale;
	vec3 _Rotation;

	float CameraDistance;
	bool PossesCamera = false;

	mat4 ModelMatrix;

	vector<CBaseComponent> _ComponentRef;

	unsigned int _VAO;
};

