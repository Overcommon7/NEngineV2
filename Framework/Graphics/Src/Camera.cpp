#include "Precompiled.h"
#include "Camera.h"

#include "GraphicsSystem.h"

using namespace NEngine;
using namespace NEngine::Graphics;

void Camera::SetMode(ProjectionMode mode)
{
	mProjectionMode = mode;
}

void Camera::SetPosition(const NMath::Vector3& position)
{
	mPosition = position;
}

void Camera::SetDirection(const NMath::Vector3& direction)
{
	// Prevent setting direction straight up or down
	auto dir = NMath::Normalize(direction);
	if (NMath::Abs(NMath::Dot(dir, NMath::Vector3::YAxis)) < 0.995f)
		mDirection = dir;
}

void Camera::SetLookAt(const NMath::Vector3& target)
{
	SetDirection(target - mPosition);
}

void Camera::SetFov(float fov)
{
	constexpr float kMinFov = 10.0f * NMath::Constants::DegToRad;
	constexpr float kMaxFov = 170.0f * NMath::Constants::DegToRad;
	mFov = NMath::Clamp(fov, kMinFov, kMaxFov);
}

void Camera::SetAspectRatio(float ratio)
{
	mAspectRatio = ratio;
}

void Camera::SetSize(float width, float height)
{
	mWidth = width;
	mHeight = height;
}

void Camera::SetNearPlane(float nearPlane)
{
	mNearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	mFarPlane = farPlane;
}

void Camera::Walk(float distance)
{
	mPosition += mDirection * distance;
}

void Camera::Strafe(float distance)
{
	const NMath::Vector3 right = NMath::Normalize(Cross(NMath::Vector3::YAxis, mDirection));
	mPosition += right * distance;
}

void Camera::Rise(float distance)
{
	mPosition += NMath::Vector3::YAxis * distance;
}

void Camera::Yaw(float radian)
{
	NMath::Matrix4 matRotate = NMath::Matrix4::RotationY(radian);
	mDirection = NMath::TransformNormal(mDirection, matRotate);
}

void Camera::Pitch(float radian)
{
	const NMath::Vector3 right = NMath::Normalize(Cross(NMath::Vector3::YAxis, mDirection));
	const NMath::Matrix4 matRot = NMath::Matrix4::RotationAxis(right, radian);
	const NMath::Vector3 newLook = NMath::TransformNormal(mDirection, matRot);
	SetDirection(newLook);
}

void Camera::Zoom(float amount)
{
	constexpr float minZoom = 170.0f * NMath::Constants::DegToRad;
	constexpr float maxZoom = 10.0f * NMath::Constants::DegToRad;
	mFov = NMath::Clamp(mFov - amount, maxZoom, minZoom);
}

const NMath::Vector3& Camera::GetPosition() const
{
	return mPosition;
}

const NMath::Vector3& Camera::GetDirection() const
{
	return mDirection;
}

NMath::Matrix4 Camera::GetViewMatrix() const
{
	const NMath::Vector3 l = mDirection;
	const NMath::Vector3 r = NMath::Normalize(NMath::Cross(NMath::Vector3::YAxis, l));
	const NMath::Vector3 u = NMath::Normalize(NMath::Cross(l, r));
	const float x = -NMath::Dot(r, mPosition);
	const float y = -NMath::Dot(u, mPosition);
	const float z = -NMath::Dot(l, mPosition);

	return
	{
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		x,   y,   z,   1.0f
	};
}

NMath::Matrix4 Camera::GetProjectionMatrix() const
{
	return (mProjectionMode == ProjectionMode::Perspective) ? GetPerspectiveMatrix() : GetOrthographicMatrix();
}

NMath::Matrix4 Camera::GetPerspectiveMatrix() const
{
	const float a = (mAspectRatio == 0.0f) ? GraphicsSystem::Get()->GetBackBufferAspectRatio() : mAspectRatio;
	const float h = 1.0f / tan(mFov * 0.5f);
	const float w = h / a;
	const float zf = mFarPlane;
	const float zn = mNearPlane;
	const float q = zf / (zf - zn);

	return {
		w,    0.0f, 0.0f,    0.0f,
		0.0f, h,    0.0f,    0.0f,
		0.0f, 0.0f, q,       1.0f,
		0.0f, 0.0f, -zn * q, 0.0f
	};
}

NMath::Matrix4 Camera::GetOrthographicMatrix() const
{
	const float w = (mWidth == 0.0f) ? GraphicsSystem::Get()->GetBackBufferWidth() : mWidth;
	const float h = (mHeight == 0.0f) ? GraphicsSystem::Get()->GetBackBufferHeight() : mHeight;
	const float f = mFarPlane;
	const float n = mNearPlane;
	return 	{
		2 / w, 0.0f,  0.0f,        0.0f,
		0.0f,  2 / h, 0.0f,        0.0f,
		0.0f,  0.0f,  1 / (f - n), 0.0f,
		0.0f,  0.0f,  n / (n - f), 1.0f
	};
}