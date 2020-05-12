#include "stdafx.h"
#include "Graphics.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Graphics* graphics, const void* vertData, uint32_t vertCount,
	uint32_t vertStride, const void* indexData, uint32_t indexCount, uint32_t indexStride) {

	// Create vertex data
	mGraphics = graphics;
	mNumVertices = vertCount;
	mStride = vertStride;
	mBuffer = mGraphics->CreateGraphicsBuffer(vertData, vertCount * vertStride, D3D11_BIND_VERTEX_BUFFER,
		D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);

	// Create index data
	mNumIndices = indexCount;
	mIndicesStride = indexStride;
	mIndexBuffer = mGraphics->CreateGraphicsBuffer(indexData, indexCount * indexStride, D3D11_BIND_INDEX_BUFFER,
		D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);

	// Set size depending on the stride
	if (indexStride == 1) {
		mIndexBufferFormat = DXGI_FORMAT::DXGI_FORMAT_R8_UINT;
	}
	else if (indexStride == 2) {
		mIndexBufferFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
	}
	else  {
		mIndexBufferFormat = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
	}

}

VertexBuffer::~VertexBuffer() {

	// Release both buffers
	mBuffer->Release();
	mIndexBuffer->Release();

}

void VertexBuffer::Draw() const {

	UINT offsets = 0;
	mGraphics->GetDeviceContext()->IASetVertexBuffers(0, 1, &mBuffer, &mStride, &offsets);

	// Set the index buffer and call the indexed draw
	mGraphics->GetDeviceContext()->IASetIndexBuffer(mIndexBuffer, mIndexBufferFormat, offsets);
	mGraphics->GetDeviceContext()->DrawIndexed(mNumIndices, 0, 0);

}