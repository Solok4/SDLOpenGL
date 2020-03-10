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
	CLog::MyLog(LogType::Debug, "SceneDestructor %s",this->GetName().c_str());
	this->Camera = nullptr;
	this->Lights.clear();
	this->MovementObject = nullptr;
	this->ObjectsToDraw.clear();
	this->Objects3D.clear();
}

std::shared_ptr<CObject3D> CScene::AddObjectToScene(std::string Name)
{
	std::shared_ptr<CObject3D> temp=std::make_shared<CObject3D>(Name,this);
	//temp->SetName(Name);
	this->Objects3D.push_back(temp);
	this->DrawableCached = false;
	return temp;
}

std::shared_ptr<CObject3D> CScene::AddObjectToScene(std::string Name, std::shared_ptr<CObject3D> &obj)
{
	std::shared_ptr<CObject3D> temp = std::make_shared<CObject3D>(*obj,Name.c_str(),this);
	//temp->SetName(Name);
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
	if (!this->DrawableCached)
	{
		this->CacheObjectsToDraw();
	}
	if (DType == DrawType::FullDraw)
	{
		if (OpenGL->GetRenderMode() == RenderMode::RenderModeDeferred)
		{
			for (int i = 0; i < RenderStep::RenderStepsMAX; i++)
			{
				if (i == RenderStep::RenderDeferred)
				{
					OpenGL->UseFramebuffer("DeferredShading");
					OpenGL->PreLoopPerspective(this->Camera);
					for (auto c : this->ObjectsToDraw)
					{
						auto sm = std::dynamic_pointer_cast<CStaticMeshComponent>(c);
						if (sm != nullptr)
						{
							sm->CalculateMatrix();
							OpenGL->SetModelMatrix(sm->GetModelMatrix());
							OpenGL->SetNormalMatrix(sm->GetModelMatrix());
							c->Draw((RenderStep)i);
						}
					}
				}
				else if (i == RenderStep::RenderLight)
				{
					this->ProcessLights();
					glViewport(0, 0, Renderer->GetWindowInfo()->ScreenWidth, Renderer->GetWindowInfo()->ScreenHeight);
					//OpenGL->UseFramebuffer("LightPass");
					//OpenGL->GetShadersClass().Uniform3f(this->Camera->GetPosition(),"Camera");
					//OpenGL->GetShadersClass().Uniform1i(this->Lights.size(),"LightCount");
					//OpenGL->PreLoopPerspective(this->Camera);
					//auto basemap = OpenGL->GetFramebuffer("DeferredShading");

					////glActiveTexture(GL_TEXTURE0);
					//glBindTexture(GL_TEXTURE_2D, basemap.Deferred[0]);
					//OpenGL->GetShadersClass().Uniform1i(0, "gDiffuse");

					////glActiveTexture(GL_TEXTURE1);
					//glBindTexture(GL_TEXTURE_2D, basemap.Deferred[1]);
					//OpenGL->GetShadersClass().Uniform1i(1, "gNormal");

					////glActiveTexture(GL_TEXTURE2);
					//glBindTexture(GL_TEXTURE_2D, basemap.Deferred[2]);
					//OpenGL->GetShadersClass().Uniform1i(2, "gSpecular");
					//for (auto c : this->ObjectsToDraw)
					//{
					//	auto sm = std::dynamic_pointer_cast<CStaticMeshComponent>(c);
					//	if (sm != nullptr)
					//	{
					//		sm->CalculateMatrix();
					//		OpenGL->SetModelMatrix(sm->GetModelMatrix());
					//		OpenGL->SetNormalMatrix(sm->GetModelMatrix());
					//		c->Draw(RenderStep::RenderJustBuffers);
					//	}
					//}
				}
			}
		}
		else
		{
			this->ProcessLights();
			glViewport(0, 0, Renderer->GetWindowInfo()->ScreenWidth, Renderer->GetWindowInfo()->ScreenHeight);
			OpenGL->UseFramebuffer("Default");
			OpenGL->GetShadersClass().Uniform1i(this->Lights.size(), "LightCount");
			OpenGL->PreLoopPerspective(this->Camera);
			if (!this->DrawableCached)
			{
				this->CacheObjectsToDraw();
			}
			for (auto a : this->ObjectsToDraw)
			{
				auto sm = std::dynamic_pointer_cast<CStaticMeshComponent>(a);
				if (sm != nullptr)
				{
					sm->CalculateMatrix();
					OpenGL->SetModelMatrix(sm->GetModelMatrix());
					OpenGL->SetNormalMatrix(sm->GetModelMatrix());
					OpenGL->GetShadersClass().Uniform3f(sm->GetModel()->Mat->GetLightMaterial()->Ambient, "Mat.Ambient");
					OpenGL->GetShadersClass().Uniform3f(sm->GetModel()->Mat->GetLightMaterial()->Diffuse, "Mat.Diffuse");
					OpenGL->GetShadersClass().Uniform3f(sm->GetModel()->Mat->GetLightMaterial()->Specular, "Mat.Specular");	
					OpenGL->GetShadersClass().Uniform1f(sm->GetModel()->Mat->GetLightMaterial()->Shininess, "Mat.Shininess");
					a->Draw(RenderStep::RenderDeferred);
				}
			}
		}
	}
	else if (DType == DrawType::VerticesOnly)
	{
		for (auto c : this->ObjectsToDraw)
		{
			auto sm = std::dynamic_pointer_cast<CStaticMeshComponent>(c);
			if (sm != nullptr)
				if (sm->GetCastShadow())
				{
					sm->CalculateMatrix();
					OpenGL->SetModelMatrix(sm->GetModelMatrix());
					c->Draw(RenderVerticesOnly);
				}
		}
	}

}

void CScene::Tick(double delta)
{
	for (auto o : this->Objects3D)
	{
		o->Tick(delta);
	}
}

//void CScene::SetSkyBox(SkyboxType type, Texture texture)
//{
//	glGenVertexArrays(1, &this->SkyboxVAO);
//	glBindVertexArray(this->SkyboxVAO);
//	glGenBuffers(2, this->SkyboxVBOs);
//
//	if (type == SkyboxType::CubeType)
//	{
//		this->Skyboxtype = type;
//		float skyboxVertices[] = {
//			// positions          
//			-1.0f,  1.0f, -1.0f,
//			-1.0f, -1.0f, -1.0f,
//			 1.0f, -1.0f, -1.0f,
//			 1.0f, -1.0f, -1.0f,
//			 1.0f,  1.0f, -1.0f,
//			-1.0f,  1.0f, -1.0f,
//
//			-1.0f, -1.0f,  1.0f,
//			-1.0f, -1.0f, -1.0f,
//			-1.0f,  1.0f, -1.0f,
//			-1.0f,  1.0f, -1.0f,
//			-1.0f,  1.0f,  1.0f,
//			-1.0f, -1.0f,  1.0f,
//
//			 1.0f, -1.0f, -1.0f,
//			 1.0f, -1.0f,  1.0f,
//			 1.0f,  1.0f,  1.0f,
//			 1.0f,  1.0f,  1.0f,
//			 1.0f,  1.0f, -1.0f,
//			 1.0f, -1.0f, -1.0f,
//
//			-1.0f, -1.0f,  1.0f,
//			-1.0f,  1.0f,  1.0f,
//			 1.0f,  1.0f,  1.0f,
//			 1.0f,  1.0f,  1.0f,
//			 1.0f, -1.0f,  1.0f,
//			-1.0f, -1.0f,  1.0f,
//
//			-1.0f,  1.0f, -1.0f,
//			 1.0f,  1.0f, -1.0f,
//			 1.0f,  1.0f,  1.0f,
//			 1.0f,  1.0f,  1.0f,
//			-1.0f,  1.0f,  1.0f,
//			-1.0f,  1.0f, -1.0f,
//
//			-1.0f, -1.0f, -1.0f,
//			-1.0f, -1.0f,  1.0f,
//			 1.0f, -1.0f, -1.0f,
//			 1.0f, -1.0f, -1.0f,
//			-1.0f, -1.0f,  1.0f,
//			 1.0f, -1.0f,  1.0f
//		};
//		glEnableVertexAttribArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, this->SkyboxVBOs[0]);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//		glDisableVertexAttribArray(0);
//	}
//}

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

void CScene::ProcessLights()
{
	for (int i=0; i<this->Lights.size();i++)
	{
		if (this->Lights[i]->IsActive())
		{
			OpenGL->ProcessLight(this->Lights[i], i);
			this->Draw(DrawType::VerticesOnly);
			OpenGL->PostProcessLight(this->Lights[i], i);
		}
	}
}
