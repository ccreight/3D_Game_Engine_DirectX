#pragma once

#include "stdafx.h"
#include "simdMath.h"
#include "Graphics.h"

struct perObjConst {
	Matrix4 perObjConst;
};

class RenderObj {

	public:
		//RenderObj(class Graphics* graphics,
		//	const class VertexBuffer* vertexBuffer, const class Shader* shader);
		RenderObj(class Graphics* graphics,  class Mesh* pMesh, class Game* game);
		virtual ~RenderObj();
		virtual void Draw();
		perObjConst mMatrix;
		void UpdateRotation(float r);
		//void SetTexture(int slot, const class Texture* texture);
		void AddComponent(class Component* pComp);
		virtual void Update(float deltaTime);

		class Game* GetGame() {
			return mGame;
		}

	protected:
		class Graphics* mGraphics;
		ID3D11Buffer* mObjBuffer;
		class Mesh* mMesh;
		std::vector<class Component*> mComponents;
		class Game* mGame;

};