#pragma once

#include "TypeIds.h"

namespace NEngine
{
	class Service
	{
	public:

		static uint32_t StaticGetTypeId() { return (uint32_t)ServiceId::Invalid; }
		virtual uint32_t GetTypeId() const = 0;

		virtual void Initialize() {}
		virtual void Terminate() {}

		virtual void Update(float deltaTime) {}
		virtual void Render() {}
		virtual void DebugUI() {}

		Service() = default;
		virtual ~Service() = default;
	private:
	public:
		
		Service(const Service& s) = delete;
		Service(const Service&& s) = delete;
		Service& operator=(const Service& s) = delete;
		Service& operator=(const Service&& s) = delete;

	};
}
