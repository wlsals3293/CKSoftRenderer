#pragma once

enum class BlendingModes
{
	Opaque,
	AlphaBlending
};

struct VertexData
{
public:
	VertexData() { }
	explicit VertexData(Vector4 InPosition) : Position(InPosition) { }
	explicit VertexData(Vector4 InPosition, Color32 InColor) : Position(InPosition), Color(InColor) { }
	explicit VertexData(Vector4 InPosition, Color32 InColor, Vector2 InUV) : Position(InPosition), Color(InColor), UV(InUV) { }

	Vector4 Position;
	Color32 Color = Color32::Error;
	Vector3 Normal;
	Vector4 Tangent;
	Vector2 UV;
};
