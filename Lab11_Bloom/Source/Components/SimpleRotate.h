#pragma once
#include "Component.h"

class SimpleRotate : public Component {

	public:
		SimpleRotate(RenderObj* pObj);
		void LoadProperties(const rapidjson::Value& properties);
		void Update(float deltaTime);

	private:
		double mSpeed;

};