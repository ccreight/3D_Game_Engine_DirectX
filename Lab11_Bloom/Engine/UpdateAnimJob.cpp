#include "stdafx.h"
#include "UpdateAnimJob.h"
#include "engineMath.h"
#include "Animation.h"
#include "Skeleton.h"
#include "SkinnedObj.h"

UpdateAnimJob::UpdateAnimJob(const Animation* anim, Skeleton* skele, float currtime, SkinnedObj* obj) {
	mCurrentAnim = anim;
	mSkeleton = skele;
	mCurrentTime = currtime;
	mSkinnedObj = obj;
}

void UpdateAnimJob::DoIt() {

	std::vector<Matrix4> mPoses;
	mCurrentAnim->GetGlobalPoseAtTime(mPoses, mSkeleton, mCurrentTime);

	std::vector<Matrix4> posesTwo;
	posesTwo = mSkeleton->GetGlobalInvBindPoses();

	for (int i = 0; i < mPoses.size(); i++) {
		mSkinnedObj->mSkinConstants.c_skinMatrix[i] = mSkeleton->GetGlobalInvBindPoses()[i] * mPoses[i];
	}

}