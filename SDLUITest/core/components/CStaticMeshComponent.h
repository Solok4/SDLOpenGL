#pragma once
#include "CBaseComponent.h"
#include "../managers/CModelManager.h"
#include "../renderer/IDraw.h"
#include "../shared/Shared.h"

class CStaticMeshComponent :
	public CBaseComponent, public IDraw
{
public:
	CStaticMeshComponent(CObject3D* ref);
	CStaticMeshComponent(const CStaticMeshComponent& mesh, CObject3D* ref);
	//CStaticMeshComponent(const std::shared_ptr<CStaticMeshComponent>& mesh);
	~CStaticMeshComponent();

	void BindModel(std::shared_ptr<Model> model);
	std::shared_ptr<Model> GetModel();
	void Draw(RenderStep step) override;
	void SetCastShadow(bool value) { this->_CastShadow = value; };
	bool GetCastShadow() { return this->_CastShadow; };

private:

	std::shared_ptr<Model> _Model;
	bool _CastShadow;
};
