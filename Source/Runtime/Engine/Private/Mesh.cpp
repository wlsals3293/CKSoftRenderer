#include "Precompiled.h"
#include "Mesh.h"


Mesh::Mesh(VertexData * InVertices, int * InIndices, int InVertexCount, int InTriangleCount)
{
	Vertices = InVertices;
	Indices = InIndices;
	VertexCount = InVertexCount;
	TriangleCount = InTriangleCount;
}

Mesh::~Mesh()
{
	delete[] Vertices;
	delete[] Indices;
}



