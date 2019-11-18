#include "Precompiled.h"
#include "BoundingBox.h"


void BoundingBox::CalcBoungingBox(Vector4 * InVertices, int VertexCount)
{
	Vector4 avgPos = Vector4::Zero;
	Vector3 minPos = Vector3(9999999.f, 9999999.f, 9999999.f);
	Vector3 maxPos = Vector3(-9999999.f, -9999999.f, -9999999.f);

	for (int i = 0; i < VertexCount; i++)
	{
		avgPos += InVertices[i];

		if (minPos.X > InVertices[i].X)
		{
			minPos.X = InVertices[i].X;
		}
		if (minPos.Y > InVertices[i].Y)
		{
			minPos.Y = InVertices[i].Y;
		}
		if (minPos.Z > InVertices[i].Z)
		{
			minPos.Z = InVertices[i].Z;
		}

		if (maxPos.X < InVertices[i].X)
		{
			maxPos.X = InVertices[i].X;
		}
		if (maxPos.Y < InVertices[i].Y)
		{
			maxPos.Y = InVertices[i].Y;
		}
		if (maxPos.Z < InVertices[i].Z)
		{
			maxPos.Z = InVertices[i].Z;
		}
	}
	avgPos /= VertexCount;
	Extent = (maxPos - minPos) * 0.5f;
	Center = minPos + Extent;
}
