#pragma once

#include "Transform.h"
#include "Matrix4x4.h"


class Camera
{
public:
	Camera(float InFOV, float InNear, float InFar, float InRepAspectRatio);

	
	Matrix4x4 GetViewMatrix();

	Matrix4x4 GetLookAtMatrix(Vector3 LookAtPos);

private:
	// 투영 행렬 계산
	void CalcProjectionMat();


public:
	Transform TransformData;
	float FOV;
	float Near;
	float Far;
	float RepAspectRatio;

private:
	Matrix4x4 ProjectionMat;
};

