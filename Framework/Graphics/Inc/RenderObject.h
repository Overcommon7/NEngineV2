#pragma once
#include "MeshBuffer.h"
#include "Transform.h"
#include "TextureManager.h"
#include "Material.h"
#include "ModelManager.h"

namespace NEngine::Graphics {

	struct Model;

	class RenderObject 
	{
	public:
		inline void Terminate() 
		{
			diffuseMapId = 0;
			bumpMapId = 0;
			specMapId = 0;
			normalMapId = 0;
			modelId = 0;
			meshBuffer.Terminate();
		}
		Transform transform;
		Material material;
		
		TextureId diffuseMapId;
		TextureId normalMapId;
		TextureId bumpMapId;
		TextureId specMapId;
		ModelId modelId;

		MeshBuffer meshBuffer;
	};

	using RenderGroup = std::vector<RenderObject>;
	[[nodiscard]] RenderGroup CreateRenderGroup(const Model& model);
	[[nodiscard]] RenderGroup CreateRenderGroup(const ModelId& id);
	void CleanupRenderGroup(RenderGroup& renderGroup);

	template<class Effect>
	inline void DrawRenderGroup(Effect& effect, const RenderGroup& renderGroup)
	{
		for (auto& renderObject : renderGroup)
		{
			effect.Render(renderObject);
		}
	}

}