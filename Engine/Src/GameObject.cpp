#include "Precompiled.h"
#include "GameObject.h"

#include "Transform.h"

void NEngine::GameObject::Initialize()
{
	ASSERT(!mInitialized, "GameObject is already initialized");

	mInitialized = true;
	mUniqueId = ++sUniqueId;
	int hasTransform = 0;

	for (auto& component : mComponents)
	{
		component->Initialize();
		if (component->GetTypeId() == Transform::StaticGetTypeId())
			++hasTransform;
	}
		
	ASSERT(hasTransform == 1, "GameObject Can Only Have One Transform");

}

void NEngine::GameObject::Terminate()
{
	for (auto& component : mComponents)
		component->Terminate();
}

void NEngine::GameObject::DebugUI()
{
	for (auto& component : mComponents)
		component->DebugUI();
}
