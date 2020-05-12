#include "stdafx.h"
#include "Graphics.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "RenderObj.h"
#include "texture.h"
#include "mesh.h"
#include "Component.h"
#include "../Source/game.h"

//RenderObj::RenderObj(Graphics* graphics, const VertexBuffer* vertexBuffer, const Shader* shader) {
RenderObj::RenderObj(Graphics* graphics,  Mesh* pMesh, Game* game) {

	// Set member variables
	mGraphics = graphics;
	mMesh = pMesh;
	mGame = game;

	// Create the object buffer from the matrix
	mMatrix.perObjConst = Matrix4::CreateRotationZ(Math::ToRadians(45.0f));
	mObjBuffer = mGraphics->CreateGraphicsBuffer(&mMatrix, sizeof(mMatrix), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);

}

RenderObj::~RenderObj() {

	mObjBuffer->Release();

}

// Helper function to update the rotation of the triangle
void RenderObj::UpdateRotation(float r) {
	mMatrix.perObjConst = Matrix4::CreateRotationZ(r);
}

void RenderObj::AddComponent(Component* pComp) {

	mComponents.push_back(pComp);

}

void RenderObj::Update(float deltaTime) {

	for (size_t i = 0; i < mComponents.size(); i++) {

		mComponents[i]->Update(deltaTime);

	}

}

// Set the shader to be active, upload the object buffer, and draw
void RenderObj::Draw() {

	mGraphics->UploadBuffer(mObjBuffer, &mMatrix, sizeof(mMatrix));
	mGraphics->GetDeviceContext()->VSSetConstantBuffers(1, 1, &mObjBuffer);
	mMesh->Draw();

}