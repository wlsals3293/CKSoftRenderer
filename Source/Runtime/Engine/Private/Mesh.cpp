#include "Precompiled.h"
#include "Mesh.h"


Mesh::Mesh(VertexData * InVertices, int * InIndices, int InVertexCount, int InTriangleCount)
{
	Vertices = InVertices;
	Indices = InIndices;
	VertexCount = InVertexCount;
	TriangleCount = InTriangleCount;

	Vector4* vertexPosArray = new Vector4[InVertexCount];
	for (int i = 0; i < InVertexCount; i++)
	{
		vertexPosArray[i] = InVertices[i].Position;
	}

	BoundBox.CalcBoungingBox(vertexPosArray, InVertexCount);
	BoundSphere.CalcSphere(vertexPosArray, InVertexCount);

	delete[] vertexPosArray;
}

Mesh::~Mesh()
{
	delete[] Vertices;
	delete[] Indices;
}



