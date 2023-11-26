#pragma once
#include "PhysicsObject.h"

#include "../Framework/Graphics/Inc/MeshTypes.h"

namespace NEngine::Physics
{
	class SoftBody final : public PhysicsObject
	{
	public:
		SoftBody() = default;
		~SoftBody();

		void Initialize(NEngine::Graphics::Mesh& mesh, float mass, const vector<int>& fixedNodeIndices);
		void Terminate();

		void Update();
		btSoftBody* GetSoftBody() const override { return mSoftBody; }

	private:
		btSoftBody* mSoftBody = nullptr;
		NEngine::Graphics::Mesh* mMesh = nullptr;

	};
}


