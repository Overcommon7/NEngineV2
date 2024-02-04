#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "GameObject.h"
#include "Components/Transform.h"
#include "Components/CameraComponent.h"
#include "Components/FPSCamera.h"
#include "Components/ModelComponent.h"
#include "Components/MeshComponent.h"

using namespace NEngine;
namespace rj = rapidjson;

void NEngine::GameObjectFactory::Make(const std::filesystem::path& templatePath, GameObject& gameObject)
{
	FILE* file;
	auto err = fopen_s(&file, templatePath.string().c_str(), "r");
	ASSERT(!err, "File Not Found");

	char readBuffer[12000];
	rj::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
	fclose(file);

	bool hasTransform = false;
	rj::Document doc;
	doc.ParseStream(readStream);
	auto components = doc["Components"].GetObj();
	for (auto& component : components)
	{
		string componentName(component.name.GetString());
		if (componentName == "Transform")
		{
			auto transform = gameObject.AddComponent<Transform>();
			transform->Deserialize(component.value);
			hasTransform = true;
		}
		else if (componentName == "CameraComponent")
		{
			auto camera = gameObject.AddComponent<CameraComponent>();
			camera->Deserialize(component.value);
		}
		else if (componentName == "FPSCamera")
		{
			auto camera = gameObject.AddComponent<FPSCamera>();
			camera->Deserialize(component.value);
		}
		else if (componentName == "Model")
		{
			auto model = gameObject.AddComponent<ModelComponent>();
			model->Deserialize(component.value);
		}
		else if (componentName == "Mesh")
		{
			auto mesh = gameObject.AddComponent<MeshComponent>();
			mesh->Deserialize(component.value);
		}
	}

	if (!hasTransform)
		gameObject.AddComponent<Transform>();
}
