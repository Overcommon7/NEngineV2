#include "Precompiled.h"
#include "Terrain.h"

using namespace NEngine;
using namespace NEngine::Graphics;

void Terrain::Initialize(const std::filesystem::path& fileName, float heightScale)
{	 
	FILE* file;
	fopen_s(&file, fileName.generic_string().c_str(), "rb");

	fseek(file, 0L, SEEK_END);
	const size_t fileSize = ftell(file);
	const uint32_t dimension = (uint32_t)sqrtf(fileSize);
	fseek(file, 0L, SEEK_SET);

	mRows = dimension;
	mColumns = dimension;

	const float	tileCount = 35.f;

	mesh.vertices.resize(mRows * mColumns);
	for (uint32_t z = 0; z < mRows; ++z)
	{
		for (uint32_t x = 0; x < mColumns; ++x)
		{
			const int c = fgetc(file);
			const float height = c / 255.f * heightScale;
			const uint32_t index = x + (z * mColumns);

			auto& vertex = mesh.vertices[index];
			vertex.position = NMath::Vector3(x, height, z);
			vertex.normal = NMath::Vector3::YAxis;
			vertex.uvCoord = NMath::Vector2((float)x / (float)mColumns, (float)z / (float)mRows) * tileCount;
		}
	}
	fclose(file);

	uint32_t cells = (mRows - 1) * (mColumns - 1);
	mesh.indices.reserve(cells * 6);

	for (uint32_t z = 0; z < mRows - 1; ++z)
	{
		for (uint32_t x = 0; x < mColumns - 1; ++x)
		{
			const uint32_t bl = x + (z * mColumns);
			const uint32_t tl = x + ((z + 1) * mColumns);
			const uint32_t br = (x + 1) + (z * mColumns);
			const uint32_t tr = (x + 1) + ((z + 1)* mColumns);

			mesh.indices.push_back(bl);
			mesh.indices.push_back(tl);
			mesh.indices.push_back(tr);

			mesh.indices.push_back(bl);
			mesh.indices.push_back(tr);
			mesh.indices.push_back(br);
		}
	}
}	 
	 
float Terrain::GetHeight(const NMath::Vector3& position) const
{
	const int x = position.x;
	const int z = position.z;

	if (x < 0 || z < 0 || x + 1 >= mColumns || z + 1 >= mRows)
		return 0;

	const uint32_t bl = x + (z * mColumns);
	const uint32_t tl = x + ((z + 1) * mColumns);
	const uint32_t br = (x + 1) + (z * mColumns);
	const uint32_t tr = (x + 1) + ((z + 1) * mColumns);

	const float u = position.x - x;
	const float v = position.z - z;

	float height = 0;
	if (u > v)
	{
		const auto& a = mesh.vertices[br];
		const auto& b = mesh.vertices[tr];
		const auto& c = mesh.vertices[bl];

		const auto deltaAB = b.position.y - a.position.y;
		const auto deltaAC = c.position.y - a.position.y;

		height = a.position.y + (deltaAB * v) + (deltaAC * (1 - u));
	}
	else
	{
		const auto& a = mesh.vertices[tl];
		const auto& b = mesh.vertices[tr];
		const auto& c = mesh.vertices[bl];

		const auto deltaAB = b.position.y - a.position.y;
		const auto deltaAC = c.position.y - a.position.y;

		height = a.position.y + (deltaAB * u) + (deltaAC * (1 - v));
	}

	return height;
}
