#include "stdafx.h"
#include "Animation.h"
#include <fstream>
#include <sstream>
#include "jsonUtil.h"
#include "BoneTransform.h"

Animation::Animation(AssetManager* a) {
	mAssetManager = a;
}

uint32_t Animation::GetNumBones() const {
	return mNumBones;
}

uint32_t Animation::GetNumFrames() const {
	return mNumFrames;
}

float Animation::GetLength() const {
	return mAnimLength;
}

void Animation::GetGlobalPoseAtTime(
	std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime) const {

	outPoses.resize(inSkeleton->GetNumBones());

	float timePerFrame = GetLength() / (GetNumFrames() - 1);
	int currentFrame = (int)(inTime / timePerFrame);

	for (int i = 0; i < inSkeleton->GetNumBones(); i++) {

		if (mData[i].size() == 0) {

			Matrix4 holder = Matrix4::Identity;
			
			if ((inSkeleton->GetBones())[i].mParentIndex != -1) {
				holder = holder * (outPoses[(inSkeleton->GetBones())[i].mParentIndex]);
			}

			outPoses[i] = holder;
			continue;
		}

		// LERP HERE
		size_t nextFrame = currentFrame + 1;
		if (nextFrame >= GetNumFrames()) {
			nextFrame = 0;
		}

		Matrix4 lerpedMat = (BoneTransform::Interpolate(mData[i][currentFrame], mData[i][nextFrame], (inTime / timePerFrame) - currentFrame)).ToMatrix();

		Matrix4 temp = mData[i][currentFrame].ToMatrix();

		if ((inSkeleton->GetBones())[i].mParentIndex != -1) {
			temp = temp * (outPoses[((inSkeleton->GetBones())[i]).mParentIndex]);
			// LERP here too?
			lerpedMat = lerpedMat * (outPoses[(inSkeleton->GetBones())[i].mParentIndex]);
		}

		//outPoses.push_back(lerpedMat);
		outPoses[i] = lerpedMat;
		//outPoses[i] = temp;

	}

}

Animation* Animation::StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager) {

	Animation* anim = new Animation(pAssetManager);

	if (false == anim->Load(fileName))
	{
		delete anim;
		return nullptr;
	}

	return anim;

}

bool Animation::Load(const WCHAR* fileName) {

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

	mNumFrames = doc["sequence"]["frames"].GetInt();
	mNumBones = doc["sequence"]["bonecount"].GetInt();
	mAnimLength = doc["sequence"]["length"].GetDouble();

	// Initialize all bones' transform vectors
	for (rapidjson::SizeType i = 0; i < mNumBones; i++) {
		std::vector<BoneTransform> temp;
		mData.push_back(temp);
	}

	const rapidjson::Value& tracks = doc["sequence"]["tracks"];
	if (!tracks.IsArray() || tracks.Size() < 1)
	{
		return false;
	}

	mData.resize(mNumBones);

	for (rapidjson::SizeType i = 0; i < tracks.Size(); i++) {

		int index = tracks[i]["bone"].GetInt();

		const rapidjson::Value& transforms = tracks[i]["transforms"];

		mData[index].resize(mNumFrames);

		for (rapidjson::SizeType j = 0; j < transforms.Size(); j++) {

			Quaternion rot;
			bool returned = GetQuaternionFromJSON(transforms[j], "rot", rot);

			Vector3 trans;
			GetVectorFromJSON(transforms[j], "trans", trans);

			mData[index][j] = BoneTransform(rot, trans);

		}

	}

	return true;

}