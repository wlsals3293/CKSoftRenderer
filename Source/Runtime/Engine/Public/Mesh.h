#pragma once

#include "MathHeaders.h"
#include "LinearColor.h"
#include "RSIDataTypes.h"


/*struct VertexData
{
	VertexData(Vector3 InPosition = Vector3::Zero, Vector2 InUV = Vector2::Zero,
		LinearColor InColor = LinearColor::Black) :
		Position(InPosition), UV(InUV), Color(InColor) {}

	Vector3 Position;
	Vector2 UV;
	LinearColor Color;
};*/


class Mesh
{
public:
	Mesh(VertexData* InVertices, int* InIndices, int InVertexCount, int InTriangleCount);
	~Mesh();

	FORCEINLINE VertexData* GetVertices() const { return Vertices; }
	FORCEINLINE int* GetIndices() const { return Indices; }

	FORCEINLINE int GetVertexCount() { return VertexCount; }
	FORCEINLINE int GetIndexCount() { return TriangleCount * 3; }
	FORCEINLINE int GetTriangleCount() { return TriangleCount; }

private:
	VertexData* Vertices;
	int* Indices;
	
	int VertexCount;
	int TriangleCount;

	Sphere BoundSphere;
	BoundingBox BoundBox;
};

