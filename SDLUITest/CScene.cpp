#include "pch.h"
#include "CScene.h"
#include "COpengl.h"



CScene::CScene()
{
	this->Camera = nullptr;
	this->MovementObject = nullptr;
}


CScene::~CScene()
{
	CLog::MyLog(LogType::Log, "SceneDestructor %s",this->GetName().c_str());
}

std::shared_ptr<CObject3D> CScene::AddObjectToScene(std::string Name)
{
	std::shared_ptr<CObject3D> temp=std::make_shared<CObject3D>();
	temp->SetName(Name);
	this->Objects3D.push_back(temp);
	this->DrawableCached = false;
	return temp;
}

std::shared_ptr<CObject3D> CScene::AddObjectToScene(std::string Name, std::shared_ptr<CObject3D> &obj)
{
	std::shared_ptr<CObject3D> temp = std::make_shared<CObject3D>(obj);
	temp->SetName(Name);
	this->Objects3D.push_back(temp);
	this->DrawableCached = false;
	return temp;
}

std::shared_ptr<CObject3D> CScene::GetObjectByName(std::string Name)
{
	for (auto o : this->Objects3D)
	{
		if (o->GetName() == Name)
		{
			return o;
		}
	}

	return nullptr;
}

void CScene::RemoveObjectFromScene(std::string Name)
{
	for (unsigned int i = 0; i < this->Objects3D.size(); i++)
	{
		if (this->Objects3D[i]->GetName() == Name)
		{
			this->Objects3D.erase(this->Objects3D.begin()+i);
			this->DrawableCached = false;
			break;
		}
	}
}

void CScene::AddLightToScene(std::shared_ptr<CObject3D> light)
{
	if (light != nullptr)
	{
		if(light->GetComponentByType(Object3DComponent::LIGHT_COMPONENT) != nullptr)
			this->Lights.push_back(std::dynamic_pointer_cast<CLightComponent>(light->GetComponentByType(Object3DComponent::LIGHT_COMPONENT)));
	}
}

void CScene::AddLightToScene(std::string Name)
{
	for (auto o : this->Objects3D)
	{
		if (o->GetName() == Name)
		{
			if (o->GetComponentByType(Object3DComponent::LIGHT_COMPONENT) != nullptr)
			{
				this->Lights.push_back(std::dynamic_pointer_cast<CLightComponent>(o->GetComponentByType(Object3DComponent::LIGHT_COMPONENT)));
			}
		}
	}
}

void CScene::RemoveLightFromScene(std::shared_ptr<CObject3D> light)
{
	if (light != nullptr)
	{
		if (light->GetComponentByType(Object3DComponent::LIGHT_COMPONENT) != nullptr)
		{
			for (unsigned int i = 0; i < this->Lights.size(); i++)
			{
				if (this->Lights[i]->GetName() == light->GetName())
				{
					this->Lights.erase(this->Lights.begin() + i);
					break;
				}
			}
		}
	}
	CScene::RemoveObjectFromScene(light->GetName());
}

void CScene::RemoveLightFromScene(std::string Name)
{
	for (unsigned int i = 0; i < this->Lights.size(); i++)
	{
		if (this->Lights[i]->GetName() == Name)
		{
			this->Lights.erase(this->Lights.begin() + i);
			break;
		}
	}
	CScene::RemoveObjectFromScene(Name);
}

std::vector<std::shared_ptr<CLightComponent>> CScene::GetLightObjects()
{
	std::vector < std::shared_ptr < CLightComponent >> EnabledLights;
	for (auto a : this->Lights)
	{
		if (a->IsActive())	EnabledLights.push_back(a);
	}
	return EnabledLights;
}


void CScene::SetName(std::string Name)
{
	this->Name = Name;
}

std::string CScene::GetName()
{
	return this->Name;
}

void CScene::Draw(DrawType DType)
{
	if (this->DrawableCached)
	{
		for (auto c : this->ObjectsToDraw)
		{
			auto sm = std::dynamic_pointer_cast<CStaticMeshComponent>(c);
			if (sm != nullptr)
			{
				if (DType == DrawType::FullDraw)
				{
					sm->CalculateMatrix();
					OpenGL->SetModelMatrix(sm->GetModelMatrix());
					OpenGL->SetNormalMatrix(sm->GetModelMatrix());
					auto material = sm->GetModel()->Mat;
					glUniform3f(OpenGL->GetShadersClass().GetUniformByNameStruct("Default", "Mat.Ambient"),
						material->LM.Ambient.x, material->LM.Ambient.y, material->LM.Ambient.z);
					glUniform3f(OpenGL->GetShadersClass().GetUniformByNameStruct("Default", "Mat.Diffuse"),
						material->LM.Diffuse.x, material->LM.Diffuse.y, material->LM.Diffuse.z);
					glUniform3f(OpenGL->GetShadersClass().GetUniformByNameStruct("Default", "Mat.Specular"),
						material->LM.Specular.x, material->LM.Specular.y, material->LM.Specular.z);
					glUniform1f(OpenGL->GetShadersClass().GetUniformByNameStruct("Default", "Mat.Shininess"),
						material->LM.Shininess);
					c->Draw(DType);
				}
				else if (DType == DrawType::VerticesOnly)
				{
					sm->CalculateMatrix();
					OpenGL->SetModelMatrix(sm->GetModelMatrix());
					c->Draw(DType);
				}
			}

		}
	}
	else
	{
		this->CacheObjectsToDraw();
	}

}

void CScene::Tick(double delta)
{
	for (auto o : this->Objects3D)
	{
		o->Tick(delta);
	}
}

void CScene::SetSkyBox(SkyboxType type, Texture texture)
{
	glGenVertexArrays(1, &this->SkyboxVAO);
	glBindVertexArray(this->SkyboxVAO);
	glGenBuffers(2, this->SkyboxVBOs);

	if (type == SkyboxType::CubeType)
	{
		this->Skyboxtype = type;
		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, this->SkyboxVBOs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glDisableVertexAttribArray(0);
	}
}

void CScene::Prepare()
{
}

void CScene::SetCamera(std::shared_ptr<CCameraComponent> Cam)
{
	if (Cam != nullptr)
	{
		this->Camera = Cam;
		return;
	}
	CLog::MyLog(LogType::Error, "Camera not bound to scene: %s ", this->Name.c_str());
}

void CScene::SetCamera(std::shared_ptr<CObject3D> Cam)
{
	std::shared_ptr<CCameraComponent> component = std::dynamic_pointer_cast<CCameraComponent>(Cam->GetComponentByType(Object3DComponent::CAMERA_COMPONENT));
	if (component == nullptr)
	{
		CLog::MyLog(LogType::Error, "This object don't have camera component in it. Object: %s", Cam->GetName().c_str());
		return;
	}
	this->Camera = component;
}

std::shared_ptr<CCameraComponent> CScene::GetCamera()
{
	return this->Camera;
}

void CScene::SetMovementObject(std::shared_ptr<CMovementComponent> Movement)
{
	if (Movement != nullptr)
	{
		this->MovementObject = Movement;
		return;
	}
	CLog::MyLog(LogType::Error, "Movement not bound to scene: %s ", this->Name.c_str());
}

void CScene::SetMovementObject(std::shared_ptr<CObject3D> Movement)
{
	std::shared_ptr<CMovementComponent> component = std::dynamic_pointer_cast<CMovementComponent>(Movement->GetComponentByType(Object3DComponent::MOVEMENT_COMPONENT));
	if (component == nullptr)
	{
		CLog::MyLog(LogType::Error, "This object don't have movement component in it. Object: %s", Movement->GetName().c_str());
		return;
	}
	this->MovementObject = component;
}

std::shared_ptr<CMovementComponent> CScene::GetMovementObject()
{
	return this->MovementObject;
}

void CScene::CacheObjectsToDraw()
{
	for (auto o : this->Objects3D)
	{
		auto list = o->GetComponentList();
			for (auto c : list)
			{
				std::shared_ptr<IDraw> drawable = std::dynamic_pointer_cast<IDraw>(c);
				if (drawable == nullptr)
				{
					continue;
				}
				else
				{
					this->ObjectsToDraw.push_back(drawable);
				}
			}
	}
	this->DrawableCached = true;
}
