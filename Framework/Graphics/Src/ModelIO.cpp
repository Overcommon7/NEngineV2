#include "Precompiled.h"
#include "ModelIO.h"

#include "Model.h"
#include "AnimationBuilder.h"



void NEngine::Graphics::ModelIO::SaveSkeleton(std::filesystem::path filepath, const Model& model)
{
	if (model.skeleton == nullptr || model.skeleton->bones.empty()) return;

	filepath.replace_extension(".skeleton");
	FILE* file = nullptr;

	fopen_s(&file, filepath.generic_string().c_str(), "w");
	if (file == nullptr) return;

	auto WriteMatrix = [&file](const NMath::Matrix4& m) -> void
		{
			fprintf_s(file, "%f %f %f %f\n", m._11, m._12, m._13, m._14);
			fprintf_s(file, "%f %f %f %f\n", m._21, m._22, m._23, m._24);
			fprintf_s(file, "%f %f %f %f\n", m._31, m._32, m._33, m._34);
			fprintf_s(file, "%f %f %f %f\n", m._41, m._42, m._43, m._44);
		};

	uint32_t boneCount = model.skeleton->bones.size();
	fprintf_s(file, "BoneCount: %d\n", boneCount);
	fprintf_s(file, "RootBone: %d\n", model.skeleton->root->index);

	for (uint32_t i = 0; i < boneCount; ++i)
	{
		const auto boneData = model.skeleton->bones[i].get();
		fprintf_s(file, "BoneName: %s\n", boneData->name.c_str());
		fprintf_s(file, "BoneIndex: %d\n", boneData->index);
		fprintf_s(file, "BoneParentIndex: %d\n", boneData->parentIndex);

		uint32_t childCount = boneData->childrenIndicies.size();
		fprintf_s(file, "BoneChildrenCount: %d\n", childCount);
		for (uint32_t c = 0; c < childCount; ++c)
		{
			fprintf_s(file, "%d\n", boneData->childrenIndicies[c]);
		}

		WriteMatrix(boneData->offsetTransform);
		WriteMatrix(boneData->toParentTransform);
	}

	fclose(file);
}

void NEngine::Graphics::ModelIO::LoadSkeleton(std::filesystem::path filepath, Model& model)
{
	filepath.replace_extension(".skeleton");
	FILE* file = nullptr;

	fopen_s(&file, filepath.generic_string().c_str(), "r");
	if (file == nullptr) return;

	auto ReadMatrix = [&file](NMath::Matrix4& m) -> void
	{
		fscanf_s(file, "%f %f %f %f\n", &m._11, &m._12, &m._13, &m._14);
		fscanf_s(file, "%f %f %f %f\n", &m._21, &m._22, &m._23, &m._24);
		fscanf_s(file, "%f %f %f %f\n", &m._31, &m._32, &m._33, &m._34);
		fscanf_s(file, "%f %f %f %f\n", &m._41, &m._42, &m._43, &m._44);
	};

	model.skeleton = std::make_unique<Skeleton>();

	uint32_t boneCount = 0;
	uint32_t rootIndex = 0;
	fscanf_s(file, "BoneCount: %d\n", &boneCount);
	fscanf_s(file, "RootBone: %d\n", &rootIndex);
	model.skeleton->bones.resize(boneCount);
	for (uint32_t i = 0; i < boneCount; ++i)
		model.skeleton->bones[i] = std::make_unique<Bone>();

	model.skeleton->root = model.skeleton->bones[rootIndex].get();

	for (uint32_t i = 0; i < boneCount; ++i)
	{
		auto boneData = model.skeleton->bones[i].get();
		uint32_t boneIndex = 0;
		char boneName[MAX_PATH]{};

		fscanf_s(file, "BoneName: %s\n", boneName, (uint32_t)sizeof(boneName));
		fscanf_s(file, "BoneIndex: %d\n", &boneData->index);
		fscanf_s(file, "BoneParentIndex: %d\n", &boneData->parentIndex);

		boneData->name = std::move(boneName);
		if (boneData->parentIndex > -1)
		{
			boneData->parent = model.skeleton->bones[boneData->parentIndex].get();
		}
		uint32_t childCount = 0;
		fscanf_s(file, "BoneChildrenCount: %d\n", &childCount);

		for (uint32_t c = 0; c < childCount; ++c)
		{
			uint32_t childIndex = 0;
			fscanf_s(file, "%d\n", &childIndex);
			boneData->childrenIndicies.push_back(childIndex);
			boneData->children.push_back(model.skeleton->bones[childIndex].get());
		}

		ReadMatrix(boneData->offsetTransform);
		ReadMatrix(boneData->toParentTransform);
	}

	fclose(file);
}

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

	filepath.replace_extension(".material");
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

void NEngine::Graphics::ModelIO::SaveAnimations(std::filesystem::path filepath, const Model& model)
{
	if (!model.skeleton || model.skeleton->bones.empty() || model.animationClips.empty()) return;

	filepath.replace_extension("aniset");

	FILE* file;
	fopen_s(&file, filepath.generic_string().c_str(), "w");
	if (!file) return;

	uint32_t animClipCount = model.animationClips.size();

	fprintf_s(file, "AnimationClipCount: %d\n", animClipCount);
	for (uint32_t i = 0; i < animClipCount; ++i)
	{
		auto& animClipData = model.animationClips[i];
		fprintf_s(file, "Animation Clip Name: %s\n", animClipData.name.c_str());
		fprintf_s(file, "Tick Duration: %f\n", animClipData.tickDuration);
		fprintf_s(file, "Ticks Per Second: %f\n", animClipData.ticksPerSecond);


		uint32_t boneAnimCount = animClipData.boneAnimations.size();
		fprintf_s(file, "Bone Animations Count: %d\n", boneAnimCount);

		for (uint32_t b = 0; b < boneAnimCount; ++b)
		{
			auto boneAnim = animClipData.boneAnimations[b].get();
			if (!boneAnim)
			{
				fprintf_s(file, "[EMPTY]\n");
				continue;
			}
			fprintf_s(file, "[ANIMATION]\n");
			AnimationIO::Write(file, *boneAnim);
		}
	}

	fclose(file);
}

void NEngine::Graphics::ModelIO::LoadAnimations(std::filesystem::path filepath, Model& model)
{
	filepath.replace_extension("aniset");

	FILE* file;
	fopen_s(&file, filepath.generic_string().c_str(), "r");
	if (!file) return;

	uint32_t animClipCount = model.animationClips.size();
	fscanf_s(file, "AnimationClipCount: %d\n", &animClipCount);
	for (uint32_t i = 0; i < animClipCount; ++i)
	{
		auto& animClipData = model.animationClips.emplace_back();
		char animClipName[MAX_PATH]{};

		fscanf_s(file, "Animation Clip Name: %s\n", animClipName, (uint32_t)sizeof(animClipName));
		animClipData.name = std::move(animClipName);


		fscanf_s(file, "Tick Duration: %f\n", &animClipData.tickDuration);
		fscanf_s(file, "Ticks Per Second: %f\n", &animClipData.ticksPerSecond);


		uint32_t boneAnimCount = 0;
		fscanf_s(file, "Bone Animations Count: %d\n", &boneAnimCount);
		animClipData.boneAnimations.resize(boneAnimCount);

		for (uint32_t b = 0; b < boneAnimCount; ++b)
		{
			char label[128]{};
			fscanf_s(file, "%s\n", label, (uint32_t)sizeof(label));
			if (strcmp(label, "[ANIMATION]") == 0)
			{
				animClipData.boneAnimations[b] = std::make_unique<Animation>();
				AnimationIO::Read(file, *animClipData.boneAnimations[b]);
			}
		}
	}

	fclose(file);
}

void NEngine::Graphics::AnimationIO::Write(FILE* file, const Animation& animation)
{
	uint32_t count = animation.mPositionKeys.size();
	fprintf_s(file, "Position Key Count: %d\n", count);
	for (auto& key : animation.mPositionKeys)
	{
		fprintf_s(file, "%f %f %f %f\n", key.time, key.Key.x, key.Key.y, key.Key.z);
	}
	count = animation.mRotationKeys.size();
	fprintf_s(file, "Rotation Key Count: %d\n", count);
	for (auto& key : animation.mRotationKeys)
	{
		fprintf_s(file, "%f %f %f %f %f\n", key.time, key.Key.x, key.Key.y, key.Key.z, key.Key.w);

	}
	count = animation.mScaleKeys.size();
	fprintf_s(file, "Scale Key Count: %d\n", count);
	for (auto& key : animation.mScaleKeys)
	{
		fprintf_s(file, "%f %f %f %f\n", key.time, key.Key.x, key.Key.y, key.Key.z);
	}
}

void NEngine::Graphics::AnimationIO::Read(FILE* file, Animation& animation)
{
	AnimationBuilder builder;
	float time = 0.f;
	uint32_t count = 0;
	fprintf_s(file, "Position Key Count: %d\n", count);
	for (uint32_t i = 0; i < count; ++i)
	{
		NMath::Vector3 position;
		fscanf_s(file, "%f %f %f %f\n", &time, &position.x, &position.y, &position.z);
		builder.AddPositionKey(position, time);
	}

	fscanf_s(file, "Rotation Key Count: %d\n", &count);
	for (uint32_t i = 0; i < count; ++i)
	{
		NMath::Quaternion rotation;
		fscanf_s(file, "%f %f %f %f %f\n", &time, &rotation.x, &rotation.y, &rotation.z, &rotation.w);
		builder.AddRotationKey(rotation, time);
	}


	fscanf_s(file, "Scale Key Count: %d\n", &count);
	for (uint32_t i = 0; i < count; ++i)
	{
		NMath::Vector3 scale;
		fscanf_s(file, "%f %f %f %f\n", &time, &scale.x, &scale.y, &scale.z);
		builder.AddScaleKey(scale, time);
	}

	animation = builder.Build();
}
