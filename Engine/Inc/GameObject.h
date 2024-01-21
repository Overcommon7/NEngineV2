#pragma once
#include "Component.h"

namespace NEngine
{			
	template<class T> 
	concept ComponentType = std::is_base_of_v<Component, T>;

	class GameObject final
	{
	public:
		GameObject() = default;

		void Initialize();
		void Terminate();

		void DebugUI();

		void SetName(const string& name) { mName = name; }
		std::string_view GetName() const { return mName; }
		uint32_t GetUniqueId() const { return mUniqueId; }

	private:
		string mName = "EMPTY";
		bool mInitialized = false;
		uint32_t mUniqueId;

		using Components = vector<std::unique_ptr<Component>>;
		Components mComponents = {};
		inline static uint32_t sUniqueId = 0;

	public:
		template<ComponentType T>
		T* AddComponent()
		{
			ASSERT(!mInitialized, "Cannot add component to itialized Obeject");

			auto& newComponent = mComponents.emplace_back(std::make_unique<T>());
			newComponent->mOwner = this;
			return static_cast<T*>(newComponent.get());
		}

		template<ComponentType T>
		bool HasComponent()
		{
			for (auto& component : mComponents)
			{
				if (component->GetTypeId() == T::StaticGetTypeId())
				{
					return true;
				}
			}
		}




	};
}


