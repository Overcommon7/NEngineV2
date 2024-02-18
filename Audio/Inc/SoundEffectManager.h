#pragma once
namespace NEngine::Audio
{
	using SoundId = std::size_t;

	class SoundEffectManager final
	{
	public:
		static void StaticInitialize(const std::filesystem::path& root);
		static void StaticTerminate();
		static SoundEffectManager* Get();

		void SetRootPath(const std::filesystem::path& root);
		SoundId Load(const std::filesystem::path& soundFile);
		void Clear();

		void Play(SoundId id, bool loop = false);
		void Stop(SoundId id);

		SoundEffectManager() = default;
		~SoundEffectManager();

	private:
		struct Entry
		{
			std::unique_ptr<DirectX::SoundEffect> effect;
			std::unique_ptr<DirectX::SoundEffectInstance> instance;
		};

		using SoundEffects = std::unordered_map<SoundId, std::unique_ptr<Entry>>;

		SoundEffects mInventory;
		std::filesystem::path mRoot;
	public:
		SoundEffectManager(const SoundEffectManager& s) = delete;
		SoundEffectManager(const SoundEffectManager&& s) = delete;
		SoundEffectManager& operator=(const SoundEffectManager& s) = delete;
		SoundEffectManager& operator=(const SoundEffectManager&& s) = delete;

	};
}


