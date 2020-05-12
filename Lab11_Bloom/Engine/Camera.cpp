#include "stdafx.h"
#include "Camera.h"
#include "Graphics.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "RenderObj.h"

Camera::Camera(Graphics* g) {

	// Set graphics
	mGraphics = g;

	// Create the WorldToCamera and Projection matrices
	mWorldToCamera = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, -100.f));
	//mProjection = Matrix4::CreateOrtho(mGraphics->GetScreenWidth(), mGraphics->GetScreenHeight(),
	//	1000.0f, -1000.0f);
	mProjection = Matrix4::CreateRotationY(-Math::PiOver2)
		* Matrix4::CreateRotationZ(-Math::PiOver2)
		* Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
			mGraphics->GetScreenWidth(), mGraphics->GetScreenHeight(),
			25.0f, 10000.0f);

	// Create the graphics buffer
	mBuffer = mGraphics->CreateGraphicsBuffer(&mObjData, sizeof(mObjData), D3D11_BIND_CONSTANT_BUFFER, 
		D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);

}

Camera::~Camera() {
	mBuffer->Release();
}

void Camera::SetActive() {

	// Set the view projection
	mObjData.c_viewProj = mWorldToCamera * mProjection;

	// Set the camera's position
	Matrix4 inverse = mWorldToCamera;
	inverse.Invert();
	mObjData.c_cameraPosition = inverse.GetTranslation();

	// Buffer operations
	mGraphics->UploadBuffer(mBuffer, &mObjData, sizeof(mObjData));
	mGraphics->GetDeviceContext()->VSSetConstantBuffers(Graphics::CONSTANT_BUFFER_CAMERA, 1, &mBuffer);
	mGraphics->GetDeviceContext()->PSSetConstantBuffers(Graphics::CONSTANT_BUFFER_CAMERA, 1, &mBuffer);

}