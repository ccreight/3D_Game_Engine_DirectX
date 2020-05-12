#include "stdafx.h"
#include "assetManager.h"
#include "Graphics.h"
#include "assetCache.h"
#include "mesh.h"
#include "Animation.h"
#include "../Lab07_Animation/Source/game.h"

AssetManager::AssetManager(Graphics* pGraphics, Game* game)
	: mGraphics(pGraphics), mAssetCacheShaders(this), mAssetCacheTextures(this), mAssetCacheMeshes(this), mAssetCacheSkeletons(this),
	mAssetCacheAnimations(this), mGame(game)
{
	
}

AssetManager::~AssetManager()
{
}

void AssetManager::Clear()
{
	mAssetCacheTextures.Clear();
	mAssetCacheShaders.Clear();
	mAssetCacheMeshes.Clear();
	mAssetCacheSkeletons.Clear();
	mAssetCacheAnimations.Clear();

}

Shader* AssetManager::GetShader(const std::wstring& shaderName) {

	return mAssetCacheShaders.Get(shaderName);

}

Texture* AssetManager::LoadTexture(const std::wstring& fileName) {

	return mAssetCacheTextures.Load(fileName);

}

void AssetManager::SetShader(const std::wstring& shaderName, Shader* pShader) {

	mAssetCacheShaders.Cache(shaderName, pShader);

}

void AssetManager::SetMesh(const std::wstring& name, Mesh* mesh) {

	mAssetCacheMeshes.Cache(name, mesh);

}

Mesh* AssetManager::GetMesh(const std::wstring& name) {

	return mAssetCacheMeshes.Load(name);

}

Mesh* AssetManager::LoadMesh(const std::wstring& fileName) {

	return mAssetCacheMeshes.Load(fileName);

}

Skeleton* AssetManager::LoadSkeleton(const std::wstring& fileName) {

	return mAssetCacheSkeletons.Load(fileName);

}

Animation* AssetManager::LoadAnimation(const std::wstring& fileName) {
	return mAssetCacheAnimations.Load(fileName);
}