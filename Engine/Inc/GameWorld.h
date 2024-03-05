#pragma once

#include "GameObject.h"
#include "Services/Service.h"

namespace NEngine
{
	template<class T>
	concept IsService = std::is_base_of_v<Service, T>;

	class GameWorld;
	using CustomService = function<bool(const string&, const rapidjson::Value&, GameWorld&)>;

	class GameWorld final
	{
	public:
		static void SetCustomService(CustomService customService);
		static void SetEditObject(const string& objectName);
		static const string& GetEditObject();

		void Initialize(uint32_t capacity);
		void Terminate();

		void Update(float deltaTime);
		void Render();
		void DebugUI();
		void EditorUI();

		GameObject* CreateGameObject(const std::filesystem::path& templateFile = "");
		GameObject* GetGameObject(const GameObjectHandle& handle);
		GameObject* GetGameObject(const string& name);
		void DestroyGameObject(const GameObjectHandle& handle);

		void SaveTemplate(const std::filesystem::path& templateFile, const GameObjectHandle& handle);
		void SaveLevel(const std::filesystem::path& levelFile);
		void LoadLevel(const std::filesystem::path& levelFile);
		const std::filesystem::path& GetLevelPath() const { return mLevelPath; }
		

		template<IsService T>
		T* AddService()
		{
			ASSERT(!mInitialized, "Cannot Add Serice To Initialized World");
			auto& service = mServices.emplace_back(std::make_unique<T>());
			service->gameWorld = this;
			return static_cast<T*>(service.get());
		}

		template<IsService T>
		T* GetService()
		{
			for (auto& service : mServices)
			{
				if (service->GetTypeId() == T::StaticGetTypeId())
				{
					return static_cast<T*>(service.get());
				}
			}
			return nullptr;
		}
	private:
		bool IsValid(const GameObjectHandle& handle);
		void ProcessDestroyList();

		struct Slot
		{
			std::unique_ptr<GameObject> gameObject;
			uint32_t generation = 0;
		};

		bool mInitialized = false;
		bool mIsUpdating = false;
		using Services = std::vector<std::unique_ptr<Service>>;
		using GameObjectSlots = std::vector<Slot>;

		std::filesystem::path mLevelPath;
		GameObjectSlots mSlots;
		std::vector<uint32_t> mFreeSlots;
		std::vector<uint32_t> mToBeDestroyed;

		Services mServices;


		inline static string sEditTemplateName = "";
	};


}


