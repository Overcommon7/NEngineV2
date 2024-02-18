#include "Precompiled.h"
#include "AudioSystem.h"

namespace
{
	std::unique_ptr<NEngine::Audio::AudioSystem> sAudioSystem;
}

using namespace DirectX;
using namespace NEngine;
namespace NEngine::Audio
{
	void AudioSystem::StaticInitialize()
	{
		ASSERT(sAudioSystem == nullptr, "AudioSystem: has already been Initialized");
		sAudioSystem = std::make_unique<AudioSystem>();
		sAudioSystem->Initialize();
	}
	void AudioSystem::StaticTerminate()
	{
		if (sAudioSystem != nullptr)
		{
			sAudioSystem->Terminate();
			sAudioSystem.reset();
		}
	}
	AudioSystem* AudioSystem::Get()
	{
		ASSERT(sAudioSystem != nullptr, "AudioSystem: was not initialized");
		return sAudioSystem.get();
	}
	void AudioSystem::Terminate()
	{
		if (mAudioEngine != nullptr)
			delete mAudioEngine;

		mAudioEngine = nullptr;
	}
	void AudioSystem::Initialize()
	{
		ASSERT(mAudioEngine == nullptr, "AudioSystem: is already initialized");
		AUDIO_ENGINE_FLAGS flags = AudioEngine_Debug;
#if defined(_DEBUG)
		flags = flags | AudioEngine_Debug;
#endif

		mAudioEngine = new AudioEngine(flags);
	}
	void AudioSystem::Update()
	{
		if (mAudioEngine != nullptr && mAudioEngine->Update())
		{
			if (mAudioEngine->IsCriticalError())
			{
				LOG("AudioSystem: critical error, shutting down");
				Terminate();
			}
		}
	}
	void AudioSystem::Suspend()
	{
		ASSERT(mAudioEngine != nullptr, "AudioSystem: audio engine is not available");
		mAudioEngine->Suspend();
	}
	AudioSystem::AudioSystem()
		: mAudioEngine(nullptr)
	{
		if (mAudioEngine != nullptr)
		{
			delete mAudioEngine;
		}
		mAudioEngine = nullptr;
	}
	AudioSystem::~AudioSystem()
	{
		ASSERT(mAudioEngine == nullptr, "AudioSystem: terminate was not called");
	}
	DirectX::AudioEngine* AudioSystem::GetAudioEngine()
	{
		return mAudioEngine;
	}
}