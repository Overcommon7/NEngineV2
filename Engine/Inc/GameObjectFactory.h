#pragma once
namespace NEngine
{
	class GameObject;

	class GameObjectFactory
	{		
	public:
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


