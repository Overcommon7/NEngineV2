#include "Precompiled.h"
#include "GameObject.h"

#include "Components/Transform.h"
#include "GameWorld.h"
#include "NEngine.h"

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
	if (ImGui::CollapsingHeader(mName.c_str()))
	{
		for (const auto& component : mComponents)
			component->DebugUI();
	}
	
}

void NEngine::GameObject::EditorUI()
{
	if (ImGui::CollapsingHeader(mName.c_str()))
	{
		for (const auto& component : mComponents)
		{
			component->EditorUI();
		}

		if (ImGui::Button(("Edit##" + mName).c_str()))
		{
			GameWorld::SetEditObject(mName);
			MainApp().ChangeState("TemplateState");
		}
	}

		
}

void NEngine::GameObject::Serialize(rapidjson::Document& doc)
{
	rapidjson::Value components(rapidjson::kObjectType);
	for (auto& component : mComponents)
	{
		component->Serialize(doc, components);
	}
	doc.SetObject();
	doc.AddMember("Components", components, doc.GetAllocator());
}
