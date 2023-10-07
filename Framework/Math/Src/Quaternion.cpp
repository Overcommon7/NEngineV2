#include "Precompiled.h"
#include "Quaternion.h"
#include "NMath.h"

#include "Constants.h"

using namespace NEngine::NMath;

void Quaternion::Conjugate() noexcept
{

}

void Quaternion::Inverse() noexcept
{
	Conjugate();
	*this = *this / MagnitudeSquared();
}

void Quaternion::Normalize() noexcept
{
	*this = *this / Magnitude();
}

float Quaternion::MagnitudeSquared() const noexcept
{
	return x * x + y * y + z * z + w * w;
}

float Quaternion::Magnitude() const noexcept
{
	return sqrt(MagnitudeSquared());
}

float Quaternion::Dot(const Quaternion& q) const noexcept
{
	return x * q.x + y * q.y + z * q.z + w * q.w;
}

Quaternion Quaternion::Conjugate(const Quaternion& q)
{
	return { -q.x, -q.y, -q.z, q.w };
}

Quaternion Quaternion::Normalize(const Quaternion& q)
{
	return q / q.Magnitude();
}

float Quaternion::Magnitude(const Quaternion& q)
{
	return q.Magnitude();
}

Quaternion Quaternion::CreateFromAxisAngle(const Vector3& axis, float angle) noexcept
{
	const float c = cos(angle * 0.5f);
	const float s = sin(angle * 0.5f);

	const Vector3 a = NMath::Normalize(axis);
	return { a.x * s, a.y * s, a.z * s, c };
}

Quaternion Quaternion::CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept
{
	const float cy = cos(yaw * 0.5f);
	const float sy = sin(yaw * 0.5f);
	const float cp = cos(pitch * 0.5f);
	const float sp = sin(pitch * 0.5f);
	const float cr = cos(roll * 0.5f);
	const float sr = sin(roll * 0.5f);

	return {
		(sr * cp * cy) - (cr * sp * sy),
		(cr * sp * cy) + (sr * cp * sy),
		(cr * cp * sy) - (sr * sp * cy),
		(cr * cp * cy) + (sr * sp * sy)
	};
}

Quaternion Quaternion::CreateFromRotationMatrix(const Matrix4& m) noexcept
{
	const float w = sqrt(m._11 + m._22 + m._33 + 1) * 0.5f;
	const float x = sqrt(m._11 - m._22 - m._33 + 1) * 0.5f;
	const float y = sqrt(m._11 + m._22 - m._33 + 1) * 0.5f;
	const float z = sqrt(m._11 - m._22 + m._33 + 1) * 0.5f;

	Quaternion q;
	if (w >= x && w >= y && w >= z)
	{
		q.w = w;
		q.x = (m._23 - m._32) / (4.f * w);
		q.y = (m._31 - m._13) / (4.f * w);
		q.z = (m._12 - m._21) / (4.f * w);
	}
	else if (x >= w && x >= y && x >= z)
	{
		q.w = (m._23 - m._32) / (4.f * x);
		q.x = x;
		q.y = (m._31 - m._13) / (4.f * w);
		q.z = (m._12 - m._21) / (4.f * w);
	}
	else if (y >= w && y >= x && y >= z)
	{
		q.w = (m._31 - m._13) / (4.f * w); ;
		q.x = (m._12 - m._21) / (4.f * w);
		q.y = y;
		q.z = (m._23 - m._32) / (4.f * x);
	}
	else if (z >= w && z >= x && z >= y)
	{
		q.w = (m._12 - m._21) / (4.f * w);
		q.x = (m._31 - m._13) / (4.f * w);
		q.y = (m._23 - m._32) / (4.f * x);
		q.z = z;
	}

	return q;
}

Quaternion Quaternion::Lerp(const Quaternion& q0, const Quaternion& q1, float t)
{
	return q0 * (1.f - t) + (q1 * t);
}

Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, float t)
{
	float dot = q0.Dot(q1);
	Quaternion q1n = Normalize(q0);
	Quaternion q2n = Normalize(q1);
	if (dot < 0.f)
	{
		dot = -dot;
		q2n.w = -q2n.w;
		q2n.x = -q2n.x;
		q2n.y = -q2n.y;
		q2n.z = -q2n.z;
	}
	else if (dot > 0.9999f)
	{
		return Normalize(Lerp(q0, q1, t));
	}

	dot = std::clamp(dot, -1.f, 1.f);
	double omega = std::acos(dot);

	double sinOmega = std::sin(omega);
	double t1 = std::sin((1.0 - t) * omega) / sinOmega;
	double t2 = std::sin(t * omega) / sinOmega;

	Quaternion result;
	result.w = q1n.w * t1 + q2n.w * t2;
	result.x = q1n.x * t1 + q2n.x * t2;
	result.y = q1n.y * t1 + q2n.y * t2;
	result.z = q1n.z * t1 + q2n.z * t2;

	return Normalize(result);


}