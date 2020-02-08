#pragma once

enum DrawType
{
	FullDraw = 0,
	VerticesOnly,
};
enum RenderStep
{
	RenderVerticesOnly = 0,
	RenderDiffuse,
	RenderNormal,
	RenderSpecular,
	RenderLight,
	RenderStepsMAX
};

class IDraw
{
public:
	virtual void Draw(RenderStep) = 0;
};