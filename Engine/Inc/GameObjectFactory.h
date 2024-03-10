#pragma once
namespace NEngine
{
	class GameObject;

	using CustomMake = function<bool(const string&, rapidjson::Value&, GameObject&)>;

	class GameObjectFactory
	{		
	public:
		static void SetCustomMake(CustomMake customMake);
		static void Make(const std::filesystem::path& templatePath, GameObject& gameObject);
	private:
	public:
		GameObjectFactory() = delete;
		GameObjectFactory(const GameObjectFactory& g) = delete;
		GameObjectFactory(const GameObjectFactory&& g) = delete;
		GameObjectFactory& operator=(const GameObjectFactory& g) = delete;
		GameObjectFactory& operator=(const GameObjectFactory&& g) = delete;

	};
}


