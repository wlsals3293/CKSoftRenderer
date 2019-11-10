#pragma once

#include "Vector3.h"
#include "Matrix4x4.h"


class Transform
{
public:
	Transform() {}

	Matrix4x4 GetTRS();


public:
	Vector3 Position = Vector3::Zero;
	Vector3 Rotaiton = Vector3::Zero;
	Vector3 Scale = Vector3::One;

};

