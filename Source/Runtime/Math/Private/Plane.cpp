#include "Precompiled.h"
#include "Plane.h"

Plane::Plane(const Vector3 & InP1, const Vector3 & InP2, const Vector3 & InP3)
{
	Vector3 v1 = (InP2 - InP1).Normalize();
	Vector3 v2 = (InP3 - InP1).Normalize();
	
	Normal = v1.Cross(v2);
	D = -Normal.Dot(InP1);
}
