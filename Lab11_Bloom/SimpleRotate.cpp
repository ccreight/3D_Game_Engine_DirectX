#include "SimpleRotate.h"
#include "RenderObj.h"

SimpleRotate::SimpleRotate(RenderObj* pObj) : Component(pObj) {
	mSpeed = 0.0f;
}

void SimpleRotate::LoadProperties(const rapidjson::Value& properties) {

	mSpeed = properties["speed"].GetDouble();

}

void SimpleRotate::Update(float deltaTime) {

	mObj->UpdateRotation(mSpeed * deltaTime);

}