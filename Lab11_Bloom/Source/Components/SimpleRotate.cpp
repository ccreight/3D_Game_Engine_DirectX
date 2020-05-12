#include "stdafx.h"
#include "SimpleRotate.h"
#include "RenderObj.h"

SimpleRotate::SimpleRotate(RenderObj* pObj) : Component(pObj) {

}

void SimpleRotate::LoadProperties(const rapidjson::Value& properties) {

	mSpeed = properties["speed"].GetDouble();

}

void SimpleRotate::Update(float deltaTime) {

	mObj->mMatrix.perObjConst *= Matrix4::CreateRotationZ(mSpeed * deltaTime);

}