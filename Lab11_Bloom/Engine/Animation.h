#pragma once
#include "stdafx.h"
#include "BoneTransform.h"
#include "assetManager.h"

class Animation {

	public:
		Animation(AssetManager* a);

		uint32_t GetNumBones() const;
		uint32_t GetNumFrames() const;
		float GetLength() const;

		static Animation* StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager);
		bool Load(const WCHAR* fileName);

		void GetGlobalPoseAtTime(
			std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime
		) const;

	private:
		uint32_t mNumBones;
		uint32_t mNumFrames;
		float mAnimLength;
		AssetManager* mAssetManager;
		std::vector<std::vector<BoneTransform>> mData;

};