#include "Precompiled.h"
#include "SoftRendererImpl3D.h"
#include "SoftRenderer.h"
#include "Camera.h"
#include "GameObject.h"

SoftRendererImpl3D::SoftRendererImpl3D(SoftRenderer* InOwner)
{
	RSI = InOwner->RSI.get();
	ScreenSize = InOwner->CurrentScreenSize;
	InputManager = InOwner->GetInputManager();

	LoadResource();
	LoadScene();
}

SoftRendererImpl3D::~SoftRendererImpl3D()
{
}


void SoftRendererImpl3D::LoadResource()
{
	// 카메라 생성
	float repA = (float)ScreenSize.Y / (float)ScreenSize.X;
	MainCamera = new Camera(60.f, 5.5f, 100.f, repA);

	// 오브젝트 생성
	Cube = new GameObject[2];

	// 메쉬 생성
	const int vertexCount = 24;
	VertexData* v = new VertexData[vertexCount]{
		// Front 
		VertexData(Vector4(0.5f, -0.5f, 0.5f)),
		VertexData(Vector4(0.5f, 0.5f, 0.5f)),
		VertexData(Vector4(0.5f, 0.5f, -0.5f)),
		VertexData(Vector4(0.5f, -0.5f, -0.5f)),
		// Left
		VertexData(Vector4(-0.5f, -0.5f, 0.5f)),
		VertexData(Vector4(-0.5f, 0.5f, 0.5f)),
		VertexData(Vector4(0.5f, 0.5f, 0.5f)),
		VertexData(Vector4(0.5f, -0.5f, 0.5f)),
		// Right
		VertexData(Vector4(0.5f, -0.5f, -0.5f)),
		VertexData(Vector4(0.5f, 0.5f, -0.5f)),
		VertexData(Vector4(-0.5f, 0.5f, -0.5f)),
		VertexData(Vector4(-0.5f, -0.5f, -0.5f)),
		// Back
		VertexData(Vector4(-0.5f, -0.5f, -0.5f)),
		VertexData(Vector4(-0.5f, 0.5f, -0.5f)),
		VertexData(Vector4(-0.5f, 0.5f, 0.5f)),
		VertexData(Vector4(-0.5f, -0.5f, 0.5f)),
		// Top
		VertexData(Vector4(0.5f, 0.5f, 0.5f)),
		VertexData(Vector4(-0.5f, 0.5f, 0.5f)),
		VertexData(Vector4(-0.5f, 0.5f, -0.5f)),
		VertexData(Vector4(0.5f, 0.5f, -0.5f)),
		// Bottom
		VertexData(Vector4(-0.5f, -0.5f, 0.5f)),
		VertexData(Vector4(0.5f, -0.5f, 0.5f)),
		VertexData(Vector4(0.5f, -0.5f, -0.5f)),
		VertexData(Vector4(-0.5f, -0.5f, -0.5f))
	};

	const int triangleCount = 12;
	const int indexCount = triangleCount * 3;
	int* i = new int[indexCount] {
		0, 2, 1, 0, 3, 2,
		4, 6, 5, 4, 7, 6,
		8, 10, 9, 8, 11, 10,
		12, 14, 13, 12, 15, 14,
		16, 18, 17, 16, 19, 18,
		20, 22, 21, 20, 23, 22
	};

	// 오브젝트에 메쉬 적용
	Cube[0].SetMesh(new Mesh(v, i, vertexCount, triangleCount));
	Cube[1].SetMesh(new Mesh(v, i, vertexCount, triangleCount));
}

void SoftRendererImpl3D::LoadScene()
{
	MainCamera->TransformData.Position = Vector3(0.f, 500.f, -500.f);
	Cube[0].TransformData.Scale = Vector3(100.f, 100.f, 100.f);
	Cube[1].TransformData.Scale = Vector3(100.f, 100.f, 100.f);
	Cube[1].TransformData.Position = Vector3(200.f, 0.f, 0.f);
}

void SoftRendererImpl3D::RenderFrameImpl()
{
	assert(RSI != nullptr && RSI->IsInitialized() && !ScreenSize.HasZero());

	Matrix4x4 m[3] = {
		Matrix4x4(),
		MainCamera->GetLookAtMatrix(Cube[0].TransformData.Position),
		MainCamera->GetProjectionMatrix()
	};

	for (int i = 0; i < 2; i++)
	{
		m[0] = Cube[i].TransformData.GetTRS();
		RSI->SetUniformMatrix(m);
		RenderObject(&Cube[i]);
	}
}

void SoftRendererImpl3D::UpdateImpl(float DeltaSeconds)
{
	static float moveSpeed = 100.f;
	static float rotateSpeed = 180.f;

	Cube[0].TransformData.Position += Vector3::UnitZ * InputManager.GetYAxis() * moveSpeed * DeltaSeconds;
	Cube[0].TransformData.Rotaiton.Y += InputManager.GetXAxis() * rotateSpeed * DeltaSeconds;
	Cube[1].TransformData.Rotaiton.Y -= InputManager.GetXAxis() * rotateSpeed * 2 * DeltaSeconds;
}

void SoftRendererImpl3D::RenderObject(GameObject * InObject)
{
	Mesh* meshToRender = InObject->MeshData;
	int vertexCount = meshToRender->GetVertexCount();
	int indexCount = meshToRender->GetIndexCount();
	
	RSI->SetVertexBuffer(meshToRender->GetVertices());
	RSI->SetIndexBuffer(meshToRender->GetIndices());
	RSI->DrawPrimitive(vertexCount, indexCount);
}

