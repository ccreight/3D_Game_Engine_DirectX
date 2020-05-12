#pragma once
#include "Component.h"

class PointLight : public Component {

	public:
		PointLight(class RenderObj* pObj, class Game* pGame);
		~PointLight();

		void LoadProperties(const rapidjson::Value& properties);
		void Update(float deltaTime);

	private:
		struct PointLightData* mData;
		class Game* mGame;

};