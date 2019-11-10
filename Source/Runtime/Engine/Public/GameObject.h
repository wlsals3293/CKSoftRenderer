#pragma once

#include "Transform.h"
#include "Mesh.h"


class GameObject
{
public:
	GameObject() { MeshData = nullptr; }

	void SetMesh(Mesh *InMesh) { MeshData = InMesh; }

public:
	Transform TransformData;
	Mesh* MeshData;
};

