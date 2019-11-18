#pragma once

#include "Vector4.h"


struct BoundingBox
{
public:
	BoundingBox() = default;


	void CalcBoungingBox(Vector4 * InVertices, int VertexCount);

public:
	Vector3 Center;
	Vector3 Extent;
};
