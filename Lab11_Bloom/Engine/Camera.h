#pragma once
#include "simdMath.h"
#include "engineMath.h"

struct perCameraConst {
	Matrix4 c_viewProj;
	Vector3 c_cameraPosition;
	float padding;
};

class Camera {

	public:
		Camera(class Graphics* g);
		virtual ~Camera();
		void SetActive();
		void SetWTCMatrix(Matrix4 m) {
			mWorldToCamera = m;
		}

		void SetViewMat(Matrix4 m) {
			mProjection = m;
		}

	protected:
		class Graphics* mGraphics;
		perCameraConst mObjData;
		ID3D11Buffer* mBuffer;
		Matrix4 mWorldToCamera;
		Matrix4 mProjection;

	private:


};