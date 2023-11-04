#include "Precompiled.h"
#include "RenderObject.h"
#include "Model.h"
#include "Animator.h"


using namespace NEngine;
using namespace NEngine::Graphics;

RenderGroup NEngine::Graphics::CreateRenderGroup(const Model& model, const Animator* animator)
{
	RenderGroup renderGroup;
	renderGroup.reserve(model.meshData.size());

	auto TryLoadTexture = [](const string& textureName) -> TextureId
	{
		if (textureName.empty()) return 0;
		return TextureManager::Get()->LoadTexture(textureName, false);
	};

	for (auto& meshData : model.meshData)
	{
		auto& renderObject = renderGroup.emplace_back();

		if (meshData.materialIndex < model.materialData.size())
		{
			auto& materialData = model.materialData[meshData.materialIndex];
			renderObject.material = materialData.material;
			renderObject.diffuseMapId = TryLoadTexture(materialData.diffuseMapName);
			renderObject.specMapId = TryLoadTexture(materialData.specularMapName);
			renderObject.bumpMapId = TryLoadTexture(materialData.bumpMapName);
			renderObject.normalMapId = TryLoadTexture(materialData.normalMapName);
		}

		renderObject.meshBuffer.Initialize(meshData.mesh);
		renderObject.skeleton = model.skeleton.get();
		renderObject.animator = animator;
	}
	return renderGroup;
}

RenderGroup NEngine::Graphics::CreateRenderGroup(const ModelId& id, const Animator* animator)
{
	auto model = ModelManager::Get()->GetModel(id);
	ASSERT(model != nullptr, "Rendergroup");

	RenderGroup group = CreateRenderGroup(*model, animator);
	for (auto& object : group)
		object.modelId = id;
	return group;
}

void NEngine::Graphics::CleanupRenderGroup(RenderGroup& renderGroup)
{
	for (auto& renderObject : renderGroup)
	{
		renderObject.Terminate();
	}
}
