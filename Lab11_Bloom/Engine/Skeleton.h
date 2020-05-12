#pragma once
#include "BoneTransform.h"
#include <string>
#include "assetManager.h"

class Skeleton {

	public:

		Skeleton(AssetManager* a) {
			mAssetManager = a;
		}

		struct Bone {

			Bone(Quaternion q, Vector3 v, std::string s, int i) {

				mBoneTransform = BoneTransform(q, v);

				mName = s;
				mParentIndex = i;

			}

			BoneTransform mBoneTransform = BoneTransform(Quaternion::Identity, Vector3::Zero);
			std::string mName;
			int mParentIndex = -1;

		};

		size_t GetNumBones() const;
		const Bone& GetBone(size_t idx) const;
		const std::vector<Bone>& GetBones() const;
		const std::vector<Matrix4>& GetGlobalInvBindPoses() const;

		static Skeleton* StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager);
		bool Load(const WCHAR* fileName);
		
	private:
		std::vector<Bone> mBones;
		std::vector<Matrix4> mInverseBindPoses;
		AssetManager* mAssetManager;

		void ComputeGlobalInvBindPose();

};