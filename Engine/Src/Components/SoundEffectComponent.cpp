#include "Precompiled.h"
#include "Components/SoundEffectComponent.h"

#include "GameObject.h"
#include "SaveUtil.h"

using namespace NEngine::Audio;

namespace NEngine
{
	void SoundEffectComponent::Initialize()
	{
		ASSERT(!mFilename.empty(), "SoundEffectComponent: No Sound file loaded");
		mSoundId = SoundEffectManager::Get()->Load(mFilename);
	}
	void SoundEffectComponent::Terminate()
	{
	}
	void SoundEffectComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
	{
		rapidjson::Value componentValue(rapidjson::kObjectType);
		SaveUtil::SaveString("Filename", mFilename.c_str(), doc, componentValue);
		SaveUtil::SaveBool("Looping", mLooping, doc, componentValue);
		value.AddMember("SoundEffect", componentValue, doc.GetAllocator());
	}
	void SoundEffectComponent::Deserialize(rapidjson::Value& value)
	{  
		if (value.HasMember("Filename"))
			mFilename = value["Filename"].GetString();

		if (value.HasMember("Looping"))
			mLooping = value["Looping"].GetBool();
		else mLooping = false;
	}
	void SoundEffectComponent::Play()
	{
		SoundEffectManager::Get()->Play(mSoundId, mLooping);
	}
	void SoundEffectComponent::Stop()
	{
		SoundEffectManager::Get()->Stop(mSoundId);
	}
	void SoundEffectComponent::DebugUI()
	{

	}
}