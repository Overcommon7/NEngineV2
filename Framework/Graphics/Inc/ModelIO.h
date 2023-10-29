#pragma once

namespace NEngine::Graphics
{
	struct Model;
	class Animation;

	class AnimationIO
	{
	public:
		static void Write(FILE* file, const Animation& animation);
		static void Read(FILE* file, Animation& animation);
	};

	namespace ModelIO
	{
		void SaveSkeleton(std::filesystem::path filepath, const Model& model);
		void LoadSkeleton(std::filesystem::path filepath, Model& model);

		void SaveModel(std::filesystem::path filepath, const Model& model);
		void LoadModel(std::filesystem::path filepath, Model& model);

		void SaveMaterial(std::filesystem::path filepath, const Model& model);
		void LoadMaterial(std::filesystem::path filepath, Model& model);

		void SaveAnimations(std::filesystem::path filepath, const Model& model);
		void LoadAnimations(std::filesystem::path filepath, Model& model);
	}
}
