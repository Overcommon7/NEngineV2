#pragma once
#include "CustomFactory.h"
#include "FirstPersonCameraUpdater.h"
#include "FirstPersonMovement.h"

class FirstPersonController final : public NEngine::Component    
{
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

	CameraComponent* mCameraComponent;
	AnimatorComponent* mAnimatorComponent;
	RigidbodyComponent* mRigidbodyComponent;

	Camera* mCamera;
	Animator* mAnimator;
	Rigidbody* mRigidbody;
};

