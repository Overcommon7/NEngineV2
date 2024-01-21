#pragma once
#include "TypeIds.h"

namespace NEngine
{
	class GameObject;

	class Component
	{
	public:
		virtual void Initialize() {}
		virtual void Terminate() {}
		virtual void Update() {}
		virtual void DebugUI() {}

		virtual uint32_t GetTypeId() const = 0;
		static uint32_t StaticGetTypeId() { return (uint32_t)ComponentId::Invalid; }
		GameObject& GetOwner() { return *mOwner; }
		const GameObject& GetOwner() const { return *mOwner; }

		virtual void Deserialize(rapidjson::Value& value) {}
		virtual void Serialize(rapidjson::Document& doc, rapidjson::Value& value) {}

	protected:
		Component() = default;
		

	public:
		virtual ~Component() = default;
		Component(const Component& c) = delete;
		Component(const Component&& c) = delete;
		Component& operator=(const Component& c) = delete;
		Component& operator=(const Component&& c) = delete;
	private:
		friend class GameObject;
		GameObject* mOwner = nullptr;

	};
}


