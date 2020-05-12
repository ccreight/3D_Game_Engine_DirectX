#include "stdafx.h"
#include "Character.h"
#include "SkinnedObj.h"
#include "mesh.h"
#include "JobManager.h"
#include "UpdateAnimJob.h"
#include "game.h"
#include <mutex>

std::mutex mMute;

Character::Character(AssetManager* am, SkinnedObj* so) : Component(nullptr) {

	mAssetManager = am;
	mSkinnedObj = so;

}

void Character::LoadProperties(const rapidjson::Value& properties) {

	std::string skel = properties["skeleton"].GetString();
	const std::wstring skele(skel.begin(), skel.end());
	mSkeleton = mAssetManager->LoadSkeleton(skele);

	const rapidjson::Value& anims = properties["animations"];

	for (rapidjson::SizeType j = 0; j < anims.Size(); j++) {

		const char* shortName = anims[j][0].GetString();
		const char* filename = anims[j][1].GetString();

		std::string test = std::string(filename);
		const std::wstring holder(test.begin(), test.end());

		Animation* a = mAssetManager->LoadAnimation(holder);
		mAnims[shortName] = a;

	}

}

bool Character::SetAnim(const std::string& animName) {

	if (mAnims.find(animName) != mAnims.end()) {
		mCurrentAnim = mAnims[animName];
		mCurrentTime = 0.0f;
		return true;
	}

	return false;

}

void Character::UpdateAnim(float deltaTime) {

	if (mSkeleton == nullptr || mCurrentAnim == nullptr || mSkinnedObj == nullptr) {
		return;
	}

	mCurrentTime += deltaTime;

	while (mCurrentTime >= mCurrentAnim->GetLength()) {
		mCurrentTime -= mCurrentAnim->GetLength();
	}

	mMute.lock();
	UpdateAnimJob* animJob = new UpdateAnimJob(mCurrentAnim, mSkeleton, mCurrentTime, mSkinnedObj);
	mAssetManager->GetGame()->GetJobManager()->AddJob(animJob);
	mMute.unlock();

}

void Character::Update(float deltaTime) {

	if (mCurrentAnim == nullptr) {
		SetAnim("idle");
	}

	UpdateAnim(deltaTime);

}