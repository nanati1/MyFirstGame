#include "Dice.h"
#include <string>


Dice::Dice()
{
}

Dice::~Dice()
{
}

HRESULT Dice::Initialize()
{
	return Quad::Initialize();


}

void Dice::Draw(XMMATRIX& worldMatrix, XMMATRIX& uvMatrix)
{
	QuadData faces[] = {
		{ { 0.0f, 0.0f, 1.0f }, {   0.0f,   0.0f,   0.0f }, { 0.25f,0.5f } },  // Front
		{ { 0.0f, 0.0f,-1.0f }, {   0.0f, -180.0f,  0.0f }, { 0.0f, 0.0f } },  // Back
		{ {-1.0f, 0.0f, 0.0f }, {   0.0f, -90.0f,   0.0f }, { 0.25f,0.0f } },  // Left
		{ { 1.0f, 0.0f, 0.0f }, {   0.0f,  90.0f,   0.0f }, { 0.0f, 0.5f } },  // Right
		{ { 0.0f, 1.0f, 0.0f }, { -90.0f,   0.0f,   0.0f }, { 0.5f, 0.0f } },  // Top
		{ { 0.0f,-1.0f, 0.0f }, {  90.0f,   0.0f,   0.0f }, { 0.75f,0.0f } },  // Bottom
	};

	for (const auto& face : faces)
	{
		XMMATRIX mat =

			XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(face.rotation.x),
				XMConvertToRadians(face.rotation.y),
				XMConvertToRadians(face.rotation.z)) *
			XMMatrixTranslation(face.position.x, face.position.y, face.position.z) *
			worldMatrix;


		/* XMMATRIX uvMatrix = XMMatrixTransformation2D({ face.scale.x,face.scale.y,0,0 }, 1,
			 { 0.25f,0.5f,1,1 }, { 0,0,0,0 }, 0, { face.scale.x,face.scale.y,0,0 });*//* = XMMatrixScaling(0.25f, 0.5f, 1.0f)* XMMatrixTranslation(face.scale.x,face.scale.y,0);*/ // UVマトリックスは単位行列

		XMMATRIX uvMatrix = XMMatrixScaling(0.25f, 0.5f, 1.0f) * XMMatrixTranslation(face.scale.x, face.scale.y, 0);
		Quad::Draw(mat, uvMatrix);
	}
}
