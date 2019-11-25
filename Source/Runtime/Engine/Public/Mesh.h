#pragma once

//#include "MathHeaders.h"
//#include "LinearColor.h"
#include "RSIDataTypes.h"


class Mesh
{
public:
	Mesh() = default;
	Mesh(VertexData* InVertexBuffer, int* InIndexBuffer, int InVertexCount, int InIndexCount);
	~Mesh()
	{
		ReleaseAllBuffers();
	}

	VertexData* GetVertexBuffer() const { return Vertices; }
	int* GetIndexBuffer() const { return Indices; }

	int GetVertexCount() { return VertexCount; }
	int GetIndexCount() { return TriangleCount * 3; }
	int GetTriangleCount() { return TriangleCount; }

	void SetMesh(VertexData* InVertexBuffer, int* InIndexBuffer, int InVertexCount, int InIndexCount);

	void ReleaseAllBuffers();

private:
	VertexData* Vertices = nullptr;
	int* Indices = nullptr;
	
	int VertexCount = 0;
	int IndexCount = 0;
	int TriangleCount = 0;

	Sphere BoundSphere;
	BoundingBox BoundBox;
};

