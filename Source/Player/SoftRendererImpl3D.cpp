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
	// 朝五虞 持失
	float repA = (float)ScreenSize.Y / (float)ScreenSize.X;
	MainCamera = new Camera(60.f, 5.5f, 100.f, repA);

	// 五習 持失
	const int vertexCount = 24;
	const int triangleCount = 12;
	const int indexCount = triangleCount * 3;

	VertexData* v = new VertexData[vertexCount]{
		// Front 
		VertexData(Vector4(0.5f, -0.5f, 0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(0.5f, 0.5f, 0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(0.5f, 0.5f, -0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(0.5f, -0.5f, -0.5f), Color32(0, 0, 0)),
		// Left
		VertexData(Vector4(-0.5f, -0.5f, 0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(-0.5f, 0.5f, 0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(0.5f, 0.5f, 0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(0.5f, -0.5f, 0.5f), Color32(0, 0, 0)),
		// Right
		VertexData(Vector4(0.5f, -0.5f, -0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(0.5f, 0.5f, -0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(-0.5f, 0.5f, -0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(-0.5f, -0.5f, -0.5f), Color32(0, 0, 0)),
		// Back
		VertexData(Vector4(-0.5f, -0.5f, -0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(-0.5f, 0.5f, -0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(-0.5f, 0.5f, 0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(-0.5f, -0.5f, 0.5f), Color32(0, 0, 0)),
		// Top
		VertexData(Vector4(0.5f, 0.5f, 0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(-0.5f, 0.5f, 0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(-0.5f, 0.5f, -0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(0.5f, 0.5f, -0.5f), Color32(0, 0, 0)),
		// Bottom
		VertexData(Vector4(-0.5f, -0.5f, 0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(0.5f, -0.5f, 0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(0.5f, -0.5f, -0.5f), Color32(0, 0, 0)),
		VertexData(Vector4(-0.5f, -0.5f, -0.5f), Color32(0, 0, 0))
	};
	
	int* i = new int[indexCount] {
		0, 2, 1, 0, 3, 2,
		4, 6, 5, 4, 7, 6,
		8, 10, 9, 8, 11, 10,
		12, 14, 13, 12, 15, 14,
		16, 18, 17, 16, 19, 18,
		20, 22, 21, 20, 23, 22
	};

	SharedCubeMesh = std::make_unique<Mesh>();
	SharedCubeMesh.get()->SetMesh(v, i, vertexCount, indexCount);
	//SharedCubeMesh.get()->SetColor(c);

	//delete[] v;
	//delete[] i;
}

void SoftRendererImpl3D::LoadScene()
{
	// Setup Game Scene 
	GameObject* player = new GameObject();
	player->SetMesh(SharedCubeMesh.get());
	player->GetTransform().Position = Vector3::Zero;
	player->GetTransform().Rotaiton = Vector3::Zero;
	player->GetTransform().Scale = Vector3::One * 100.f;
	Scene.emplace_back(player);

	for (int i = 0; i < 5; ++i)
	{
		GameObject* worldStatic = new GameObject();
		worldStatic->SetMesh(SharedCubeMesh.get());
		worldStatic->GetTransform().Position = Vector3(0.f, 0.f, i * 100.f);
		worldStatic->GetTransform().Scale = Vector3::One * 100.f;

		Scene.emplace_back(worldStatic);
	}

	MainCamera->GetTransform().Position = Vector3(500.f, 500.f, -500.f);
	/*
	MainCamera->TransformData.Position = Vector3(0.f, 500.f, -500.f);
	Cube[1].TransformData.Position = Vector3(200.f, 0.f, 0.f);*/
}

void SoftRendererImpl3D::RenderFrameImpl()
{
	assert(RSI != nullptr && RSI->IsInitialized() && !ScreenSize.HasZero());

	auto player = Scene[0].get();
	Matrix4x4 vMat = MainCamera->GetLookAtMatrix(player->GetTransform().Position);
	Matrix4x4 pMat = MainCamera->GetProjectionMatrix();

	for (auto& g : Scene)
	{
		Matrix4x4 mMat = g->GetTransform().GetTRS();

		Matrix4x4 m[3] = { mMat, vMat, pMat };

		Mesh* meshToRender = player->GetMesh();
		int vertexCount = meshToRender->GetVertexCount();
		int indexCount = meshToRender->GetIndexCount();
		VertexData* meshVertexBuffer = meshToRender->GetVertexBuffer();
		int* i = meshToRender->GetIndexBuffer();

		RSI->SetUniformMatrix(m);
		RSI->SetVertexBuffer(meshVertexBuffer);
		RSI->SetIndexBuffer(i);
		RSI->DrawPrimitive(vertexCount, indexCount);
	}

	/*for (int i = 0; i < 2; i++)
	{
		m[0] = Cube[i].TransformData.GetTRS();
		RSI->SetUniformMatrix(m);
		Mesh* meshToRender = Cube[i]>MeshData;
		int vertexCount = meshToRender->GetVertexCount();
		int indexCount = meshToRender->GetIndexCount();

		RSI->SetVertexBuffer(meshToRender->GetVertices());
		RSI->SetIndexBuffer(meshToRender->GetIndices());
		RSI->DrawPrimitive(vertexCount, indexCount);
	}*/

	DrawGizmo3D(vMat, pMat);
	DrawXYPlane(vMat, pMat);
}

void SoftRendererImpl3D::UpdateImpl(float DeltaSeconds)
{
	static float moveSpeed = 1000.f;
	static float rotateSpeed = 180.f;

	auto player = Scene[0].get();
	Vector3 deltaPos = Vector3::UnitZ * InputManager.GetYAxis() * moveSpeed * DeltaSeconds;
	float deltaDegree = InputManager.GetXAxis() * rotateSpeed * DeltaSeconds;
	float deltaDegree2 = InputManager.GetYAxis() * rotateSpeed * DeltaSeconds;
	player->GetTransform().Position += deltaPos;
	player->GetTransform().Rotaiton.Y += deltaDegree;
}

void SoftRendererImpl3D::DrawGizmo3D(Matrix4x4 InVMatrix, Matrix4x4 InPMatrix)
{
	const int gizmosVertexCount = 6;
	VertexData v[gizmosVertexCount] = {
		VertexData(Vector3::Zero, LinearColor::Red.ToColor32()),
		VertexData(Vector3::UnitX * 500.f, LinearColor::Red.ToColor32()),
		VertexData(Vector3::Zero, LinearColor::Green.ToColor32()),
		VertexData(Vector3::UnitY * 500.f, LinearColor::Green.ToColor32()),
		VertexData(Vector3::Zero, LinearColor::Blue.ToColor32()),
		VertexData(Vector3::UnitZ * 500.f, LinearColor::Blue.ToColor32()),
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

