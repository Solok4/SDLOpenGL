#pragma once

enum DrawType
{
	FullDraw = 0,
	VerticesOnly,
};

class IDraw
{
public:
	virtual void Draw(DrawType) = 0;
};