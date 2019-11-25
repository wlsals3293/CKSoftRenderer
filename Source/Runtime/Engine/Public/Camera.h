#pragma once

#include "Transform.h"
#include "Matrix4x4.h"


class Camera
{
public:
	Camera() = default;
	Camera(float InFOV, float InNear, float InFar, float InRepAspectRatio);

	Transform& GetTransform() { return TransformData; }

	Matrix4x4 GetViewMatrix();
	Matrix4x4 GetProjectionMatrix();

	Matrix4x4 GetLookAtMatrix(Vector3& LookAtPos);

private:
	// 투영 행렬 계산
	void CalcProjectionMat();


public:
	float FOV = 60.f;
	float Near = 5.5f;
	float Far = 100.f;
	float RepAspectRatio = 1.f;

private:
	Transform TransformData;
	Matrix4x4 ProjectionMat;
};

