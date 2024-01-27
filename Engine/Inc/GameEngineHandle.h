#pragma once

namespace NEngine
{
	class GameObjectHandle
	{
		friend class GameWorld;
	public:
		GameObjectHandle() = default;
	private:
		int mIndex = -1;
		int mGeneration = -1;

	};
}
