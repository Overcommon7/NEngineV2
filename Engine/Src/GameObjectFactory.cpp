#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "GameObject.h"
#include "Components/Transform.h"

using namespace NEngine;
namespace rj = rapidjson;

void NEngine::GameObjectFactory::Make(const std::filesystem::path& templatePath, GameObject& gameObject)
{
	FILE* file;
	auto err = fopen_s(&file, templatePath.string().c_str(), "r");
	ASSERT(!err, "File Not Found");

	char readBuffer[USHRT_MAX];
	rj::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
	fclose(file);

	rj::Document doc;
	doc.ParseStream(readStream);
	auto components = doc["Components"].GetObj();
	for (auto& component : components)
	{
		const char* componentName = component.name.GetString();
		if (strcmp(componentName, "Transform") == 0)
		{
			auto transform = gameObject.AddComponent<Transform>();
			transform->Deserialize(component.value);
		}
	}
}
