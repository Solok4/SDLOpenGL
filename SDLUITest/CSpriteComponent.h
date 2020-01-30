#pragma once

#include "IDraw.h"
#include "CBaseComponent.h"

enum SpriteBehaviour
{
	KEEP_ROTATION = 0,
	ALWAYS_LOOK_AT_CAMERA,
	EIGHT_DIRECTIONS
};

enum SpriteEightDirections
{
	FORWARD=0,
	FR,
	R,
	BR,
	BACKWARD,
	BL,
	L,
	FL
};

class CSpriteComponent :
	public CBaseComponent,IDraw
{
public:
	CSpriteComponent();
	~CSpriteComponent();

	void PreDraw();
	virtual void Draw(DrawType dt);
	void SetSpriteBehaviour(SpriteBehaviour behaviour) { this->_SpBehaviour = behaviour; };

private:
	SpriteBehaviour _SpBehaviour;
	SpriteEightDirections _SpDirection;
	GLuint _SpritePlane;
};

