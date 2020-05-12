#include "stdafx.h"
#include "PointLight.h"
#include "RenderObj.h"
#include "Game.h"

PointLight::PointLight(RenderObj* pObj, Game* pGame) : Component(pObj) {

	mGame = pGame;
	mData = mGame->AllocateLight();

}

PointLight::~PointLight() {

	mGame->FreeLight(mData);

}

void PointLight::LoadProperties(const rapidjson::Value& properties) {

	Vector3 diffuseColor;
	GetVectorFromJSON(properties, "diffuseColor", diffuseColor);

	Vector3 specularColor;
	GetVectorFromJSON(properties, "specularColor", specularColor);

	float specularPower = (float)(properties["specularPower"].GetDouble());
	float innerRadius = (float)(properties["innerRadius"].GetDouble());
	float outerRadius = (float)(properties["outerRadius"].GetDouble());

	mData->diffuseColor = diffuseColor;
	mData->specularColor = specularColor;
	mData->specularPower = specularPower;
	mData->innerRadius = innerRadius;
	mData->outerRadius = outerRadius;

}

void PointLight::Update(float deltaTime) {

	mData->position = Vector3(mObj->mMatrix.perObjConst.GetTranslation());
	// Use deltaTime?

}