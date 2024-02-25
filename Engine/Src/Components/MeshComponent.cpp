#include "Precompiled.h"
#include "Components/MeshComponent.h"
#include "Services/RenderService.h"
#include "GameWorld.h"

#include "GameObject.h"

void NEngine::MeshComponent::Initialize()
{
	auto renderService = mOwner->GetWorld().GetService<RenderService>();
	renderService->Register(this);
}

void NEngine::MeshComponent::Terminate()
{
	auto renderService = mOwner->GetWorld().GetService<RenderService>();
	renderService->Unregister(this);
}

void NEngine::MeshComponent::Deserialize(rapidjson::Value& value)
{
	Graphics::Model::MeshData& meshData = mModel.meshData.emplace_back();
	Graphics::Model::MaterialData& material = mModel.materialData.emplace_back();
	if (value.HasMember("Shape"))
	{
		const auto& shapeData = value["Shape"].GetObj();
		if (shapeData.HasMember("Type"))
		{
			const string shapeType(shapeData["Type"].GetString());
			if (shapeType == "Cube")
			{
				const float size = shapeData["Size"].GetFloat();
				//meshData.mesh = Graphics::MeshBuilder::CreateCubePX(size)
			
			}
			else if (shapeType == "Sphere")
			{
				const int slices = shapeData["Slices"].GetInt();
				const int rings = shapeData["Rings"].GetInt();
				const float radius = shapeData["Radius"].GetFloat();

				meshData.mesh = Graphics::MeshBuilder::CreateSphere(slices, rings, radius);
			}
			else if (shapeType == "Plane")
			{
				const int rows = shapeData["Rows"].GetInt();
				const int columns = shapeData["Columns"].GetInt();
				const float spacing = shapeData["Spacing"].GetFloat();

				meshData.mesh = Graphics::MeshBuilder::CreateGroundPlane(rows, columns, spacing);
			}
			else
			{
				ASSERT(false, "ShapeType %s is invalid", shapeType.c_str());
			}
		}

		if (value.HasMember("Material"))
		{
			if (value.HasMember("ColorAmbient"))
			{
				const auto& color = value["ColorAmbient"].GetArray();
				const float r = color[0].GetFloat();
				const float g = color[1].GetFloat();
				const float b = color[2].GetFloat();
				const float a = color[3].GetFloat();
				material.material.ambient = { r, g, b, a };
			}
			if (value.HasMember("ColorDiffuse"))
			{
				const auto& color = value["ColorDiffuse"].GetArray();
				const float r = color[0].GetFloat();
				const float g = color[1].GetFloat();
				const float b = color[2].GetFloat();
				const float a = color[3].GetFloat();
				material.material.diffuse = { r, g, b, a };
			}
			if (value.HasMember("ColorEmissive"))
			{
				const auto& color = value["ColorEmissive"].GetArray();
				const float r = color[0].GetFloat();
				const float g = color[1].GetFloat();
				const float b = color[2].GetFloat();
				const float a = color[3].GetFloat();
				material.material.emissive = { r, g, b, a };
			}
			if (value.HasMember("ColorSpecular"))
			{
				const auto& color = value["ColorSpecular"].GetArray();
				const float r = color[0].GetFloat();
				const float g = color[1].GetFloat();
				const float b = color[2].GetFloat();
				const float a = color[3].GetFloat();
				material.material.specular = { r, g, b, a };
			}
			if (value.HasMember("SpecularPower"))
			{
				material.material.materialPower = value["SpecularPower"].GetInt();
			}

			if (value.HasMember("Textures"))
			{
				const auto& textureData = value["Textures"].GetObj();
				if (textureData.HasMember("DiffuseMap"))
					material.diffuseMapName = textureData["DiffuseMap"].GetString();
				if (textureData.HasMember("NormalMap"))
					material.normalMapName = textureData["NormalMap"].GetString();
				if (textureData.HasMember("BumpMap"))
					material.bumpMapName = textureData["BumpMap"].GetString();
				if (textureData.HasMember("SpecularMap"))
					material.specularMapName = textureData["SpecularMap"].GetString();
			}
		}

		if (value.HasMember("CastShadow"))
		{
			mCastShadow = value["CastShadow"].GetBool();
		}
	}
}
