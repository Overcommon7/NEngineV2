#include "Precompiled.h"
#include "Components/AnimatorComponent.h"

#include "GameObject.h"
#include "Components/ModelComponent.h"

#include "GameWorld.h"
#include "Services/UpdateService.h"

namespace NEngine
{
	void AnimatorComponent::Initialize()
	{
		ModelComponent* modelComponent = GetOwner().GetComponent<ModelComponent>();
		mAnimator.Initialize(modelComponent->GetModelId());

		GetOwner().GetWorld().GetService<UpdateService>()->Register(this);
	}
	void AnimatorComponent::Terminate()
	{
		GetOwner().GetWorld().GetService<UpdateService>()->Unregister(this);
	}
	void AnimatorComponent::Update(float deltaTime)
	{
		mAnimator.Update(deltaTime);
	}
	void AnimatorComponent::DebugUI()
	{
		mAnimator.DebugUI();
	}
	bool AnimatorComponent::Play(int index, bool looping)
	{
		mAnimator.PlayAnimation(index, looping);

		return false;
	}
}