#pragma once
#include "Component.h"

namespace NEngine
{
	class MeshComponent	final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Model);

		void Initialize() override;
		void Terminate() override;
		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void Deserialize(rapidjson::Value& value) override;

		void EditorUI() override;

		bool CastShadow() const { return mCastShadow; }
		const Graphics::Model& GetModel() const { return mModel; }

	private:
		Graphics::Model mModel;
		bool mCastShadow = false;

		struct LoadingData
		{
			string shapeType;
			float fParam;
			int iParam0;
			int iParam1;
		};

		LoadingData mLoadingData;
	};
}


