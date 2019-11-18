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

	Matrix4x4 vMat = MainCamera->GetLookAtMatrix(Cube[0].TransformData.Position);
	Matrix4x4 pMat = MainCamera->GetProjectionMatrix();
	Matrix4x4 m[3] = { Matrix4x4(),	vMat, pMat };

	for (int i = 0; i < 2; i++)
	{
		m[0] = Cube[i].TransformData.GetTRS();
		RSI->SetUniformMatrix(m);
		RenderObject(&Cube[i]);
	}

	DrawGizmo3D(vMat, pMat);
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

void SoftRendererImpl3D::DrawGizmo3D(Matrix4x4 InVMatrix, Matrix4x4 InPMatrix)
{
	const int gizmosVertexCount = 6;
	VertexData v[gizmosVertexCount] = {
		VertexData(Vector3::Zero, LinearColor::Red),
		VertexData(Vector3::UnitX * 500.f, LinearColor::Red),
		VertexData(Vector3::Zero, LinearColor::Green),
		VertexData(Vector3::UnitY * 500.f, LinearColor::Green),
		VertexData(Vector3::Zero, LinearColor::Blue),
		VertexData(Vector3::UnitZ * 500.f, LinearColor::Blue),
	};

	const int gizmosLineCount = 3;
	const int gizmosIndexCount = gizmosLineCount * 2;
	int i[gizmosIndexCount] = {
		0, 1,
		2, 3,
		4, 5
	};

	Matrix4x4 m[3] = { Matrix4x4(), InVMatrix, InPMatrix };

	RSI->SetUniformMatrix(m);
	RSI->SetVertexBuffer(v);
	RSI->SetIndexBuffer(i);
	RSI->DrawLinePrimitive(gizmosVertexCount, gizmosIndexCount, 1);
}

void SoftRendererImpl3D::DrawXYPlane(Matrix4x4 InVMatrix, Matrix4x4 InPMatrix)
{
	//float planeLineHalfSize = 500.f;
	//const int planeSideLineCount = 50;
	//const int planeLineCount = planeSideLineCount * 2 + 1;
	//const int planeIndexCount = planeLineCount * 2;
	//const int planeVertexCount = planeLineCount * 2;

	//VertexData v[planeVertexCount];
	//int i[planeIndexCount];

	//for (int vi = 0; vi < planeLineCount / 4; ++vi)
	//{
	//	if (vi == 0)
	//	{
	//		v[0] = VertexData(Vector4(Vector3(-planeLineHalfSize, 0.f, 0.f)), LinearColor(0.7f, 0.7f, 0.7f));
	//		v[1] = VertexData(Vector4(Vector3(planeLineHalfSize, 0.f, 0.f)), LinearColor(0.7f, 0.7f, 0.7f));
	//		v[2] = VertexData(Vector4(Vector3(0.f, 0.f, -planeLineHalfSize)), LinearColor(0.7f, 0.7f, 0.7f));
	//		v[3] = VertexData(Vector4(Vector3(0.f, 0.f, planeLineHalfSize)), LinearColor(0.7f, 0.7f, 0.7f));

	//		i[0] = 0;
	//		i[1] = 1;
	//		i[2] = 2;
	//		i[3] = 3;
	//	}
	//	else
	//	{
	//		v[vi * 4] = VertexData(Vector4(Vector3(-planeLineHalfSize, 0.f, 10.f * vi)), LinearColor(0.7f, 0.7f, 0.7f));
	//		v[vi * 4 + 1] = VertexData(Vector4(Vector3(planeLineHalfSize, 0.f, 10.f * vi)), LinearColor(0.7f, 0.7f, 0.7f));
	//		v[vi * 4 + 2] = VertexData(Vector4(Vector3(-planeLineHalfSize, 0.f, -10.f * vi)), LinearColor(0.7f, 0.7f, 0.7f));
	//		v[vi * 4 + 3] = VertexData(Vector4(Vector3(planeLineHalfSize, 0.f, -10.f * vi)), LinearColor(0.7f, 0.7f, 0.7f));
	//		v[vi * 4 + 4] = VertexData(Vector4(Vector3(10.f * vi, 0.f, -planeLineHalfSize)), LinearColor(0.7f, 0.7f, 0.7f));
	//		v[vi * 4 + 5] = VertexData(Vector4(Vector3(10.f * vi, 0.f, planeLineHalfSize)), LinearColor(0.7f, 0.7f, 0.7f));
	//		v[vi * 4 + 6] = VertexData(Vector4(Vector3(-10.f * vi, 0.f, -planeLineHalfSize)), LinearColor(0.7f, 0.7f, 0.7f));
	//		v[vi * 4 + 7] = VertexData(Vector4(Vector3(-10.f * vi, 0.f, planeLineHalfSize)), LinearColor(0.7f, 0.7f, 0.7f));

	//		i[vi * 4] = vi * 4;
	//		i[vi * 4 + 1] = vi * 4 + 1;
	//		i[vi * 4 + 2] = vi * 4 + 2;
	//		i[vi * 4 + 3] = vi * 4 + 3;
	//	}

	//}

	//Matrix4x4 m[3] = { Matrix4x4(), InVMatrix, InPMatrix };
	//RSI->SetUniformMatrix(m);
	//RSI->SetVertexBuffer(v);
	//RSI->SetIndexBuffer(i);
	//RSI->DrawLinePrimitive(4, 4);
}

