#pragma once

#include "MeshTypes.h"
#include "Material.h"
#include "Skeleton.h"

namespace NEngine::Graphics
{
	struct Model
	{
		struct MeshData
		{
			Mesh mesh;
			uint32_t materialIndex = 0;
		};

		struct MaterialData
		{
			Material material;
			string diffuseMapName;
			string specularMapName;
			string bumpMapName;
			string normalMapName;
		};

		vector<MeshData> meshData;
		vector<MaterialData> materialData;
		std::unique_ptr<Skeleton> skeleton;
	};
}
