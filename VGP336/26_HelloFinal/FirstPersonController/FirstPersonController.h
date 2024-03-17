#pragma once
#include "CustomFactory.h"
#include "FirstPersonCameraUpdater.h"
#include "FirstPersonMovement.h"

class FirstPersonController final : public Component    
{
	struct Values
	{
		bool useOrbitCamera = false;
		float orbitCameraSpeed = 15.f;
		Vector3 orbitCameraPosition = { 0, 0, 0 };
		Vector3 orbitCameraDirection = { 0, 0, 1 };
		float rotationOffset = 90.f;
	};
public:
	SET_TYPE_ID(CustomComponentId::FirstPersonController);
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void DebugUI() override;

	void Deserialize(rapidjson::Value& value) override;
	void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
private:
	FirstPersonCameraUpdater::Values mCameraValues;
	FirstPersonMovement::Values mMovementValues;
	Values mValues;

	CameraComponent* mCameraComponent;
	AnimatorComponent* mAnimatorComponent;
	RigidbodyComponent* mRigidbodyComponent;

	Camera* mCamera;
	Animator* mAnimator;
	Rigidbody* mRigidbody;
	NEngine::Transform* mTransform;
	NEngine::Transform* mCameraTransform;

	static inline bool imGuiHovered = false;
private:
	void OrbitCameraUpdate(float deltaTime);
	void OnCameraSwitch();
	void UpdateRotation();
};

