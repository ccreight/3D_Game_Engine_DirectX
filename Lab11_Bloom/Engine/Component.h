#pragma once
#include "RenderObj.h"
#include "jsonUtil.h"

class Component {

	public:
		Component(RenderObj* pObj);
		virtual ~Component() {}
		virtual void LoadProperties(const rapidjson::Value& properties) {}
		virtual void Update(float deltaTime) {}

	protected:
		RenderObj* mObj;

};