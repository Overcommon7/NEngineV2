#include "Precompiled.h"
#include "ModelIO.h"

#include "Model.h"

void NEngine::Graphics::ModelIO::SaveModel(std::filesystem::path filepath, const Model& model)
{
	if (model.meshData.empty()) return;

	if (std::filesystem::exists(filepath))
		std::filesystem::remove(filepath);

	ofstream(filepath).close();

	fstream file(filepath);
	if (!file.is_open()) return;

	const uint32_t meshCount = (uint32_t)model.meshData.size();
	file << "MeshCount: " << meshCount << '\n';
	for (uint32_t i = 0 ; i < meshCount; i++)
	{
		const auto& meshData = model.meshData[i];
		file << "MaterialIndex: " << meshData.materialIndex << '\n';

		const auto& mesh = meshData.mesh;
		const uint32_t vertexCount = (uint32_t)mesh.vertices.size();
		file << "vertexCount: " << vertexCount << '\n';

		for (auto& v : mesh.vertices)
		{
			for (int i = 0; i < 3; i++)
				file << v.position.v[i] << ' ';
			for (int i = 0; i < 3; i++)
				file << v.normal.v[i] << ' ';
			for (int i = 0; i < 3; i++)
				file << v.tangent.v[i] << ' ';
			for (int i = 0; i < 2; i++)
				file << v.uvCoord.v[i] << ' ';
			file << '\n';
		}

		const uint32_t indexCount = mesh.indices.size();
		file << "IndexCount: " << indexCount << '\n';

		for (uint32_t n = 2; n < indexCount; n += 3)
		{
			file 
				<< mesh.indices[n - 2] << ' ' 
				<< mesh.indices[n - 1] << ' ' 
				<< mesh.indices[n] << ' '
				<< '\n';
		}
	
	}

	file.close();
}

void NEngine::Graphics::ModelIO::LoadModel(std::filesystem::path filepath, Model& model)
{
	FILE* file = NULL;
	fopen_s(&file, filepath.generic_string().c_str(), "r");

	if (file == NULL) return;

	uint32_t meshCount = 0;
	fscanf_s(file, "MeshCount: %d\n", &meshCount);
	model.meshData.resize(meshCount);

	for (int i = 0; i < meshCount; ++i)
	{
		auto& meshData = model.meshData[i];
		fscanf_s(file, "MaterialIndex: %d\n", &meshData.materialIndex);

		auto& mesh = meshData.mesh;
		uint32_t vertexCount = 0;
		fscanf_s(file, "vertexCount: %d\n", &vertexCount);
		mesh.vertices.resize(vertexCount);

		for (auto& v : mesh.vertices)
		{
			fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f \n",
				&v.position.x, &v.position.y, &v.position.z,
				&v.normal.x,   &v.normal.y,   &v.normal.z,
				&v.tangent.x,  &v.tangent.y,  &v.tangent.z,
				&v.uvCoord.x,  &v.uvCoord.y);
		}

		uint32_t indexCount = 0;
		fscanf_s(file, "IndexCount: %d\n", &indexCount);
		mesh.indices.resize(indexCount);

		for (int n = 2; n < indexCount; n += 3)
		{
			fscanf_s(file, "%d %d %d \n", &mesh.indices[n - 2], &mesh.indices[n - 1], &mesh.indices[n]);
		}
	}

	fclose(file);
}

void NEngine::Graphics::ModelIO::SaveMaterial(std::filesystem::path filepath, const Model& model)
{
	if (model.materialData.empty()) return;

	//if (std::filesystem::exists(filepath))
	//	std::filesystem::remove(filepath);

	//ofstream(filepath).close();

	//fstream file(filepath);
	//if (!file.is_open()) return;

	//uint32_t materialCount = (uint32_t)model.materialData.size();

	//file << "MaterialCount: " << materialCount;




	FILE* file = NULL;

	fopen_s(&file, filepath.generic_string().c_str(), "w");
	if (file == NULL)
	{
		return;
	}

	uint32_t materialCount = (uint32_t)model.materialData.size();

	fprintf_s(file, "materialCount: %d\n", materialCount);
	for (auto& materialData : model.materialData)
	{
		auto& m = materialData.material;
		fprintf_s(file, "%f %f %f %f\n", m.ambient.r, m.ambient.g, m.ambient.b, m.ambient.a);
		fprintf_s(file, "%f %f %f %f\n", m.diffuse.r, m.diffuse.g, m.diffuse.b, m.diffuse.a);
		fprintf_s(file, "%f %f %f %f\n", m.specular.r, m.specular.g, m.specular.b, m.specular.a);
		fprintf_s(file, "%f %f %f %f\n", m.emissive.r, m.emissive.g, m.emissive.b, m.emissive.a);
		fprintf_s(file, "Power: %f\n", m.materialPower);

		fprintf_s(file, "%s\n", materialData.diffuseMapName.empty() ? "<none>" : materialData.diffuseMapName.c_str());
		fprintf_s(file, "%s\n", materialData.specularMapName.empty() ? "<none>" : materialData.specularMapName.c_str());
		fprintf_s(file, "%s\n", materialData.bumpMapName.empty() ? "<none>" : materialData.bumpMapName.c_str());
		fprintf_s(file, "%s\n", materialData.normalMapName.empty() ? "<none>" : materialData.normalMapName.c_str());
	}
	fclose(file);

}

void NEngine::Graphics::ModelIO::LoadMaterial(std::filesystem::path filepath, Model& model)
{
	FILE* file = NULL;

	fopen_s(&file, filepath.generic_string().c_str(), "r");
	if (file == NULL)
	{
		return;
	}

	auto TryReadTextureName = [&file, &filepath](string& filename)
	{
		char buffer[MAX_PATH]{};
		fscanf_s(file, "%s\n", &buffer, (uint32_t)sizeof(buffer));

		if (strcmp(buffer, "<none>") != 0)
		{
			filename = filepath.replace_extension("material").generic_string();
		}
	};

	uint32_t materialCount = 0;

	fscanf_s(file, "materialCount: %d\n", &materialCount);
	model.materialData.resize(materialCount);
	for (auto& materialData : model.materialData)
	{
		auto& m = materialData.material;
		fscanf_s(file, "%f %f %f %f\n", &m.ambient.r,  &m.ambient.g,  &m.ambient.b,  &m.ambient.a);
		fscanf_s(file, "%f %f %f %f\n", &m.diffuse.r,  &m.diffuse.g,  &m.diffuse.b,  &m.diffuse.a);
		fscanf_s(file, "%f %f %f %f\n", &m.specular.r, &m.specular.g, &m.specular.b, &m.specular.a);
		fscanf_s(file, "%f %f %f %f\n", &m.emissive.r, &m.emissive.g, &m.emissive.b, &m.emissive.a);
		fscanf_s(file, "Power: %f\n", &m.materialPower);
		 
		TryReadTextureName(materialData.diffuseMapName);
		TryReadTextureName(materialData.specularMapName);
		TryReadTextureName(materialData.bumpMapName);
		TryReadTextureName(materialData.normalMapName);
	}
	fclose(file);
}
