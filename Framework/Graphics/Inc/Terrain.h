#pragma once

#include "MeshTypes.h"

namespace NEngine::Graphics
{
	class Terrain
	{
	public:
		void Initialize(const std::filesystem::path& fileName, float heightScale);
		float GetHeight(const NMath::Vector3& position) const;

		Mesh mesh;
		uint32_t mRows = 0;
		uint32_t mColumns;
	};
}
