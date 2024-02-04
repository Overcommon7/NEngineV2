#pragma once

#include "TypeIds.h"

namespace NEngine
{
	class Service
	{
		friend class GameWorld;
	public:

		static uint32_t StaticGetTypeId() { return (uint32_t)ServiceId::Invalid; }
		virtual uint32_t GetTypeId() const = 0;

		virtual void Initialize() {}
		virtual void Terminate() {}

		virtual void Update(float deltaTime) {}
		virtual void Render() {}
		virtual void DebugUI() {}

		virtual void Deserialize(rapidjson::Value& value) {}

		GameWorld& GetGameWorld() { return *gameWorld; }
		const GameWorld& GetGameWorld() const { return *gameWorld; }

		Service() = default;
		virtual ~Service() = default;
	private:
		
		GameWorld* gameWorld;
	public:
		
		Service(const Service& s) = delete;
		Service(const Service&& s) = delete;
		Service& operator=(const Service& s) = delete;
		Service& operator=(const Service&& s) = delete;

	};
}
