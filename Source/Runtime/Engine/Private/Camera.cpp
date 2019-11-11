#include "Precompiled.h"
#include "Camera.h"


Camera::Camera(float InFOV, float InNear, float InFar, float InRepAspectRatio)
{
	FOV = InFOV;
	Near = InNear;
	Far = InFar;
	RepAspectRatio = InRepAspectRatio;

	CalcProjectionMat();
}

Matrix4x4 Camera::GetViewMatrix()
{
	float cy, sy, cp, sp, cr, sr;
	Math::GetSinCos(cy, sy, TransformData.Rotaiton.Y);
	Math::GetSinCos(cp, sp, TransformData.Rotaiton.X);
	Math::GetSinCos(cr, sr, TransformData.Rotaiton.Z);

	Matrix4x4 vitMat(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-TransformData.Position));
	Matrix4x4 virMat(
		Vector4(cr * cy + sr * sp * sy, cy * sr - cr * sp * sy, -cp * sy, false),
		Vector4(-sr * cp, cr * cp, -sp, false),
		Vector4(cr * sy - sr * sp * cy, sr * sy + cr * sp * cy, cp * cy, false),
		Vector4::UnitW);

	return virMat.Tranpose() * vitMat;
}

Matrix4x4 Camera::GetProjectionMatrix()
{
	return ProjectionMat;
}

Matrix4x4 Camera::GetLookAtMatrix(Vector3 LookAtPos)
{
	Vector3 viewZ = (TransformData.Position - LookAtPos).Normalize();
	Vector3 viewX = Vector3::UnitY.Cross(viewZ).Normalize();
	if (viewX.IsZero())
	{
		viewX = Vector3::UnitX;
	}
	Vector3 viewY = viewZ.Cross(viewX).Normalize();

	Matrix4x4 virMat = Matrix4x4(Vector4(viewX, false), Vector4(viewY, false), Vector4(viewZ, false), Vector4::UnitW).Tranpose();
	Matrix4x4 vitMat = Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-TransformData.Position));

	return virMat * vitMat;
}

void Camera::CalcProjectionMat()
{
	float d = 1.f / tanf(Math::Deg2Rad(FOV) * 0.5f);
	float repNF = 1.f / (Near - Far);
	float k = Far * repNF;
	float l = Far * Near * repNF;
	ProjectionMat = Matrix4x4(
		Vector4::UnitX * RepAspectRatio * d,
		Vector4::UnitY * d,
		Vector4(0.f, 0.f, k, -1.f),
		Vector4(0.f, 0.f, l, 0.f)
	);
}
