#include "stdafx.h"
#include "simdMath.h"
#include "engineMath.h"
#include "RenderCube.h"
#include "Graphics.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "assetManager.h"
#include "mesh.h"

struct VertexPosColor
{
	Vector3 pos;
	Graphics::Color4 color;
	Vector2 texturePos;
	Vector3 normal;

};

static VertexPosColor cubeVertex[] =
{
	// Far parallel face
	{ Vector3(-0.5f, 0.5f, -0.5f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0,0), Vector3(0,0,-1)},
	{ Vector3(0.5f, 0.5f, -0.5f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0,1), Vector3(0,0,-1) },
	{ Vector3(0.5f, -0.5f, -0.5f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1,1), Vector3(0,0,-1) },
	{ Vector3(-0.5f, -0.5f, -0.5f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1,0), Vector3(0,0,-1) },

	// Right face, perpendicular to us
	{ Vector3(0.5f, 0.5f, -0.5f), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0,1), Vector3(1,0,0)},
	{ Vector3(0.5f, 0.5f, 0.5f), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0,0), Vector3(1,0,0) },
	{ Vector3(0.5f, -0.5f, 0.5f), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(1,0), Vector3(1,0,0) },
	{ Vector3(0.5f, -0.5f, -0.5f), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(1,1), Vector3(1,0,0) },

	// Parallel face facing us
	{ Vector3(-0.5f, -0.5f, 0.5f), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(1,0), Vector3(0,0,1)},
	{ Vector3(0.5f, -0.5f, 0.5f), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(1,1), Vector3(0,0,1) },
	{ Vector3(0.5f, 0.5f, 0.5f), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0,1), Vector3(0,0,1) },
	{ Vector3(-0.5f, 0.5f, 0.5f), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0,0), Vector3(0,0,1) },

	// Left face, perpendicular to us
	{ Vector3(-0.5f, -0.5f, -0.5f), Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(1,0), Vector3(-1,0,0)},
	{ Vector3(-0.5f, -0.5f, 0.5f), Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(1,1), Vector3(-1,0,0) },
	{ Vector3(-0.5f, 0.5f, 0.5f), Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(0,1), Vector3(-1,0,0) },
	{ Vector3(-0.5f, 0.5f, -0.5f), Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(0,0), Vector3(-1,0,0) },

	// Top face
	{ Vector3(-0.5f, 0.5f, -0.5f), Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(0,0), Vector3(0,1,0)},
	{ Vector3(-0.5f, 0.5f, 0.5f), Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(1,0), Vector3(0,1,0)},
	{ Vector3(0.5f, 0.5f, 0.5f), Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(1,1), Vector3(0,1,0) },
	{ Vector3(0.5f, 0.5f, -0.5f), Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(0,1), Vector3(0,1,0) },

	// Bottom face
	{ Vector3(-0.5f, -0.5f, -0.5f), Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f), Vector2(1,0), Vector3(0,-1,0)},
	{ Vector3(-0.5f, -0.5f, 0.5f), Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f), Vector2(0,0), Vector3(0,-1,0) },
	{ Vector3(0.5f, -0.5f, 0.5f), Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f), Vector2(0,1), Vector3(0,-1,0) },
	{ Vector3(0.5f, -0.5f, -0.5f), Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f), Vector2(1,1), Vector3(0,-1,0) },

};

// The array storing the ordering of cube indices
static uint16_t cubeIndex[] =
{
	2, 1, 0,
	3, 2, 0,
	6, 5, 4,
	7, 6, 4,
	10, 9, 8,
	11, 10, 8,
	14, 13, 12,
	15, 14, 12,
	17, 16, 18,
	16, 19, 18,
	20, 22, 23,
	20, 21, 22,
};

// Create the cube's vertex buffer
//RenderCube::RenderCube(Graphics* graphics, const Shader* shader) : RenderObj(graphics, nullptr, shader) {
RenderCube::RenderCube(AssetManager* pAssetManager, const Shader* pShader, const Texture* pTex) :
	RenderObj(pAssetManager->GetGraphics(), nullptr, nullptr){

	/*mBuffer = new VertexBuffer(mGraphics, cubeVertex, 24, sizeof(VertexPosColor), 
		cubeIndex, 36, sizeof(uint16_t));*/

	mMesh = new Mesh(pAssetManager, new VertexBuffer(pAssetManager->GetGraphics(), cubeVertex, 24, sizeof(VertexPosColor),
		cubeIndex, 36, sizeof(uint16_t)), pShader);

	mMesh->SetTexture(Graphics::TEXTURE_SLOT_DIFFUSE, pAssetManager->LoadTexture(L"Assets/Textures/Cube.png"));


}

RenderCube::~RenderCube() {
	delete mMesh;
}