#include "stdafx.h"
#include "Game.h"
#include "engineMath.h"
#include "Graphics.h"
#include "Shader.h"
#include "stringUtil.h"
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"
#include <fstream>
#include <sstream>
#include "RenderCube.h"
#include "RenderObj.h"
#include "mesh.h"
#include "assetManager.h"
#include "jsonUtil.h"
#include "Components/PointLight.h"
#include "Profiler.h"
#include "Skeleton.h"
#include "SkinnedObj.h"
#include "Components/Character.h"
#include "Components/player.h"
#include "Components/followCam.h"
#include "CollisionComponent.h"
#include "Components/SimpleRotate.h"

// TODO Lab 02e
struct VertexPosColor
{
	Vector3 pos;
	Graphics::Color4 color;
};

Game::Game() : mAssetManager(&m_Graphics, this)
{
	mPhysics = new Physics();
}

Game::~Game()
{
}

void Game::Init(HWND hWnd, float width, float height)
{
	// Initialize DSD window
	m_Graphics.InitD3D(hWnd, width, height);

    // Create vertices of triangle
	VertexPosColor vert[] =
	{
	{ Vector3(0.0f, 0.5f, 0.0f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ Vector3(0.45f, -0.5f, 0.0f), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ Vector3(-0.45f, -0.5f, 0.0f), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f) }
	};

	uint16_t vertices[] = { 0,1,2 };

	// Create the shaders
	mAssetManager.SetShader(L"Mesh", new Shader(&m_Graphics));
	mAssetManager.SetShader(L"BasicMesh", new Shader(&m_Graphics));
	mAssetManager.SetShader(L"Phong", new Shader(&m_Graphics));
	mAssetManager.SetShader(L"Unlit", new Shader(&m_Graphics));
	mAssetManager.SetShader(L"Skinned", new Shader(&m_Graphics));
	mAssetManager.SetShader(L"Normal", new Shader(&m_Graphics));
	mAssetManager.SetShader(L"Copy", new Shader(&m_Graphics));
	mAssetManager.SetShader(L"BloomMask", new Shader(&m_Graphics));
	mAssetManager.SetShader(L"VerticalBlur", new Shader(&m_Graphics));
	mAssetManager.SetShader(L"HorizontalBlur", new Shader(&m_Graphics));

	// Create the shader using below layout values
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			  D3D11_INPUT_PER_VERTEX_DATA,  offsetof(VertexPosColor, pos) },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,
				0, 12, D3D11_INPUT_PER_VERTEX_DATA, offsetof(VertexPosColor, pos) },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
			  D3D11_INPUT_PER_VERTEX_DATA,  offsetof(VertexPosColor, pos) },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20,
			  D3D11_INPUT_PER_VERTEX_DATA,  offsetof(VertexPosColor, pos) }

	};

	D3D11_INPUT_ELEMENT_DESC layout2[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			  D3D11_INPUT_PER_VERTEX_DATA,  offsetof(VertexPosColor, pos) },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
				0, 12, D3D11_INPUT_PER_VERTEX_DATA, offsetof(VertexPosColor, pos) },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
			  D3D11_INPUT_PER_VERTEX_DATA,  offsetof(VertexPosColor, pos) },
		{ "COLOR", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32,
			  D3D11_INPUT_PER_VERTEX_DATA,  offsetof(VertexPosColor, pos) }

	};

	D3D11_INPUT_ELEMENT_DESC inputElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosNormSkinUV, pos),         D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosNormSkinUV, norm),        D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES",     0, DXGI_FORMAT_R8G8B8A8_UINT,      0, offsetof(VertexPosNormSkinUV, boneIndex),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS",   0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, offsetof(VertexPosNormSkinUV, boneWeight),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, offsetof(VertexPosNormSkinUV, uv),          D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D11_INPUT_ELEMENT_DESC layout3[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			  D3D11_INPUT_PER_VERTEX_DATA,  offsetof(VertexPosColor, pos) + sizeof(int)*2},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
				0, 12, D3D11_INPUT_PER_VERTEX_DATA, offsetof(VertexPosColor, pos) + sizeof(int) * 2},
		{ "BONES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA
			, offsetof(VertexPosColor, pos) + sizeof(int) * 2}, // are these two right?
		{ "WEIGHTS", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 40, D3D11_INPUT_PER_VERTEX_DATA
			, offsetof(VertexPosColor, pos) + sizeof(int) * 2},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 56,
			  D3D11_INPUT_PER_VERTEX_DATA,  offsetof(VertexPosColor, pos) + sizeof(int) * 2 }
	};

	D3D11_INPUT_ELEMENT_DESC normalInput[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosTangent, pos),         D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosTangent, norm),        D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",     0, DXGI_FORMAT_R32G32_FLOAT,      0, offsetof(VertexPosTangent, tangent),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32B32_FLOAT,     0, offsetof(VertexPosTangent, texturePos),  D3D11_INPUT_PER_VERTEX_DATA, 0 }

	};

	D3D11_INPUT_ELEMENT_DESC copyShader[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosUV, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosUV, uv), D3D11_INPUT_PER_VERTEX_DATA, 0 }

	};

	bool test = mAssetManager.GetShader(L"Mesh")->Load(L"Shaders/Mesh.hlsl", layout, sizeof(layout)/sizeof(layout[0]));
	test = test && mAssetManager.GetShader(L"BasicMesh")->Load(L"Shaders/BasicMesh.hlsl", layout2, sizeof(layout2) / sizeof(layout2[0]));
	test = test && mAssetManager.GetShader(L"Normal")->Load(L"Shaders/Normal.hlsl", normalInput, sizeof(normalInput) / sizeof(normalInput[0]));
	test = test && mAssetManager.GetShader(L"Copy")->Load(L"Shaders/Copy.hlsl", copyShader, sizeof(copyShader) / sizeof(copyShader[0]));
	test = test && mAssetManager.GetShader(L"BloomMask")->Load(L"Shaders/BloomMask.hlsl", copyShader, sizeof(copyShader) / sizeof(copyShader[0]));
	test = test && mAssetManager.GetShader(L"VerticalBlur")->Load(L"Shaders/VerticalBlur.hlsl", copyShader, sizeof(copyShader) / sizeof(copyShader[0]));
	test = test && mAssetManager.GetShader(L"HorizontalBlur")->Load(L"Shaders/HorizontalBlur.hlsl", copyShader, sizeof(copyShader) / sizeof(copyShader[0]));
	assert(test);
	
	mAssetManager.GetShader(L"Phong")->Load(L"Shaders/Phong.hlsl", layout2, sizeof(layout2) / sizeof(layout2[0]));
	mAssetManager.GetShader(L"Unlit")->Load(L"Shaders/Unlit.hlsl", layout2, sizeof(layout2) / sizeof(layout2[0]));
	test = mAssetManager.GetShader(L"Skinned")->Load(L"Shaders/Skinned.hlsl", inputElem, sizeof(inputElem) / sizeof(inputElem[0]));
	assert(test);

	mCamera = new Camera(&m_Graphics);
	
	test = LoadLevel(L"Assets/Levels/Level10.itplevel");
	assert(test);

	// Creating the lighting buffer
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc = {
		sizeof(LightingConstants),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		0
	};

	m_Graphics.GetDevice()->CreateBuffer(&desc, NULL, &mLightingBuffer);

	mAssetManager.LoadSkeleton(L"Assets/Anims/SK_Mannequin.itpskel");
	mAssetManager.LoadAnimation(L"Assets/Anims/ThirdPersonIdle.itpanim2");

	mJobManager = new JobManager();
	mJobManager->Begin();

	// Create new textures and render targets
	mTextureBuff = new Texture(&m_Graphics);
	mTextureRendTarget = mTextureBuff->CreateRenderTarget(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	mHalfTextureBuff = new Texture(&m_Graphics);
	mHalfScreenQuad = mHalfTextureBuff->CreateRenderTarget(width / 2, height / 2, DXGI_FORMAT_R32G32B32A32_FLOAT);
	mQuarterTextureBuffOne = new Texture(&m_Graphics);
	mQuarterScreenQuadOne = mQuarterTextureBuffOne->CreateRenderTarget(width / 4, height / 4, DXGI_FORMAT_R32G32B32A32_FLOAT);
	mQuarterTextureBuffTwo = new Texture(&m_Graphics);
	mQuarterScreenQuadTwo = mQuarterTextureBuffTwo->CreateRenderTarget(width / 4, height / 4, DXGI_FORMAT_R32G32B32A32_FLOAT);

	VertexPosUV copyShaderVals[] =
	{
		{ Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(1.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f) },
		{ Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f) },
		{ Vector3(-1.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f) }
	};

	uint16_t copyShaderIndices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	mFullScreenQuad = new VertexBuffer(&m_Graphics, copyShaderVals, sizeof(copyShaderVals) / sizeof(copyShaderVals[0]),
		sizeof(copyShaderVals[0]), copyShaderIndices, sizeof(copyShaderIndices) / sizeof(copyShaderIndices[0]),
		sizeof(copyShaderIndices[0]));

	mWidth = width;
	mHeight = height;

	mBlendState = m_Graphics.CreateBlendState(true, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE);

	mOpaqueState = m_Graphics.CreateBlendState(false, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ZERO);

}

PointLightData* Game::AllocateLight() {

	// Search for next available light and enable it
	for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
		if (!mLights.c_pointLight[i].isEnabled) {
			mLights.c_pointLight[i].isEnabled = true;
			return &mLights.c_pointLight[i];
		}
	}

	return nullptr;

}

void Game::FreeLight(PointLightData* pLight) {

	pLight->isEnabled = false;

}

void Game::SetAmbientLight(const Vector3& color) {

	mLights.c_ambient = color;

}
const Vector3& Game::GetAmbientLight() const {

	return mLights.c_ambient;

}

void Game::Shutdown()
{
	// Release / Delete remaining pointers
	delete mCamera;

	for (int i = 0; i < mObjects.size(); i++) {
		delete mObjects[i];
	}

	mJobManager->End();
	
	mTextureRendTarget->Release();
	mHalfScreenQuad->Release();
	mQuarterScreenQuadOne->Release();
	mQuarterScreenQuadTwo->Release();
	
	delete mTextureBuff;
	delete mHalfTextureBuff;
	delete mQuarterTextureBuffOne;
	delete mQuarterTextureBuffTwo;

	delete mFullScreenQuad; 

	mOpaqueState->Release();
	mBlendState->Release();

	mLightingBuffer->Release();
	mAssetManager.Clear();
	m_Graphics.CleanD3D();

}

void Game::Update(float deltaTime)
{

	for (int i = 0; i < mObjects.size(); i++) {
		mObjects[i]->Update(deltaTime);
	}

	mJobManager->WaitForJobs();

}

void Game::RenderFrame()
{
	m_Graphics.SetRenderTarget(mTextureRendTarget, m_Graphics.GetDepthView());
	m_Graphics.SetViewport(0.0f, 0.0f, mWidth, mHeight);
	m_Graphics.SetBlendState(mOpaqueState);

	{
		PROFILE_SCOPE(RenderFrame_Begin);

		// Start of frame
		m_Graphics.BeginFrame(Graphics::Color4(0.0f, 0.2f, 0.4f, 1.0f));

	}

	{
		PROFILE_SCOPE(RenderFrame_SetActive);

		// Set the camera to be active
		mCamera->SetActive();

	}

	{
		PROFILE_SCOPE(RenderFrame_Draw);

		for (int i = 0; i < mObjects.size(); i++) {
			mObjects[i]->Draw();
		}

	}

	// Full to half w/ bloom
	m_Graphics.SetActiveTexture(0, nullptr);
	m_Graphics.SetRenderTarget(mHalfScreenQuad, nullptr);
	m_Graphics.SetViewport(0.0f, 0.0f, mWidth / 2, mHeight / 2);
	mAssetManager.GetShader(L"BloomMask")->SetActive();
	mTextureBuff->SetActive(0);
	mFullScreenQuad->Draw();

	// Half to first quarter
	m_Graphics.SetActiveTexture(0, nullptr);
	m_Graphics.SetRenderTarget(mQuarterScreenQuadOne, nullptr);
	m_Graphics.SetViewport(0.0f, 0.0f, mWidth / 4, mHeight / 4);
	mAssetManager.GetShader(L"Copy")->SetActive();
	mHalfTextureBuff->SetActive(0);
	mFullScreenQuad->Draw();

	// First quarter to second quarter w/ first blur
	m_Graphics.SetActiveTexture(0, nullptr);
	m_Graphics.SetRenderTarget(mQuarterScreenQuadTwo, nullptr);
	m_Graphics.SetViewport(0.0f, 0.0f, mWidth / 4, mHeight / 4);
	mAssetManager.GetShader(L"HorizontalBlur")->SetActive();
	mQuarterTextureBuffOne->SetActive(0);
	mFullScreenQuad->Draw();

	// Second quarter back to first quarter w/ second blur pass
	m_Graphics.SetActiveTexture(0, nullptr);
	m_Graphics.SetRenderTarget(mQuarterScreenQuadOne, nullptr);
	m_Graphics.SetViewport(0.0f, 0.0f, mWidth / 4, mHeight / 4);
	mAssetManager.GetShader(L"VerticalBlur")->SetActive();
	mQuarterTextureBuffTwo->SetActive(0);
	mFullScreenQuad->Draw();

	// First quarter to back buffer
	m_Graphics.SetActiveTexture(0, nullptr);
	m_Graphics.SetRenderTarget(m_Graphics.GetBackBuffer(), nullptr);
	m_Graphics.SetViewport(0.0f, 0.0f, mWidth, mHeight);
	mAssetManager.GetShader(L"Copy")->SetActive();
	mQuarterTextureBuffOne->SetActive(0);
	mFullScreenQuad->Draw();

	// Switch to blend state
	m_Graphics.SetBlendState(mBlendState);

	// Second (full) screen image
	m_Graphics.SetActiveTexture(0, nullptr);
	m_Graphics.SetRenderTarget(m_Graphics.GetBackBuffer(), nullptr);
	m_Graphics.SetViewport(0.0f, 0.0f, mWidth, mHeight);
	mAssetManager.GetShader(L"Copy")->SetActive();
	mTextureBuff->SetActive(0);
	mFullScreenQuad->Draw();

	m_Graphics.SetActiveTexture(0, nullptr);

	{
		PROFILE_SCOPE(RenderFrame_UploadEnd);

		// Set the lighting buffer
		m_Graphics.UploadBuffer(mLightingBuffer, &mLights, sizeof(mLights));
		m_Graphics.GetDeviceContext()->PSSetConstantBuffers(Graphics::CONSTANT_BUFFER_LIGHTING, 1, &mLightingBuffer);

	}

	// End of frame
	m_Graphics.EndFrame();

}

void Game::OnKeyDown(uint32_t key)
{
	m_keyIsHeld[key] = true;
}

void Game::OnKeyUp(uint32_t key)
{
	m_keyIsHeld[key] = false;
}

bool Game::IsKeyHeld(uint32_t key) const
{
	const auto find = m_keyIsHeld.find(key);
	if (find != m_keyIsHeld.end())
		return find->second;
	return false;
}

bool Game::LoadLevel(const WCHAR* fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);
	
	if (!doc.IsObject())
	{
		return false;
	}

	std::string str = doc["metadata"]["type"].GetString();
	int ver = doc["metadata"]["version"].GetInt();

	// Check the metadata
	if (!doc["metadata"].IsObject() ||
		str != "itplevel" ||
		ver != 2)
	{
		return false;
	}

	// Read the camera information
	const rapidjson::Value& cameraInfo = doc["camera"];
	Vector3 cameraPos;
	Quaternion cameraRot;

	GetVectorFromJSON(cameraInfo, "position", cameraPos);
	GetQuaternionFromJSON(cameraInfo, "rotation", cameraRot);

	// Set the camera's WorldToCamera
	Matrix4 mult = Matrix4::CreateFromQuaternion(cameraRot) * Matrix4::CreateTranslation(cameraPos);
	mult.Invert();
	mCamera->SetWTCMatrix(mult);

	// Load ambient light
	const rapidjson::Value& camLight = doc["lightingData"];
	Vector3 cameraAmbient;
	GetVectorFromJSON(camLight, "ambient", cameraAmbient);
	mLights.c_ambient = cameraAmbient;

	// Load render objects
	const rapidjson::Value& renderObjs = doc["renderObjects"];
	if (!renderObjs.IsArray() || renderObjs.Size() < 1)
	{
		return false;
	}

	for (rapidjson::SizeType i = 0; i < renderObjs.Size(); i++)
	{
		if (!renderObjs[i].IsObject())
		{
			return false;
		}

		Vector3 pos; // "position"
		Quaternion rot; // "rotation"
		float scale; // "scale"
		std::string fileName; // "mesh"
		
		GetVectorFromJSON(renderObjs[i], "position", pos);
		GetQuaternionFromJSON(renderObjs[i], "rotation", rot);
		scale = (float)(renderObjs[i]["scale"].GetDouble());
		fileName = renderObjs[i]["mesh"].GetString();

		const std::wstring file(fileName.begin(), fileName.end());

		if (i == 0) {
			assert(pos.z == 50.0f);
			assert(rot.x == 0);
			assert(scale == 0.5f);
			assert(fileName == "Assets/Meshes/LightSphere.itpmesh2");
		}

		Matrix4 mat = Matrix4::CreateScale(scale) * Matrix4::CreateFromQuaternion(rot) *
			Matrix4::CreateTranslation(pos);

		Mesh* m = mAssetManager.LoadMesh(file);
		RenderObj* obj;

		if (!m->IsSkinned()) {
			obj = new RenderObj(&m_Graphics, m, this);
			obj->mMatrix.perObjConst = mat;
			mObjects.push_back(obj);
		}
		else {
			obj = new SkinnedObj(&m_Graphics, m, &mAssetManager, this);
			obj->mMatrix.perObjConst = mat;
			mObjects.push_back(obj);
		}

		const rapidjson::Value& comps = renderObjs[i]["components"];
		if (!comps.IsArray())
		{
			return false;
		}

		for (rapidjson::SizeType j = 0; j < comps.Size(); j++) {

			if (comps[j]["type"] == "PointLight") {

				PointLight* light = new PointLight(obj, this);
				light->LoadProperties(comps[j]);
				obj->AddComponent(light);

			}

			else if (comps[j]["type"] == "Character") {

				Character* character = new Character(&mAssetManager, (SkinnedObj*)obj);
				character->LoadProperties(comps[j]);
				obj->AddComponent(character);

			}

			else if (comps[j]["type"] == "Player") {

				Player* player = new Player((SkinnedObj*)obj, &mAssetManager);
				player->LoadProperties(comps[j]);
				obj->AddComponent(player);

			}

			else if (comps[j]["type"] == "FollowCam") {

				FollowCam* camera = new FollowCam((RenderObj*)obj);
				camera->LoadProperties(comps[j]);
				obj->AddComponent(camera);

			}

			else if (comps[j]["type"] == "Collision")
			{

				CollisionComponent* col = new CollisionComponent(obj);
				col->LoadProperties(comps[j]);
				obj->AddComponent(col);

			}

			else if (comps[j]["type"] == "SimpleRotate") {
				SimpleRotate* rot = new SimpleRotate(obj);
				rot->LoadProperties(comps[j]);
				obj->AddComponent(rot);
			}

		}

	}

	return true;
}