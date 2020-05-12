#include "stdafx.h"
#include "BoneTransform.h"

Matrix4 BoneTransform::ToMatrix() const {

	Matrix4 rot = Matrix4::CreateFromQuaternion(mRotation);
	Matrix4 trans = Matrix4::CreateTranslation(mTranslation);

	return rot * trans;

}

BoneTransform BoneTransform::Interpolate(
	const BoneTransform& a, const BoneTransform& b, float f) {

	Quaternion q = Lerp(a.GetQuat(), b.GetQuat(), f);
	Vector3 v = Lerp(a.GetVector(), b.GetVector(), f);

	return BoneTransform(q, v);

}