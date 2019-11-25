#pragma once

#include "RenderingSoftwareInterface.h"
#include "InputManager.h"

class SoftRendererImpl3D
{
public:
	SoftRendererImpl3D(class SoftRenderer* InOwner);
	~SoftRendererImpl3D();

public:
	void LoadResource();
	void LoadScene();
	void RenderFrameImpl();
	void UpdateImpl(float DeltaSeconds);

private:
	FORCEINLINE void DrawGizmo3D(Matrix4x4 InVMatrix, Matrix4x4 InPMatrix);
	FORCEINLINE void DrawXYPlane(Matrix4x4 InVMatrix, Matrix4x4 InPMatrix);

private:
	RenderingSoftwareInterface* RSI = nullptr;

	// 2D Grid Options
	int Grid2DUnit = 10;
	ScreenPoint ScreenSize;

	// Input Manager
	InputManager InputManager;

	// Resource
	std::unique_ptr<Mesh> SharedCubeMesh;

	// Camera
	Camera* MainCamera;

	// GameObject
	std::vector<std::unique_ptr<GameObject>> Scene;
};
