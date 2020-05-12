#pragma once
#include "stdafx.h"
#include "engineMath.h"

class BoneTransform {

	public:
		BoneTransform() {
			mRotation = Quaternion(0,0,0,1);
			mTranslation = Vector3(0,0,0);
		}

		BoneTransform(Quaternion q, Vector3 v) {
			mRotation = q;
			mTranslation = v;
		}

		Matrix4 ToMatrix() const;
		Quaternion GetQuat() const { return mRotation; }
		Vector3 GetVector() const { return mTranslation; }

		static BoneTransform Interpolate(
			const BoneTransform& a, const BoneTransform& b, float f);

	private:
		Quaternion mRotation;
		Vector3 mTranslation;

};