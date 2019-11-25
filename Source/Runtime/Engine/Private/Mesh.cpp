#include "Precompiled.h"
#include "Mesh.h"


Mesh::Mesh(VertexData * InVertexBuffer, int * InIndexBuffer, int InVertexCount, int InIndexCount)
{
	SetMesh(InVertexBuffer, InIndexBuffer, InVertexCount, InIndexCount);
}

void Mesh::SetMesh(VertexData * InVertexBuffer, int * InIndexBuffer, int InVertexCount, int InIndexCount)
{
	Vertices = InVertexBuffer;
	Indices = InIndexBuffer;
	VertexCount = InVertexCount;
	IndexCount = InIndexCount;
	TriangleCount = IndexCount / 3;

	Vector4* vertexPosArray = new Vector4[InVertexCount];
	for (int i = 0; i < InVertexCount; i++)
	{
		vertexPosArray[i] = InVertexBuffer[i].Position;
	}

	BoundBox.CalcBoungingBox(vertexPosArray, InVertexCount);
	BoundSphere.CalcSphere(vertexPosArray, InVertexCount);

	delete[] vertexPosArray;
}

void Mesh::ReleaseAllBuffers()
{
	if (nullptr != Vertices)
	{
		delete[] Vertices;
		Vertices = nullptr;
	}

	if (nullptr != Indices)
	{
		delete[] Indices;
		Indices = nullptr;
	}
}



