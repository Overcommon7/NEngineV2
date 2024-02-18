#include "Precompiled.h"
#include "SoundEffectManager.h"

#include "AudioSystem.h"

using namespace DirectX;
using namespace NEngine;

namespace
{
	std::unique_ptr<Audio::SoundEffectManager> sSoundEffectManager;
}

namespace NEngine::Audio
{
	void SoundEffectManager::StaticInitialize(const std::filesystem::path& root)
	{
		ASSERT(sSoundEffectManager == nullptr, "AudioSystem: has already been Initialized");
		sSoundEffectManager = std::make_unique<SoundEffectManager>();
		sSoundEffectManager->SetRootPath(root);
	}
	void SoundEffectManager::StaticTerminate()
	{
		if (sSoundEffectManager != nullptr)
		{
			sSoundEffectManager->Clear();
			sSoundEffectManager.reset();
		}
	}
	SoundEffectManager* SoundEffectManager::Get()
	{
		ASSERT(sSoundEffectManager != nullptr, "SoundEffectManager: was not initialized");
		return sSoundEffectManager.get();
	}
	void SoundEffectManager::SetRootPath(const std::filesystem::path& root)
	{
		mRoot = root;
	}
	SoundId SoundEffectManager::Load(const std::filesystem::path& soundFile)
	{
		std::filesystem::path fullpath = soundFile;
		if (!std::filesystem::exists(fullpath))
			fullpath = mRoot / fullpath;

		SoundId id = std::filesystem::hash_value(fullpath);
		auto [iter, success] = mInventory.insert({ id, nullptr });
		if (success)
		{
			auto as = AudioSystem::Get();
			auto& soundEventPtr = iter->second;
			soundEventPtr = std::make_unique<Entry>();
			soundEventPtr->effect = std::make_unique<SoundEffect>(as->GetAudioEngine(), fullpath.wstring().c_str());
			soundEventPtr->instance = soundEventPtr->effect->CreateInstance();
		}

		return id;
	}
	void SoundEffectManager::Clear()
	{
		for (auto& entry : mInventory)
		{
			if (!entry.second)
				continue;

			entry.second->instance->Stop();
	    }

		mInventory.clear();
	}
	void SoundEffectManager::Play(SoundId id, bool loop)
	{
		auto it = mInventory.find(id);
		if (it != mInventory.end())
			it->second->instance->Play(loop);
	}
	void SoundEffectManager::Stop(SoundId id)
	{
		auto it = mInventory.find(id);
		if (it != mInventory.end())
			it->second->instance->Stop();
	}
	SoundEffectManager::~SoundEffectManager()
	{
		ASSERT(mInventory.empty(), "SoundEffectManager: clear must be called");
	}
}