#include "Precompiled.h"
#include "Services/UpdateService.h"

#include "Component.h"

void NEngine::UpdateService::Update(float deltaTime)
{
	isUpdating = true;
	size_t size = mUpdateComponents.size();
	for (size_t i = 0; i < size; ++i)
		mUpdateComponents[i]->Update(deltaTime);

	isUpdating = false;
}

void NEngine::UpdateService::Register(Component* component)
{
	mUpdateComponents.push_back(component);
}

void NEngine::UpdateService::Unregister(Component* component)
{
	ASSERT(!isUpdating, "Cannot Unregister While Updating");

	auto it = std::find(mUpdateComponents.begin(), mUpdateComponents.end(), component);
	if (it != mUpdateComponents.end())
		mUpdateComponents.erase(it);
}

void NEngine::UpdateService::Deserialize(rapidjson::Value& value)
{
}
