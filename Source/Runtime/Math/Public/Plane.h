#pragma once

#include "Vector3.h"


struct Plane
{
public:
	Plane() = default;
	Plane(const Vector3 & InNormal, float InD) : Normal(InNormal), D(InD) {}
	Plane(const Vector3 & InP1, const Vector3 & InP2, const Vector3 & InP3);


public:
	Vector3 Normal = Vector3::UnitY;
	float D = 0.f;
};
