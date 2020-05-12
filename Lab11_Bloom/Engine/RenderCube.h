#pragma once
#include "RenderObj.h"
#include "assetManager.h"

class RenderCube : public RenderObj {

	public:
		//RenderCube(class Graphics* graphics, const class Shader* shader);
		RenderCube(AssetManager* pAssetManager, const Shader* pShader, const Texture* pTex);
		~RenderCube();
};