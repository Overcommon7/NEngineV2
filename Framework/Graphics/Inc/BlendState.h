#pragma once
namespace NEngine::Graphics {
	class BlendState final {
	public:
		static void ClearState();

		enum class Mode {
			Opaque,
			AlphaBlend,
			AlphaPreMultiplied,
			Additive
		};

		BlendState() = default;
		~BlendState();

		BlendState(const BlendState&) = delete;
		BlendState& operator=(const BlendState&) = delete;

		void Initailize(Mode mode);
		void Terminate();

		void Set();
	private:

		ID3D11BlendState* mBlendState = nullptr;
	};
}