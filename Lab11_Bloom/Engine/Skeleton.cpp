#include "stdafx.h"
#include "Skeleton.h"
#include <fstream>
#include <sstream>
#include "jsonUtil.h"

void Skeleton::ComputeGlobalInvBindPose() {

	mInverseBindPoses.resize(mBones.size());

	for (int i = 0; i < mBones.size(); i++) {

		Matrix4 invBP = mBones[i].mBoneTransform.ToMatrix();

		if (mBones[i].mParentIndex != -1) {
			invBP = invBP * mInverseBindPoses[mBones[i].mParentIndex];
		}

		mInverseBindPoses[i] = invBP;

	}

	for (int i = 0; i < mInverseBindPoses.size(); i++){ 
		mInverseBindPoses[i].Invert();
	}

}

size_t Skeleton::GetNumBones() const {

	return mBones.size();

}

const Skeleton::Bone& Skeleton::GetBone(size_t idx) const {

	return mBones[idx];

}

const std::vector<Skeleton::Bone>& Skeleton::GetBones() const {

	return mBones;

}

const std::vector<Matrix4>& Skeleton::GetGlobalInvBindPoses() const {

	return mInverseBindPoses;

}

Skeleton* Skeleton::StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager) {

	Skeleton* s = new Skeleton(pAssetManager);

	if (false == s->Load(fileName))
	{
		delete s;
		return nullptr;
	}

	return s;

}

bool Skeleton::Load(const WCHAR* fileName) {

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

	int boneCount = doc["bonecount"].GetInt();

	const rapidjson::Value& fileBones = doc["bones"];
	if (!fileBones.IsArray() || fileBones.Size() < 1)
	{
		return false;
	}

	for (int i = 0; i < boneCount; i++) {

		std::string name = fileBones[i]["name"].GetString();
		int parentIndex = fileBones[i]["parent"].GetInt();
		
		Quaternion rot;
		GetQuaternionFromJSON(fileBones[i]["bindpose"], "rot", rot);

		Vector3 trans;
		GetVectorFromJSON(fileBones[i]["bindpose"], "trans", trans);

		Skeleton::Bone b = Bone(rot, trans, name, parentIndex);
		mBones.push_back(b);

	}

	ComputeGlobalInvBindPose();
	return true;

}