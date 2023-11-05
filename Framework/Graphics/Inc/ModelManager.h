#pragma once
#include "Model.h"

namespace NEngine::Graphics
{
	using ModelId = size_t;

	class ModelManager final
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static ModelManager* Get();

		ModelManager(const ModelManager&) = delete;
		ModelManager(const ModelManager&&) = delete;
		ModelManager() = default;
		~ModelManager() = default;

		ModelId GetModelId(const std::filesystem::path& filepath);
		ModelId LoadModel(const std::filesystem::path& filepath);
		void AddAnimation(ModelId modelID, const std::filesystem::path& filepath);
		const NEngine::Graphics::Model* GetModel(ModelId id);

	private:
		using ModelCache = std::map<ModelId, std::unique_ptr<NEngine::Graphics::Model>>;
		ModelCache inventory;
	};
}