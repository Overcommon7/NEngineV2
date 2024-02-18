#pragma once
#include "Component.h"
namespace NEngine
{
	class SoundEffectComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::SoundEffect);

		void Initialize() override;
		void Terminate() override;
		void Deserialize(rapidjson::Value& value) override;

		void Play();
		void Stop();

		void DebugUI() override;
	private:
		bool mLooping;
		std::string mFilename;
		Audio::SoundId mSoundId;
	};
}


