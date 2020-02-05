#include "pch.h"
#include "CObject2D.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <fstream>
#include "SDL_image.h"
#include "CLog.h"
#include "Primitives.h"
#include "CLayout.h"


CObject2D::CObject2D(const char* name, glm::vec2 position, glm::vec2 objsize,CLayout* ref)
	:_Name(std::string(name)), _Position(position), _Size(objsize), Layer(0), ObjectAligment(Object2DAligment::LEFT_TOP),
	_LocalOffset(0.f), ModelMatrix(glm::mat4(1.0f)), _VAO(0), _Rotation(glm::vec3(0.f)), ParrentObject(nullptr), ColorMask(1.0f)
{
	this->RefreshModelMatrix();
	this->SetLayoutReference(ref);
};

CObject2D::~CObject2D()
{
	CLog::MyLog(LogType::Log, "Object2DDestructor %s",this->_Name);
}

void CObject2D::Prepare()
{
	
	glGenVertexArrays(1, &this->_VAO);
	glBindVertexArray(this->_VAO);

	GLuint VBO[2];
	glGenBuffers(2, VBO);
	GLuint EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Plane_Indices), Plane_Indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Plane_Vertices), Plane_Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(TexCords), TexCords, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Plane_TexCords), Plane_TexCords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void CObject2D::RefreshModelMatrix()
{
	mat4 Translation = translate(mat4(), vec3(this->_Position.x + (this->_LocalOffset.x), this->_Position.y + (this->_LocalOffset.y), (float)Layer));
	mat4 Scaling = scale(vec3(this->_Size.x, this->_Size.y, 1.0f));
	mat4 RotationX = rotate(radians(this->_Rotation.x), vec3(1.0f, 0.0f, 0.0f));
	mat4 RotationY = rotate(radians(this->_Rotation.y), vec3(0.0f, 1.0f, 0.0f));
	mat4 RotationZ = rotate(radians(this->_Rotation.z), vec3(0.0f, 0.0f, 1.0f));
	mat4 Rotation = RotationX * RotationY *RotationZ;
	this->ModelMatrix = Translation * Rotation* Scaling;

}

void CObject2D::SetPosition(vec2 vec)
{
	if (this->ParrentObject == nullptr)
	{
		this->_Position.x = vec.x;
		this->_Position.y = vec.y;
	}
	else
	{
		this->_Position.x = this->ParrentObject->_Position.x + vec.x;
		this->_Position.y = this->ParrentObject->_Position.y + vec.y;
	}
	this->RefreshModelMatrix();
}

void CObject2D::SetRotation(vec3 vec)
{
	if (this->ParrentObject == nullptr)
	{
		this->_Rotation = vec;

	}
	else
	{
		this->_Rotation = this->ParrentObject->_Rotation + vec;
	}
	this->RefreshModelMatrix();
}

void CObject2D::SetSize(vec2 vec)
{
	this->_Size = vec;
	switch (this->ObjectAligment)
	{
		case LEFT_TOP:
		{
			this->_LocalOffset.x = 0;
			this->_LocalOffset.y = 0;
			break;
		}
		case TOP:
		{
			this->_LocalOffset.x = -this->_Size.x/2;
			this->_LocalOffset.y = 0;
			break;
		}
		case RIGHT_TOP:
		{
			this->_LocalOffset.x = -this->_Size.x;
			this->_LocalOffset.y = 0;
			break;
		}
		case LEFT:
		{
			this->_LocalOffset.x = 0;
			this->_LocalOffset.y = this->_Size.y/2;
			break;
		}
		case CENTER:
		{
			this->_LocalOffset.x = -this->_Size.x/2;
			this->_LocalOffset.y = this->_Size.y/2;
			break;
		}
		case RIGHT:
		{
			this->_LocalOffset.x = -this->_Size.x;
			this->_LocalOffset.y = this->_Size.y/2;
			break;
		}
		case LEFT_BOTTOM:
		{
			this->_LocalOffset.x = 0;
			this->_LocalOffset.y = this->_Size.y;
			break;
		}
		case BOTTOM:
		{
			this->_LocalOffset.x = -this->_Size.x/2;
			this->_LocalOffset.y = this->_Size.y;
			break;
		}
		case RIGHT_BOTTOM:
		{
			this->_LocalOffset.x = -this->_Size.x;
			this->_LocalOffset.y = this->_Size.y;
			break;
		}
		default:
			break;
	}
	this->RefreshModelMatrix();
}

void CObject2D::SetAligment(Object2DAligment Aligment)
{
	this->ObjectAligment = Aligment;
	this->SetSize(this->_Size);
}

void CObject2D::BindParrentObject(CObject2D* obj)
{
	this->ParrentObject = obj;
	obj->AddToParrentOfTable(this);
	this->SetPosition(this->_Position);
}

CObject2D* CObject2D::GetParrentObject() const
{
	return this->ParrentObject;
}

void CObject2D::AddToParrentOfTable(CObject2D* obj)
{
	this->ParrentOf.push_back(obj);
}

std::vector<CObject2D*> CObject2D::GetParentOfTable() const
{
	return this->ParrentOf;
}

vec3 CObject2D::GetForwardVector() const
{
	glm::vec3 FV;
	FV.x = this->_Position.x + (cos(glm::radians(this->_Rotation.x))*cos(glm::radians(this->_Rotation.y)));
	FV.y = this->_Position.y + sin(glm::radians(this->_Rotation.x));
	FV.z = this->Layer + (cos(glm::radians(this->_Rotation.x))*sin(glm::radians(this->_Rotation.y)));
	return FV;
}

void CObject2D::MoveObjectLayerDown()
{
	if (this->Layer <= 0)
	{
		this->Layer--;

	}
}

void CObject2D::MoveObjectLayerUp()
{
	if (this->Layer >= -255)
	{
		this->Layer++;
	}
}

void CObject2D::SetObjectLayer(int Layer)
{
	if (this->Layer >= -255 && this->Layer <= 0)
	{
		this->Layer = -Layer;
	}
}

int CObject2D::GetObjectLayer()
{
	return this->Layer;
}

void CObject2D::SetVisibility(bool visibility)
{
	this->_IsVisible = visibility;
}

bool CObject2D::GetVisibility() const
{
	return this->_IsVisible;
}

void CObject2D::SetIsActive(bool active)
{
	this->_IsActive = active;
}

bool CObject2D::GetIsActive() const
{
	return this->_IsActive;
}

void CObject2D::SetColor(vec3 color)
{
	this->ColorMask = vec4(color, this->ColorMask.a);
}

void CObject2D::SetAlpha(float alpha)
{
	this->ColorMask = vec4(vec3(this->ColorMask), alpha);
}

void CObject2D::SetColorMask(vec4 ColorMask)
{
	this->ColorMask = ColorMask;
}

vec4 CObject2D::GetColorMask() const
{
	return this->ColorMask;
}

void CObject2D::Tick(double delta)
{
	if(this->TickFunc != nullptr)
	this->TickFunc(delta);
}

void CObject2D::SetName(const char* name)
{
	this->_Name = name;
}

const char* CObject2D::GetName() const
{
	return this->_Name.c_str();
}

void CObject2D::SetID(int id)
{
	this->_ID = id;
}

int CObject2D::GetID() const
{
	return this->_ID;
}

mat4 CObject2D::GetModelMatrix()
{
	return this->ModelMatrix;
}

void CObject2D::BindTickFunction(std::function<void(double)> func)
{
	this->TickFunc = func;
}

void CObject2D::SetLayoutReference(CLayout* Layout)
{
	this->LayoutRef = Layout;
}
