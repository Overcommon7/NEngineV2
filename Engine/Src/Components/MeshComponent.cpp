#include "Precompiled.h"
#include "Components/MeshComponent.h"
#include "Services/RenderService.h"
#include "GameWorld.h"

#include "GameObject.h"

#include "SaveUtil.h"

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

void NEngine::MeshComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	rapidjson::Value shapeValue(rapidjson::kObjectType);
	rapidjson::Value materialValue(rapidjson::kObjectType);
	rapidjson::Value texturesValue(rapidjson::kObjectType);

	SaveUtil::SaveString("Type", mLoadingData.shapeType.c_str(), doc, shapeValue);
	if (mLoadingData.shapeType == "Cube")
	{
		SaveUtil::SaveFloat("Size", mLoadingData.fParam, doc, shapeValue);
	}
	else if (mLoadingData.shapeType == "Sphere")
	{
		SaveUtil::SaveInt("Slices", mLoadingData.iParam0, doc, shapeValue);
		SaveUtil::SaveInt("Rings", mLoadingData.iParam0, doc, shapeValue);
		SaveUtil::SaveFloat("Radius", mLoadingData.fParam, doc, shapeValue);
	}
	else if (mLoadingData.shapeType == "Plane")
	{
		SaveUtil::SaveInt("Rows", mLoadingData.iParam0, doc, shapeValue);
		SaveUtil::SaveInt("Columns", mLoadingData.iParam0, doc, shapeValue);
		SaveUtil::SaveFloat("Spacing", mLoadingData.fParam, doc, shapeValue);
	}

	auto& material = mModel.materialData.front().material;
	SaveUtil::SaveColor("ColorAmbient", material.ambient, doc, materialValue);
	SaveUtil::SaveColor("ColorDiffuse", material.diffuse, doc, materialValue);
	SaveUtil::SaveColor("ColorSpecular", material.specular, doc, materialValue);
	SaveUtil::SaveColor("ColorEmissive", material.emissive, doc, materialValue);
	SaveUtil::SaveFloat("SpecularPower", material.materialPower, doc, materialValue);

	auto& data = mModel.materialData.front();
	if (!data.diffuseMapName.empty())
		SaveUtil::SaveString("DiffuseMap", data.diffuseMapName.c_str(), doc, texturesValue);
	if (!data.bumpMapName.empty())
		SaveUtil::SaveString("BumpMap", data.bumpMapName.c_str(), doc, texturesValue);
	if (!data.bumpMapName.empty())
		SaveUtil::SaveString("NormalMap", data.normalMapName.c_str(), doc, texturesValue);
	if (!data.specularMapName.empty())
		SaveUtil::SaveString("SpecularMap", data.specularMapName.c_str(), doc, texturesValue);

	componentValue.AddMember("Shape", shapeValue, doc.GetAllocator());
	componentValue.AddMember("Material", materialValue, doc.GetAllocator());
	componentValue.AddMember("Textures", texturesValue, doc.GetAllocator());

	SaveUtil::SaveBool("CastShadow", mCastShadow, doc, componentValue);

	value.AddMember("Mesh", componentValue, doc.GetAllocator());
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
			mLoadingData.shapeType = shapeData["Type"].GetString();
			if (mLoadingData.shapeType == "Cube")
			{
				mLoadingData.fParam = shapeData["Size"].GetFloat();
				//meshData.mesh = Graphics::MeshBuilder::CreateCubePX(size)
			
			}
			else if (mLoadingData.shapeType == "Sphere")
			{
				mLoadingData.iParam0 = shapeData["Slices"].GetInt();
				mLoadingData.iParam1 = shapeData["Rings"].GetInt();
				mLoadingData.fParam = shapeData["Radius"].GetFloat();

				meshData.mesh = Graphics::MeshBuilder::CreateSphere(mLoadingData.iParam0, mLoadingData.iParam1, mLoadingData.fParam);
			}
			else if (mLoadingData.shapeType == "Plane")
			{
				mLoadingData.iParam0 = shapeData["Rows"].GetInt();
				mLoadingData.iParam1 = shapeData["Columns"].GetInt();
				mLoadingData.fParam  = shapeData["Spacing"].GetFloat();

				meshData.mesh = Graphics::MeshBuilder::CreateGroundPlane(mLoadingData.iParam0, mLoadingData.iParam1, mLoadingData.fParam);
			}
			else
			{
				ASSERT(false, "ShapeType %s is invalid", mLoadingData.shapeType.c_str());
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

void NEngine::MeshComponent::EditorUI()
{
	if (ImGui::CollapsingHeader((string("MeshComponent##") + mOwner->GetName().data()).c_str()))
	{
		bool update = false;
		auto& material = mModel.materialData.front().material;
		update |= ImGui::ColorEdit4("Ambient", &material.ambient.r);
		update |= ImGui::ColorEdit4("Diffuse", &material.diffuse.r);
		update |= ImGui::ColorEdit4("Specular", &material.specular.r);
		update |= ImGui::ColorEdit4("Emissive", &material.emissive.r);
		update |= ImGui::DragFloat("Power", &material.materialPower);
		update |= ImGui::Checkbox("CastShadow", &mCastShadow);

		if (update)
		{
			RenderService* rs = mOwner->GetWorld().GetService<RenderService>();
			rs->Unregister(this);
			rs->Register(this);
		}


		
	}
}
