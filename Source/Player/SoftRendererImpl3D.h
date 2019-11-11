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


	void RenderObject(class GameObject* InObject);

private:
	RenderingSoftwareInterface* RSI = nullptr;

	// 2D Grid Options
	int Grid2DUnit = 10;
	ScreenPoint ScreenSize;

	// Input Manager
	InputManager InputManager;

	// Resource
	//VertexData* v = nullptr;
	//int* i = nullptr;

	// Camera
	class Camera* MainCamera;

	// GameObject
	GameObject* Cube;
};
