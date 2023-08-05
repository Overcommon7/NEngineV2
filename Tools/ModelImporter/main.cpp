#include <../Engine/Inc/NEngine.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cstdio>
#include <iostream>

using namespace NEngine;
using namespace NEngine::Graphics;
using namespace NEngine::NMath;

struct Arguments
{
	std::filesystem::path inputFileName;
	std::filesystem::path outputFileName;
	float scale = 1.0f;
};

std::optional<Arguments> ParseArgs(int  argc,  char* argv[])
{
	if (argc < 3)
	{
		return std::nullopt;
	}
	Arguments arguments;
	arguments.inputFileName = argv[argc - 2];
	arguments.outputFileName = argv[argc - 1];
	for (int i = 1; i + 2 < argc; ++i)
	{
		if (strcmp(argv[i], "-scale") == 0)
		{
			arguments.scale = atof(argv[i + 1]);
			++i;
		}
	}

	return arguments;
}

Vector3 ToVector3(const aiVector3D& v)
{
	return{
		static_cast<float>(v.x),
		static_cast<float>(v.y),
		static_cast<float>(v.z)
	};
}

Quaternion ToQuaternion3(const aiQuaternion& q)
{
	return{
		static_cast<float>(q.x),
		static_cast<float>(q.y),
		static_cast<float>(q.z),
		static_cast<float>(q.w)
	};
}
Color ToColor(const aiColor3D& c) 
{
	return{
		static_cast<float>(c.r),
		static_cast<float>(c.g),
		static_cast<float>(c.b),
		1.0f
	};
}

Matrix4 ToMatrix4(const aiMatrix4x4& m)
{
	return{
		static_cast<float>(m.a1), static_cast<float>(m.b1),static_cast<float>(m.c1),static_cast<float>(m.d1),
		static_cast<float>(m.a2), static_cast<float>(m.b2),static_cast<float>(m.c2),static_cast<float>(m.d2),
		static_cast<float>(m.a3), static_cast<float>(m.b3),static_cast<float>(m.c3),static_cast<float>(m.d3),
		static_cast<float>(m.a4), static_cast<float>(m.b4),static_cast<float>(m.c4),static_cast<float>(m.d4)
	};
}

void ExportEmbeddedTexture(const aiTexture* texture, const Arguments& args, const std::filesystem::path filename)
{
	std::cout << "Extracting embedded texture" << std::endl;
	std::string fullFilename = args.outputFileName.generic_string();
	fullFilename = fullFilename.substr(0, fullFilename.rfind('/') + 1);
	fullFilename += filename.filename().generic_string();

	FILE* file = nullptr;
	errno_t err = fopen_s(&file, fullFilename.c_str(), "wb");
	if (err != 0 || !file)
	{
		std::cout << "ERROR: failed to open file " << fullFilename << " for saving" << std::endl;
		return;
	}

	size_t written = fwrite(texture->pcData, 1, texture->mWidth, file);
	ASSERT(written == texture->mWidth, "ERROR: failed to extract embedded texture");
	fclose(file);
}

std::string FindTexture(const aiScene* scene, const aiMaterial* aiMaterial, aiTextureType textureType, const Arguments& args, const std::string& suffix, uint32_t materialIndex)
{
	const uint32_t textureCount = aiMaterial->GetTextureCount(textureType);
	if (textureCount == 0)
	{
		return std::string();
	}


	std::filesystem::path textureName;
	aiString texturePath;
	if (aiMaterial->GetTexture(textureType, 0, &texturePath) == aiReturn_SUCCESS)
	{
		if (texturePath.C_Str()[0] == '*')
		{
			std::string filename = args.inputFileName.generic_string();
			filename.erase(filename.length() - 4);
			filename += suffix;
			filename += texturePath.C_Str()[1];

			ASSERT(scene->HasTextures(), "Error: NO embedded texture found!");

			int textureIndex = atoi(texturePath.C_Str() + 1);
			ASSERT(textureIndex < scene->mNumTextures, "ERROR: Invelid texture index");

			const aiTexture* embeddedTexture = scene->mTextures[textureIndex];
			ASSERT(embeddedTexture->mHeight == 0, "ERROR: Uncompressed texture found!");

			if (embeddedTexture->CheckFormat("jpg"))
			{
				filename += ".jpg";
			}
			if (embeddedTexture->CheckFormat("png"))
			{
				filename += ".png";
			}
			else
			{
				ASSERT(false, "ERROR: Unrecognized texture format!");
			}

			ExportEmbeddedTexture(embeddedTexture, args, filename);

			std::cout << "Adding Texture " << filename << std::endl;
		}
		else if (const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(texturePath.C_Str()); embeddedTexture)
		{
			std::filesystem::path embeddedFilePath = texturePath.C_Str();
			std::string filename = args.inputFileName.generic_string();
			filename.erase(filename.length() - 4);
			filename += suffix;
			filename += "_" + std::to_string(materialIndex);
			filename += embeddedFilePath.extension().generic_string();

			ExportEmbeddedTexture(embeddedTexture, args, filename);

			std::cout << "Adding Texture " << filename.c_str() << std::endl;
			textureName = filename;
		}
		else
		{
			std::filesystem::path filePath = texturePath.C_Str();
			std::string filename = filePath.filename().generic_string();
			std::cout << "Adding Texture " << filename.c_str() << std::endl;
			textureName = filename;
		}
	}

	return textureName.filename().generic_string();
}

int main(int argc, char* argv[])
{
	const std::optional<Arguments> argOpt = ParseArgs(argc, argv);
	if (!argOpt)
	{
		std::cout << "No arguments to import fbx model" << std::endl;
		return 1;
	}

	const Arguments& args = *argOpt;
	Assimp::Importer importer;
	const uint32_t flags = aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded;

	const aiScene* scene = importer.ReadFile(args.inputFileName.generic_string(), flags);
	if (!scene)
	{
		std::cout << "Error: " << importer.GetErrorString();
		return 1;
	}

	std::cout << "Importing " << args.inputFileName.generic_string() << "..." << std::endl;

	Model model;
	if (scene->HasMeshes())
	{
		std::cout << "Reading Mesh Data..." << std::endl;
		for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			const aiMesh* aiMesh = scene->mMeshes[meshIndex];
			if (aiMesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
			{
				continue;
			}

			const uint32_t numVertices = aiMesh->mNumVertices;
			const uint32_t numFaces = aiMesh->mNumFaces;
			const uint32_t numIndices = numFaces * 3;

			Model::MeshData& meshData = model.meshData.emplace_back();

			std::cout << "Reading Material Index..." << std::endl;
			meshData.materialIndex = aiMesh->mMaterialIndex;

			std::cout << "Reading vertices..." << std::endl;

			Mesh& mesh = meshData.mesh;
			mesh.vertices.reserve(numVertices);

			const aiVector3D* positions = aiMesh->mVertices;
			const aiVector3D* normals = aiMesh->mNormals;
			const aiVector3D* tangents = aiMesh->HasTangentsAndBitangents() ? aiMesh->mTangents : nullptr;
			const aiVector3D* texCoords = aiMesh->HasTextureCoords(0) ? aiMesh->mTextureCoords[0] : nullptr;
			for (uint32_t v = 0; v < numVertices; ++v)
			{
				Vertex& vertex = mesh.vertices.emplace_back();
				vertex.position = ToVector3(positions[v]) * args.scale;
				vertex.normal = ToVector3(normals[v]);
				vertex.tangent = tangents ? ToVector3(tangents[v]) : Vector3::Zero;
				if (texCoords)
				{
					vertex.uvCoord = { texCoords[v].x, texCoords[v].y };
				}
			}

			std::cout << "Reading indices..." << std::endl;
			mesh.indices.reserve(numIndices);
			const auto& aiFaces = aiMesh->mFaces;
			for (uint32_t i = 0; i < numFaces; ++i)
			{
				const aiFace& aiFace = aiFaces[i];
				mesh.indices.push_back(aiFace.mIndices[0]);
				mesh.indices.push_back(aiFace.mIndices[1]);
				mesh.indices.push_back(aiFace.mIndices[2]);
			}
		}
	}
	if (scene->HasMaterials())
	{
		std::cout << "Reading Material Data....\n" << std::endl;

		const uint32_t numMaterials = scene->mNumMaterials;
		model.materialData.reserve(numMaterials);
		for (uint32_t materialIndex = 0; materialIndex < numMaterials; ++materialIndex)
		{
			const aiMaterial* aiMaterial = scene->mMaterials[materialIndex];
			aiColor3D ambient, diffuse, emissive, specular;
			ai_real specularPower = 1.f;

			aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
			aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
			aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
			aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
			aiMaterial->Get(AI_MATKEY_SHININESS, specularPower);

			Model::MaterialData& materialData = model.materialData.emplace_back();
			materialData.material.ambient = ToColor(ambient);
			materialData.material.diffuse = ToColor(diffuse);
			materialData.material.emissive = ToColor(emissive);
			materialData.material.specular = ToColor(specular);

			materialData.material.materialPower = specularPower;

			materialData.diffuseMapName = FindTexture(scene, aiMaterial, aiTextureType_DIFFUSE, args, "_diffuse", materialIndex);
			materialData.normalMapName = FindTexture(scene, aiMaterial, aiTextureType_NORMALS, args, "_normal", materialIndex);
			materialData.bumpMapName = FindTexture(scene, aiMaterial, aiTextureType_DISPLACEMENT, args, "_displacement", materialIndex);
			materialData.specularMapName = FindTexture(scene, aiMaterial, aiTextureType_SPECULAR, args, "_specular", materialIndex);
		}
	}

	std::cout << "Saving Model..." << std::endl;
	ModelIO::SaveModel(args.outputFileName, model);

	std::cout << "Saving Material..." << std::endl;
	ModelIO::SaveMaterial(args.outputFileName, model);

	return 0;
}
