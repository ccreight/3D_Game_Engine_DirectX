#pragma once
#include "stdafx.h"
#include "RenderObj.h"

#define MAX_SKELETON_BONES 80

struct SkinnedConstants {
	Matrix4 c_skinMatrix[MAX_SKELETON_BONES];
};

class SkinnedObj : public RenderObj {

	public:
		SkinnedObj(class Graphics* g, class Mesh* m, class AssetManager* a, class Game* game);
		~SkinnedObj() {
			mConstantBuffer->Release();
		}
		SkinnedConstants mSkinConstants;
		void Draw();

	private:
		ID3D11Buffer* mConstantBuffer;
		//class Character* mCharacter;

};