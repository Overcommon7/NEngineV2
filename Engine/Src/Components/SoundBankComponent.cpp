#include "Precompiled.h"
#include "Components/SoundBankComponent.h"

#include "SaveUtil.h"

namespace NEngine
{
	void SoundBankComponent::Initialize()
	{
		auto SFXmanager = Audio::SoundEffectManager::Get();
		for (auto& [name, soundData] : mSoundEffects)
		{
			soundData.soundId = SFXmanager->Load(soundData.fileName);
		}
	}
	void SoundBankComponent::Terminate()
	{
	}
	void SoundBankComponent::Deserialize(rapidjson::Value& value)
	{
		if (value.HasMember("SoundEffects"))
		{
			auto soundEffects = value["SoundEffects"].GetObj();
			for (auto& soundFX : soundEffects)
			{
				SoundData& soundData = mSoundEffects[soundFX.name.GetString()];
				if (soundFX.value.HasMember("Filename"))
				{
					soundData.fileName = soundFX.value["Filename"].GetString();
				}
				if (soundFX.value.HasMember("Looping"))
				{
					soundData.isLooping = soundFX.value["Looping"].GetBool();
				}
			}
		}
	}
	void SoundBankComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
	{
		rapidjson::Value componentValue(rapidjson::kObjectType);
		for (auto& [name, data] : mSoundEffects)
		{
			rapidjson::Value effectValue(rapidjson::kObjectType);
			SaveUtil::SaveString("Filename", data.fileName.c_str(), doc, effectValue);
			SaveUtil::SaveBool("Looping", data.isLooping, doc, effectValue);
			componentValue.AddMember(GetStringRef(name.c_str()), effectValue, doc.GetAllocator());
		}
		value.AddMember("SoundBank", componentValue, doc.GetAllocator());
	}
	void SoundBankComponent::Play(const string& key)
	{
		auto iter = mSoundEffects.find(key);
		if (iter == mSoundEffects.end())
			return;


		auto SFXmanager = Audio::SoundEffectManager::Get();
		const auto& soundData = iter->second;
		SFXmanager->Play(soundData.soundId, soundData.isLooping);
	}
	void SoundBankComponent::Stop(const string& key)
	{
		auto iter = mSoundEffects.find(key);
		if (iter == mSoundEffects.end())
			return;


		auto SFXmanager = Audio::SoundEffectManager::Get();
		const auto& soundData = iter->second;
		SFXmanager->Stop(soundData.soundId);
	}
	void SoundBankComponent::StopAll()
	{
	}
	void SoundBankComponent::DebugUI()
	{
		auto SFXmanager = Audio::SoundEffectManager::Get();

		for (auto& [name, soundData] : mSoundEffects)
		{
			if (ImGui::Button(name.c_str()))
			{
				SFXmanager->Stop(soundData.soundId);
				SFXmanager->Play(soundData.soundId, soundData.isLooping);
			}
		}
			
	}
}