#pragma once

namespace NEngine::Graphics
{
	struct Model;
	namespace ModelIO
	{
		void SaveModel(std::filesystem::path filepath, const Model& model);
		void LoadModel(std::filesystem::path filepath, Model& model);

		void SaveMaterial(std::filesystem::path filepath, const Model& model);
		void LoadMaterial(std::filesystem::path filepath, Model& model);
	}
}
