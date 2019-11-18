#include "Precompiled.h"
#include "Sphere.h"


void Sphere::CalcSphere(Vector4 * InVertices, int VertexCount)
{
	Vector4 avgPos = Vector4::Zero;
	for (int i = 0; i < VertexCount; i++)
	{
		avgPos += InVertices[i];
	}
	avgPos /= VertexCount;

	float distance = -1.f;
	for (int i = 0; i < VertexCount; i++)
	{
		if ((InVertices[i] - avgPos).SizeSquared() > distance)
		{
			distance = (InVertices[i] - avgPos).SizeSquared();
		}
	}
	distance = sqrtf(distance);

	Center = avgPos.ToVector3();
	Radius = distance;
}
