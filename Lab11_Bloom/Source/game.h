#pragma once
#include "Graphics.h"
#include "VertexBuffer.h"
#include "RenderObj.h"
#include "Camera.h"
#include "texture.h"
#include "assetManager.h"
#include "JobManager.h"
#include "Physics.h"

class Shader;

#define MAX_POINT_LIGHTS 8
struct PointLightData
{
	Vector3 diffuseColor;
	float pad0;

	Vector3 specularColor;
	float pad1;

	Vector3 position;
	float specularPower;

	float innerRadius;
	float outerRadius;

	bool isEnabled;
	bool pad2, pad3, pad4;
	float pad5;

};

struct VertexPosNormSkinUV {

	Vector3 pos;
	Vector3 norm;
	float boneIndex;
	float boneWeight;
	Vector2 uv;

};

struct VertexPosUV {
	Vector3 pos;
	Vector2 uv;
};

struct VertexPosTangent {

	Vector3 pos;
	Vector3 norm;
	Vector3 tangent;
	Vector2 texturePos;

};

struct LightingConstants
{
	Vector3 c_ambient;
	float pad0;
	PointLightData c_pointLight[MAX_POINT_LIGHTS];
};

class Game
{
public:
    Game();
    ~Game();

    void Init(HWND hWnd, float width, float height);
    void Shutdown();
	void Update(float deltaTime);
    void RenderFrame();

	void OnKeyDown(uint32_t key);
	void OnKeyUp(uint32_t key);
	bool IsKeyHeld(uint32_t key) const;

	float mTriRotation = 0;

	PointLightData* AllocateLight();
	void FreeLight(PointLightData* pLight);
	void SetAmbientLight(const Vector3& color);
	const Vector3& GetAmbientLight() const;
	JobManager* GetJobManager() {
		return mJobManager;
	}

	Camera* GetCamera() {
		return mCamera;
	}

	Physics* GetPhysics() {
		return mPhysics;
	}

private:
	std::unordered_map<uint32_t, bool> m_keyIsHeld;
	
	Graphics m_Graphics;

	Camera* mCamera;

	bool LoadLevel(const WCHAR* fileName);

	LightingConstants mLights;
	ID3D11Buffer* mLightingBuffer;

	AssetManager mAssetManager;
	std::vector<RenderObj*> mObjects;

	class Character* mCharacter;

	JobManager* mJobManager;

	Physics* mPhysics;

	Texture* mTextureBuff;
	ID3D11RenderTargetView* mTextureRendTarget;

	VertexBuffer* mFullScreenQuad;
	int mWidth, mHeight;

	ID3D11RenderTargetView* mHalfScreenQuad;
	Texture* mHalfTextureBuff;

	ID3D11RenderTargetView* mQuarterScreenQuadOne;
	Texture* mQuarterTextureBuffOne;

	ID3D11RenderTargetView* mQuarterScreenQuadTwo;
	Texture* mQuarterTextureBuffTwo;

	ID3D11BlendState* mOpaqueState;
	ID3D11BlendState* mBlendState;

};