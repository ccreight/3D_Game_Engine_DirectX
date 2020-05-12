#include "stdafx.h"
#include "CollisionComponent.h"
#include "RenderObj.h"
#include "../Source/game.h"

CollisionComponent::CollisionComponent(RenderObj* pObj) : Component(pObj) {
	pObj->GetGame()->GetPhysics()->AddObj(this);
}

void CollisionComponent::LoadProperties(const rapidjson::Value& properties) {

	Vector3 min;
	GetVectorFromJSON(properties, "min", min);
	mAABB.mMin = min;

	Vector3 max;
	GetVectorFromJSON(properties, "max", max);
	mAABB.mMax = max;

}

Physics::AABB CollisionComponent::GetAABB() const {

	Vector3 scale = mObj->mMatrix.perObjConst.GetScale();

	Physics::AABB result = mAABB;
	result.mMax = mAABB.mMax * scale.x;
	result.mMin = mAABB.mMin * scale.x;

	Vector3 translation = mObj->mMatrix.perObjConst.GetTranslation();
	result.mMax += translation;
	result.mMin += translation;

	return result;

}