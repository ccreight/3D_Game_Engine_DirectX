#pragma once
#include "Component.h"
#include "Physics.h"

class CollisionComponent : public Component {

	public:
		CollisionComponent(class RenderObj* pObj);
		void LoadProperties(const rapidjson::Value& properties);
		Physics::AABB GetAABB() const;

	private:
		Physics::AABB mAABB;

};