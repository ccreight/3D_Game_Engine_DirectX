#include "stdafx.h"
#include "Graphics.h"
#include "Shader.h"
#include "engineMath.h"

#pragma comment (lib, "d3d11.lib") 

/*static*/ Graphics* Graphics::s_theGraphics = nullptr;


Graphics::Graphics()
    : mScreenWidth(0)
    , mScreenHeight(0)
{
	//mTexSlot = TextureSlot(1);
    DbgAssert(nullptr == s_theGraphics, "You can only have 1 Graphics");
    s_theGraphics = this;
}

Graphics::~Graphics()
{
    DbgAssert(this == s_theGraphics, "There can only be 1 Graphics");
    s_theGraphics = nullptr;
}

void Graphics::InitD3D(HWND hWnd, float width, float height)
{
    mScreenWidth = width;
    mScreenHeight = height;

	// Init the swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = (UINT)width;
	sd.BufferDesc.Height = (UINT)height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	D3D_FEATURE_LEVEL  FeatureLevelsSupported;
	D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		&FeatureLevelsRequested, 1, D3D11_SDK_VERSION, &sd, &mSwapChain, &mDevice, &FeatureLevelsSupported, &mDeviceContext);
	
	// Init the view
	D3D11_VIEWPORT view;
	ZeroMemory(&view, sizeof(view));
	view.Width = width;
	view.Height = height;
	view.MaxDepth = 1.0f;
	view.MinDepth = 0.0f;
	mDeviceContext->RSSetViewports(1, &view);

    // Init the texture and create the target view
	HRESULT hr;
	ID3D11Texture2D* texture;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&texture);
	mDevice->CreateRenderTargetView(texture, NULL, &mTargetView);
	texture->Release();

	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CreateDepthStencil((int)width, (int)height, &mDepthTexture, &mDepthStencilView);

	// Create depth stencil state
	mDepthStencilState = CreateDepthStencilState(true, D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS); 

	// Creating sampler state
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	mDevice->CreateSamplerState(&samplerDesc, &mSamplerState);
	SetActiveSampler(0, mSamplerState); 

	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(rastDesc));
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = true;

	ID3D11RasterizerState* rastState;
	mDevice->CreateRasterizerState(&rastDesc, &rastState);
	mDeviceContext->RSSetState(rastState);
	rastState->Release();

}

void Graphics::UploadBuffer(ID3D11Buffer* buffer, const void* data, size_t dataSize) {

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, data, dataSize);
	GetDeviceContext()->Unmap(buffer, 0);

}

bool Graphics::CreateDepthStencil(int inWidth, int inHeight,
	ID3D11Texture2D** pDepthTexture, ID3D11DepthStencilView** pDepthView) {

	// Create the 2D texture description
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = inWidth;
	desc.Height = inHeight;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	GetDevice()->CreateTexture2D(&desc, NULL, pDepthTexture);

	// Create depth stencil state
	ID3D11DepthStencilState* pDSState = nullptr;
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;


	GetDevice()->CreateDepthStencilView(*pDepthTexture, &descDSV, pDepthView);

	return true;

}

ID3D11DepthStencilState* Graphics::CreateDepthStencilState(
	bool inDepthTestEnable, D3D11_COMPARISON_FUNC inDepthComparisonFunction) {

	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	GetDevice()->CreateDepthStencilState(&dsDesc, &mDepthStencilState);

	return mDepthStencilState;

}

ID3D11Buffer* Graphics::CreateGraphicsBuffer(const void* initialData, int inDataSize,
	D3D11_BIND_FLAG inBindFlags, D3D11_CPU_ACCESS_FLAG inCPUAccessFlags, D3D11_USAGE inUsage) {

	ID3D11Buffer* buffer;

	// Create values for the vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = inDataSize;
	vertexBufferDesc.Usage = inUsage;
	vertexBufferDesc.BindFlags = inBindFlags;
	vertexBufferDesc.CPUAccessFlags = inCPUAccessFlags;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	GetDevice()->CreateBuffer(&vertexBufferDesc, NULL, &buffer);

	// Init mBuffer
	if (initialData != NULL) {
		UploadBuffer(buffer, initialData, inDataSize);
	}
	
	return buffer;

}

ID3D11BlendState* Graphics::CreateBlendState(
	bool enable, D3D11_BLEND srcBlend, D3D11_BLEND dstBlend) {

	ID3D11BlendState* mBlendState;

	D3D11_BLEND_DESC mBlendDesc;
	ZeroMemory(&mBlendDesc, sizeof(D3D11_BLEND_DESC));

	mBlendDesc.RenderTarget[0].BlendEnable = enable;
	mBlendDesc.RenderTarget[0].SrcBlend = srcBlend;
	mBlendDesc.RenderTarget[0].DestBlend = dstBlend;
	mBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	mBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	mBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	mBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	mBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	mDevice->CreateBlendState(&mBlendDesc, &mBlendState);

	return mBlendState;

}

void Graphics::SetBlendState(ID3D11BlendState* inBlendState)
{
	float blendFactor[4];
	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	mDeviceContext->OMSetBlendState(inBlendState, blendFactor, 0xffffffff);
}

void Graphics::SetActiveTexture(int slot, ID3D11ShaderResourceView* pView) {

	mDeviceContext->PSSetShaderResources(slot, 1, &pView);

}

void Graphics::SetActiveSampler(int slot, ID3D11SamplerState* pSampler) {

	mDeviceContext->PSSetSamplers(slot, 1, &pSampler);

}

void Graphics::SetRenderTarget(ID3D11RenderTargetView* renderTarget,
	ID3D11DepthStencilView* depthView) {

	mCurrView = renderTarget;
	mDeviceContext->OMSetRenderTargets(1, &renderTarget, depthView);

}

void Graphics::BeginFrame(const Color4& clearColor)
{
	mDeviceContext->OMSetRenderTargets(1, &mCurrView, mDepthStencilView);
	mDeviceContext->ClearRenderTargetView(mCurrView, (float*)&clearColor);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void Graphics::SetViewport(float x, float y, float width, float height) {

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));

	viewport.TopLeftX = x;
	viewport.TopLeftY = y;
	viewport.MinDepth = 0.0;
	viewport.MaxDepth = 1.0;
	viewport.Width = width;
	viewport.Height = height;
	
	mDeviceContext->RSSetViewports(1, &viewport);

}

void Graphics::EndFrame()
{
	mSwapChain->Present(1, 0);
}

void Graphics::CleanD3D()
{

	// Release pointers
	mDevice->Release();
	mDeviceContext->Release();
	mSwapChain->Release();
	mTargetView->Release();

	mDepthStencilState->Release();
	mDepthStencilView->Release();
	mDepthTexture->Release();

	mSamplerState->Release();

}
