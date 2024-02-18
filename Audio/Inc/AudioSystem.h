#pragma once
namespace NEngine::Audio
{
	class AudioSystem final
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static AudioSystem* Get();

		void Terminate();
		void Initialize();

		void Update();
		void Suspend();

		AudioSystem();
		~AudioSystem();

		DirectX::AudioEngine* GetAudioEngine();
	private:
		friend class SoundEffectManager;
		DirectX::AudioEngine* mAudioEngine = nullptr;


	public:
		AudioSystem(const AudioSystem& a) = delete;
		AudioSystem(const AudioSystem&& a) = delete;
		AudioSystem& operator=(const AudioSystem& a) = delete;
		AudioSystem& operator=(const AudioSystem&& a) = delete;
	};
}


