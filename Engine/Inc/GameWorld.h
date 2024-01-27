#pragma once

#include "GameObject.h"
#include "Services/Service.h"

namespace NEngine
{
	template<class T>
	concept IsService = std::is_base_of_v<Service, T>;

	class GameWorld final
	{
	public:
		void Intialize(uint32_t capacity);
		void Terminate();

		void Update(float deltaTime);
		void Render();
		void DebugUI();

		GameObject* CreateGameObject(const std::filesystem::path& templateFile);
		GameObject* GetGameObject(const GameObjectHandle& handle);
		void DestroyGameObject(const GameObjectHandle& handle);

		template<IsService T>
		T* AddService()
		{
			ASSERT(!mInitialized, "Cannot Add Serice To Initialized World");
			auto& service = mServices.emplace_back(std::make_unique<T>());
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

		
		GameObjectSlots mSlots;
		std::vector<uint32_t> mFreeSlots;
		std::vector<uint32_t> mToBeDestroyed;

		Services mServices;

	};
}
