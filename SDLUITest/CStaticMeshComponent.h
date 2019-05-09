#pragma once
#include "CBaseComponent.h"
#include "CModelManager.h"

class CStaticMeshComponent :
	public CBaseComponent
{
public:
	CStaticMeshComponent();
	~CStaticMeshComponent();

	void BindModel(std::shared_ptr<Model> model);
	std::shared_ptr<Model> GetModel();
	void Draw(int program) override;


private:

	std::shared_ptr<Model> _Model;
};

