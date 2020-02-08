#pragma once
#include "CBaseComponent.h"
#include "CModelManager.h"
#include "IDraw.h"
#include "Shared.h"

class CStaticMeshComponent :
	public CBaseComponent,public IDraw
{
public:
	CStaticMeshComponent();
	CStaticMeshComponent(const CStaticMeshComponent& mesh);
	CStaticMeshComponent(const std::shared_ptr<CStaticMeshComponent>& mesh);
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

