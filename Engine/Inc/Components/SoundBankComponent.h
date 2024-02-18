#pragma once
#include "Component.h"

namespace NEngine
{
	class SoundBankComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::SoundBank);

		void Initialize() override;
		void Terminate() override;
		void Deserialize(rapidjson::Value& value) override;

		void Play(const string& key);
		void Stop(const string& key);
		void StopAll();

		void DebugUI() override;
	private:

		struct SoundData
		{
			std::string fileName;
			Audio::SoundId soundId;
			bool isLooping;
		};

		using SoundEffects = std::unordered_map<string, SoundData>;
		SoundEffects mSoundEffects;
	};
}


