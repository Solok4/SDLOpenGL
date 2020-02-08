#include "CSpriteComponent.h"
#include "CSceneManager.h"



CSpriteComponent::CSpriteComponent()
{
}


CSpriteComponent::~CSpriteComponent()
{
}

void CSpriteComponent::PreDraw()
{
	if (this->_SpBehaviour == SpriteBehaviour::KEEP_ROTATION)
	{
		return;
	}
	else if (this->_SpBehaviour == SpriteBehaviour::EIGHT_DIRECTIONS)
	{
		auto camera = SceneManager->GetCurrentScene()->GetCamera();

		float DotProduct = glm::dot(camera->GetRotation(), this->GetRotation());
	}
	else if (this->_SpBehaviour == SpriteBehaviour::ALWAYS_LOOK_AT_CAMERA)
	{
		auto camera = SceneManager->GetCurrentScene()->GetCamera();
		auto DesiredRotation = camera->GetRotation();
		DesiredRotation.y += 180.f;
		this->SetRotation(DesiredRotation);
	}
}

void CSpriteComponent::Draw(RenderStep step)
{
	
}
