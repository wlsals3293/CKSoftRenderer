#pragma once

#include "Transform.h"
#include "Mesh.h"


class GameObject
{
public:
	GameObject() = default;

	Transform& GetTransform() { return TransformData; }
	void SetMesh(Mesh *InMesh) { MeshData = InMesh; }
	Mesh* GetMesh() { return MeshData; }

private:
	Transform TransformData;
	Mesh* MeshData = nullptr;
};

