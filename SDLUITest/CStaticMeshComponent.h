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
	void Draw() override;


private:

	std::shared_ptr<Model> _Model;
	

};

