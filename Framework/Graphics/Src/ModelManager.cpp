#include "Precompiled.h"
#include "ModelManager.h"

#include "ModelIO.h"

namespace
{
	std::unique_ptr<NEngine::Graphics::ModelManager> modelManager;
}

void NEngine::Graphics::ModelManager::StaticInitialize()
{
	ASSERT(modelManager == nullptr, "Model Manager already Initialized");
	modelManager = std::make_unique<ModelManager>();
}

void NEngine::Graphics::ModelManager::StaticTerminate()
{
	modelManager.reset();
}

NEngine::Graphics::ModelManager* NEngine::Graphics::ModelManager::Get()
{
	ASSERT(modelManager != nullptr, "Model Manager already Initialized");
	return modelManager.get();
}

NEngine::Graphics::ModelId NEngine::Graphics::ModelManager::GetModelId(const std::filesystem::path& filepath)
{
	return std::filesystem::hash_value(filepath);
}

NEngine::Graphics::ModelId NEngine::Graphics::ModelManager::LoadModel(const std::filesystem::path& filepath)
{
	const auto modelId = GetModelId(filepath);
	auto [iter, success] = inventory.insert({ modelId, nullptr });

	if (success)
	{
		auto& modelPtr = iter->second;
		modelPtr = std::make_unique<Model>();
		NEngine::Graphics::ModelIO::LoadModel(filepath, *modelPtr);
		NEngine::Graphics::ModelIO::LoadMaterial(filepath, *modelPtr);
	}
	return modelId;
}

const NEngine::Graphics::Model* NEngine::Graphics::ModelManager::GetModel(ModelId id)
{
	auto model = inventory.find(id);
	if (model != inventory.end())
	{
		return model->second.get();
	}
	return nullptr;
}
