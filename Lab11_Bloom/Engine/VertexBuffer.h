#pragma once

#include "stdafx.h"
#include "Graphics.h"

class VertexBuffer {

	public:
		VertexBuffer(class Graphics* graphics, const void* vertData, UINT vertCount, 
			UINT vertStride, const void* indexData, UINT indexCount, UINT indexStride);
		~VertexBuffer();
		void Draw() const;

	private:
		ID3D11Buffer* mBuffer;
		UINT mStride;
		UINT mNumVertices;
		class Graphics* mGraphics;

		UINT mNumIndices;
		UINT mIndicesStride;
		ID3D11Buffer* mIndexBuffer;
		DXGI_FORMAT mIndexBufferFormat;
};