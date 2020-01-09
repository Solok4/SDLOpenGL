#pragma once

#include <vector>
#include <map>
#include <memory>
#include "glm/glm.hpp"
#include "GL/glew.h"
#include <functional>
using namespace glm;

enum Object2DType
{
	OBJECT2D_LABEL = 0,
	OBJECT2D_IMAGE,
	OBJECT2D_BUTTON,
	OBJECT2D_CONTAINER,
};

enum Object2DAligment
{
	LEFT_TOP,
	TOP,
	RIGHT_TOP,
	LEFT,
	CENTER,
	RIGHT,
	LEFT_BOTTOM,
	BOTTOM,
	RIGHT_BOTTOM,
};

class CObject2D
{
public:
	CObject2D();
	~CObject2D();

	//Prepares geometry to being used by opengl.
	virtual void Prepare();
	//Recalculates model matrix using actual position, rotation and scale. 
	void RefreshModelMatrix();
	//Sets position object on the screen. If this object is a child of other object then the position is relative to parent object.
	void SetPosition(vec2 vec);
	//Sets rotation object on the screen. If this object is a child of other object then the rotation is relative to parent object.
	void SetRotation(vec3 vec);
	//Sets size object on the screen.
	void SetSize(vec2 vec);

	void SetAligment(Object2DAligment Aligment);

	//Returns vec2 position vector.
	vec2 GetPosition() { return this->_Position; };
	//returns vec2 rotation vector.
	vec2 GetRotaton() { return this->_Rotation; };
	//returns vec2 size vector.
	vec2 GetSize() { return this->_Size; };

	//Prepares object to draw.
	virtual void PreDraw();
	//Draws the object. Should be used after PreDraw func.
	virtual void Draw();
	//Cleans up things after drawing.
	virtual void PostDraw();

	//Binds the object to other one.
	void BindParrentObject(std::shared_ptr<CObject2D> obj);
	//Returns parent object.
	std::shared_ptr<CObject2D> GetParrentObject() const;

	//Adds other object as child of the object.
	void AddToParrentOfTable(std::shared_ptr<CObject2D> obj);
	//Returns list of child objects of this object.
	std::vector<std::shared_ptr<CObject2D>> GetParentOfTable() const;
	//Returns forward vector of the object.
	vec3 GetForwardVector() const;
	//Moves object one step front.
	void MoveObjectLayerDown();
	//Moves object one step back.
	void MoveObjectLayerUp();
	//Sets object layer. On default all elements are at 0 layer.
	void SetObjectLayer(int Layer);
	//Returns current object layer.
	int GetObjectLayer();
	//Sets visibility of the object.
	void SetVisibility(bool visibility);
	//Returns visibility state.
	bool GetVisibility() const;
	//Sets if element is active. It might be useful with buttons, when you want to show it, but disable it's ability to be clickable.
	void SetIsActive(bool active);
	//Returns current active state.
	bool GetIsActive() const;
	//Sets colour of the object.
	void SetColor(vec3 color);
	//Sets element opacity. 1 - full visible, 0- full transprarent.
	void SetAlpha(float alpha);
	//Sets colour and alpha in one go.
	void SetColorMask(vec4 ColorMask);
	//Returns colour and alpha as vec4.
	vec4 GetColorMask() const;
	//Function being called every frame.
	virtual void Tick(double delta);
	//Bind texture to the object.
	void BindTexture(GLuint Tex);
	//Set name of the object.
	void SetName(const char* name);
	//Returns name of the object.
	const char* GetName() const;
	//Sets element id. It's used to determine what kind of element it is.
	void SetID(int id);
	//Returns element ID.
	int GetID() const;
	//Returns model matrix.
	mat4 GetModelMatrix();
	//Binds func that will be called in tick func.
	void BindTickFunction(std::function<void(double)> func);


protected:

	int _ID;
	vec2 _Position;
	vec2 _Size;
	vec3 _Rotation;
	vec2 _LocalOffset;
	Object2DAligment ObjectAligment;
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
};