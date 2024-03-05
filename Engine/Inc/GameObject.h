#pragma once
#include "Component.h"
#include "GameEngineHandle.h"

namespace NEngine
{			
	template<class T> 
	concept ComponentType = std::is_base_of_v<Component, T>;
	class GameWorld;
	
	class GameObject final
	{
		friend class GameWorld;
	public:
		GameObject() = default;

		void Initialize();
		void Terminate();

		void DebugUI();
		void EditorUI();

		void SetName(const string& name) { mName = name; }
		std::string_view GetName() const { return mName; }
		uint32_t GetUniqueId() const { return mUniqueId; }
		const std::filesystem::path GetTemplatePath() const { return templateFile; }
		void SetTemplateFilePath(const std::filesystem::path& file) { templateFile = file; }
		const GameObjectHandle& GetHandle() const { return mHandle; }

		GameWorld& GetWorld() { return *mWorld; }
		void Serialize(rapidjson::Document& doc);

	private:
		GameWorld* mWorld = nullptr;
		GameObjectHandle mHandle;
		string mName = "EMPTY";
		bool mInitialized = false;
		uint32_t mUniqueId;

		using Components = vector<std::unique_ptr<Component>>;
		Components mComponents = {};
		inline static uint32_t sUniqueId = 0;
		std::filesystem::path templateFile = "";

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
		bool HasComponent() const
		{
			for (auto& component : mComponents)
			{
				if (component->GetTypeId() == T::StaticGetTypeId())
				{
					return true;
				}
			}
		}

		template<ComponentType T>
		T* GetComponent()
		{
			for (auto& component : mComponents)
			{
				if (component->GetTypeId() == T::StaticGetTypeId())
				{
					return static_cast<T*>(component.get());
				}
			}

			return nullptr;
		}

		template<ComponentType T>
		const T* GetComponent()	const
		{
			for (auto& component : mComponents)
			{
				if (component->GetTypeId() == T::StaticGetTypeId())
				{
					return static_cast<T*>(component.get());
				}
			}

			return nullptr;
		}

	};
}


