#pragma once
#include "Shader.h"
#include "texture.h"
#include "assetCache.h"

class Graphics;

class AssetManager
{
public:
    AssetManager(Graphics* pGraphics, class Game* game);
    ~AssetManager();

    void Clear();

    Graphics* GetGraphics() { return mGraphics; }

	Shader* GetShader(const std::wstring& shaderName);
	void SetShader(const std::wstring& shaderName, Shader* pShader);
	Texture* LoadTexture(const std::wstring& fileName);
	void SetMesh(const std::wstring& name, class Mesh* mesh);
	Mesh* GetMesh(const std::wstring& name);
	Mesh* LoadMesh(const std::wstring& fileName);

	class Skeleton* LoadSkeleton(const std::wstring& fileName);
	class Animation* LoadAnimation(const std::wstring& fileName);

	class Game* GetGame() {
		return mGame;
	}

private:
    Graphics* mGraphics;
	AssetCache<Shader> mAssetCacheShaders;
	AssetCache<Texture> mAssetCacheTextures;
	AssetCache<Mesh> mAssetCacheMeshes;
	AssetCache<class Skeleton> mAssetCacheSkeletons;
	AssetCache<class Animation> mAssetCacheAnimations;
	class Game* mGame;

};
