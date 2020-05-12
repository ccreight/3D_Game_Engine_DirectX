#include "stdafx.h"
#include "SkinnedObj.h"
#include "mesh.h"
#include "Graphics.h"
#include "assetManager.h"
#include "../Lab07_Animation/Source/Components/Character.h"
#include "../Source/game.h"

SkinnedObj::SkinnedObj(Graphics* g, Mesh* m, AssetManager* a, Game* game) : RenderObj(g, m, game) {

	for (int i = 0; i < MAX_SKELETON_BONES; i++) {
		mSkinConstants.c_skinMatrix[i] = Matrix4::Identity;
	}

	mConstantBuffer = g->CreateGraphicsBuffer(&mSkinConstants, 
		sizeof(mSkinConstants), 
		D3D11_BIND_CONSTANT_BUFFER, 
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);

}

void SkinnedObj::Draw() {

	mGraphics->UploadBuffer(mConstantBuffer, &mSkinConstants, sizeof(mSkinConstants));
	mGraphics->GetDeviceContext()->VSSetConstantBuffers(Graphics::CONSTANT_BUFFER_SKINNING, 1, &mConstantBuffer);
	RenderObj::Draw();

}