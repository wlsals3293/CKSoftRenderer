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


	// 카메라 생성
	float repA = (float)ScreenSize.Y / (float)ScreenSize.X;
	MainCamera = new Camera(60.f, 5.5f, 100.f, repA);
	MainCamera->TransformData.Position = Vector3(0.f, 500.f, -500.f);

	// 오브젝트 생성
	Cube = new GameObject();
	Cube->TransformData.Scale = Vector3(100.f, 100.f, 100.f);

	// 메쉬 생성
	int vertexCount = 24;
	VertexData *v = new VertexData[vertexCount] {
		// Front 
		VertexData(Vector3(0.5f, -0.5f, 0.5f)),
		VertexData(Vector3(0.5f, 0.5f, 0.5f)),
		VertexData(Vector3(0.5f, 0.5f, -0.5f)),
		VertexData(Vector3(0.5f, -0.5f, -0.5f)),
		// Left
		VertexData(Vector3(-0.5f, -0.5f, 0.5f)),
		VertexData(Vector3(-0.5f, 0.5f, 0.5f)),
		VertexData(Vector3(0.5f, 0.5f, 0.5f)),
		VertexData(Vector3(0.5f, -0.5f, 0.5f)),
		// Right
		VertexData(Vector3(0.5f, -0.5f, -0.5f)),
		VertexData(Vector3(0.5f, 0.5f, -0.5f)),
		VertexData(Vector3(-0.5f, 0.5f, -0.5f)),
		VertexData(Vector3(-0.5f, -0.5f, -0.5f)),
		// Back
		VertexData(Vector3(-0.5f, -0.5f, -0.5f)),
		VertexData(Vector3(-0.5f, 0.5f, -0.5f)),
		VertexData(Vector3(-0.5f, 0.5f, 0.5f)),
		VertexData(Vector3(-0.5f, -0.5f, 0.5f)),
		// Top
		VertexData(Vector3(0.5f, 0.5f, 0.5f)),
		VertexData(Vector3(-0.5f, 0.5f, 0.5f)),
		VertexData(Vector3(-0.5f, 0.5f, -0.5f)),
		VertexData(Vector3(0.5f, 0.5f, -0.5f)),
		// Bottom
		VertexData(Vector3(-0.5f, -0.5f, 0.5f)),
		VertexData(Vector3(0.5f, -0.5f, 0.5f)),
		VertexData(Vector3(0.5f, -0.5f, -0.5f)),
		VertexData(Vector3(-0.5f, -0.5f, -0.5f))
	};

	int triangleCount = 12;
	int indexCount = triangleCount * 3;
	int *i = new int[indexCount] {
	 0, 2, 1, 0, 3, 2,
	 4, 6, 5, 4, 7, 6,
	 8, 10, 9, 8, 11, 10,
	 12, 14, 13, 12, 15, 14,
	 16, 18, 17, 16, 19, 18,
	 20, 22, 21, 20, 23, 22
	};

	Cube->SetMesh(new Mesh(v, i, vertexCount, triangleCount));

}

SoftRendererImpl3D::~SoftRendererImpl3D()
{
}


void SoftRendererImpl3D::RenderFrameImpl()
{
	assert(RSI != nullptr && RSI->IsInitialized() && !ScreenSize.HasZero());

	// 카메라 메트릭스 (투영행렬 * 뷰행렬)
	Matrix4x4 camMat = MainCamera->GetLookAtMatrix(Cube->TransformData.Position);
	//Matrix4x4 camMat = MainCamera->GetViewMatrix();

	int triangleCount = Cube->MeshData->GetTriangleCount();
	VertexData* vertexDatas = Cube->MeshData->GetVertices();
	int* indexDatas = Cube->MeshData->GetIndices();

	// 최종 행렬
	Matrix4x4 finalMat = camMat * Cube->TransformData.GetTRS();

	for (int t = 0; t < triangleCount; t++)
	{
		Vector4 tp[3];
		tp[0] = vertexDatas[indexDatas[t * 3]].Position;
		tp[1] = vertexDatas[indexDatas[t * 3 + 1]].Position;
		tp[2] = vertexDatas[indexDatas[t * 3 + 2]].Position;

		for (int ti = 0; ti < 3; ti++)
		{
			tp[ti] = finalMat * tp[ti];
			float repW = 1.f / tp[ti].W;
			tp[ti].X *= repW;
			tp[ti].Y *= repW;
			tp[ti].Z *= repW;
		}

		// Backface Culling
		Vector3 edge1 = (tp[1] - tp[0]).ToVector3();
		Vector3 edge2 = (tp[2] - tp[0]).ToVector3();
		Vector3 faceNormal = -edge1.Cross(edge2).Normalize();
		static Vector3 cameraDir = -Vector3::UnitZ;
		if (cameraDir.Dot(faceNormal) > 0.f)
		{
			continue;
		}

		for (int ti = 0; ti < 3; ti++)
		{
			tp[ti].X *= (ScreenSize.X * 0.5f);
			tp[ti].Y *= (ScreenSize.Y * 0.5f);
		}
		
		RSI->DrawLine(tp[0].ToVector2(), tp[1].ToVector2(), LinearColor::Red);
		RSI->DrawLine(tp[0].ToVector2(), tp[2].ToVector2(), LinearColor::Red);
		RSI->DrawLine(tp[1].ToVector2(), tp[2].ToVector2(), LinearColor::Red);
	}
}

void SoftRendererImpl3D::UpdateImpl(float DeltaSeconds)
{
	static float moveSpeed = 100.f;
	static float rotateSpeed = 180.f;

	Cube->TransformData.Position += Vector3::UnitZ * InputManager.GetYAxis() * moveSpeed * DeltaSeconds;
	Cube->TransformData.Rotaiton.Y += InputManager.GetXAxis() * rotateSpeed * DeltaSeconds;

}
