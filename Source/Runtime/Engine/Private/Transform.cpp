#include "Precompiled.h"
#include "Transform.h"

Matrix4x4 Transform::GetTRS()
{
	float cy, sy, cp, sp, cr, sr;
	Math::GetSinCos(cy, sy, Rotaiton.Y);
	Math::GetSinCos(cp, sp, Rotaiton.X);
	Math::GetSinCos(cr, sr, Rotaiton.Z);

	Matrix4x4 tMat(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(Position));
	Matrix4x4 rMat(
		Vector4(cr * cy + sr * sp * sy, cy * sr - cr * sp * sy, -cp * sy, false),
		Vector4(-sr * cp, cr * cp, -sp, false),
		Vector4(cr * sy - sr * sp * cy, sr * sy + cr * sp * cy, cp * cy, false),
		Vector4::UnitW);

	Matrix4x4 sMat(Vector4::UnitX * Scale.X, Vector4::UnitY * Scale.Y, Vector4::UnitZ * Scale.Z, Vector4::UnitW);
	Matrix4x4 mMat = tMat * rMat * sMat;

	return mMat;
}
