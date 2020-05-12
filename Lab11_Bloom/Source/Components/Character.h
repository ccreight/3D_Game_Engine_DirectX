#pragma once
#include "SkinnedObj.h"
#include "Skeleton.h"
#include "Animation.h"
#include "jsonUtil.h"
#include "assetManager.h"
#include "Component.h"

class Character : public Component {

	public:
		Character(AssetManager* am, SkinnedObj* so);
		void LoadProperties(const rapidjson::Value& properties) override;
		bool SetAnim(const std::string& animName);
		void UpdateAnim(float deltaTime);
		void Update(float deltaTime);

	protected:
		class SkinnedObj* mSkinnedObj = nullptr;
		class Skeleton* mSkeleton = nullptr;
		std::unordered_map<std::string, const Animation*> mAnims;
		const Animation* mCurrentAnim = nullptr;
		float mCurrentTime = 0.0f;
		AssetManager* mAssetManager;

};