#include "stdafx.h"
#include "Physics.h"
#include "CollisionComponent.h"

struct TestPhysicsAABB
{
	Physics::AABB a;
	Physics::AABB b;
	Physics::AABB overlap;
};
const TestPhysicsAABB testAABB[] =
{
	{
	Physics::AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f)),
	Physics::AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f))
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::AABB(Vector3(-110.0f, -10.0f, -10.0f), Vector3(-90.0f, 10.0f, 10.0f)),
	Physics::AABB(Vector3(-100.0f, -10.0f, -10.0f), Vector3(-90.0f, 10.0f, 10.0f))
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::AABB(Vector3(90.0f, -10.0f, -10.0f), Vector3(110.0f, 10.0f, 10.0f)),
	Physics::AABB(Vector3(90.0f, -10.0f, -10.0f), Vector3(100.0f, 10.0f, 10.0f))
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::AABB(Vector3(-10.0f, -110.0f, -10.0f), Vector3(10.0f, -90.0f, 10.0f)),
	Physics::AABB(Vector3(-10.0f, -100.0f, -10.0f), Vector3(10.0f, -90.0f, 10.0f))
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::AABB(Vector3(-10.0f, 90.0f, -10.0f), Vector3(10.0f, 110.0f, 10.0f)),
	Physics::AABB(Vector3(-10.0f, 90.0f, -10.0f), Vector3(10.0f, 100.0f, 10.0f))
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::AABB(Vector3(-10.0f, -10.0f, -110.0f), Vector3(10.0f, 10.0f, -90.0f)),
	Physics::AABB(Vector3(-10.0f, -10.0f, -100.0f), Vector3(10.0f, 10.0f, -90.0f))
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::AABB(Vector3(-10.0f, -10.0f, 90.0f), Vector3(10.0f, 10.0f, 110.0f)),
	Physics::AABB(Vector3(-10.0f, -10.0f, 90.0f), Vector3(10.0f, 10.0f, 100.0f))
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::AABB(Vector3(-120.0f, -10.0f, -10.0f), Vector3(-110.0f, 10.0f, 10.0f)),
	Physics::AABB(Vector3::One, Vector3::Zero)
	},
	{
	 Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	 Physics::AABB(Vector3(110.0f, -10.0f, -10.0f), Vector3(120.0f, 10.0f, 10.0f)),
	 Physics::AABB(Vector3::One, Vector3::Zero)
	 },
	 {
	 Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	 Physics::AABB(Vector3(-10.0f, -120.0f, -10.0f), Vector3(10.0f, -110.0f, 10.0f)),
	 Physics::AABB(Vector3::One, Vector3::Zero)
	 },
	 {
	 Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	 Physics::AABB(Vector3(-10.0f, 110.0f, -10.0f), Vector3(10.0f, 120.0f, 10.0f)),
	 Physics::AABB(Vector3::One, Vector3::Zero)
	 },
	 {
	 Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	 Physics::AABB(Vector3(-10.0f, -10.0f, -120.0f), Vector3(10.0f, -10.0f, -110.0f)),
	 Physics::AABB(Vector3::One, Vector3::Zero)
	 },
	 {
	 Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	 Physics::AABB(Vector3(-10.0f, -10.0f, 110.0f), Vector3(10.0f, 10.0f, 120.0f)),
	 Physics::AABB(Vector3::One, Vector3::Zero)
	 }

};

struct TestSegment
{
	Physics::AABB box;
	Physics::LineSegment segment;
	bool hit;
	Vector3 point;
};
const TestSegment testSegment[] =
{
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::LineSegment(Vector3(-110.0f, 0.0f, 0.0f), Vector3(-90.0f, 0.0f, 0.0f)),
	true, Vector3(-100.0f, 0.0f, 0.0f)
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::LineSegment(Vector3(0.0f, -110.0f, 0.0f), Vector3(0.0f, -90.0f, 0.0f)),
	true, Vector3(0.0f, -100.0f, 0.0f)
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::LineSegment(Vector3(0.0f, 0.0f, -110.0f), Vector3(0.0f, 0.0f, -90.0f)),
	true, Vector3(0.0f, 0.0f, -100.0f)
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::LineSegment(Vector3(110.0f, 0.0f, 0.0f), Vector3(90.0f, 0.0f, 0.0f)),
	true, Vector3(100.0f, 0.0f, 0.0f)
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::LineSegment(Vector3(0.0f, 110.0f, 0.0f), Vector3(0.0f, 90.0f, 0.0f)),
	true, Vector3(0.0f, 100.0f, 0.0f)
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::LineSegment(Vector3(0.0f, 0.0f, 110.0f), Vector3(0.0f, 0.0f, 90.0f)),
	true, Vector3(0.0f, 0.0f, 100.0f)
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::LineSegment(Vector3(-120.0f, 0.0f, 0.0f), Vector3(-110.0f, 0.0f, 0.0f)),
	false, Vector3::Zero
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::LineSegment(Vector3(0.0f, -120.0f, 0.0f), Vector3(0.0f, -110.0f, 0.0f)),
	false, Vector3::Zero
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::LineSegment(Vector3(0.0f, 0.0f, -120.0f), Vector3(0.0f, 0.0f, -110.0f)),
	false, Vector3::Zero
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::LineSegment(Vector3(120.0f, 0.0f, 0.0f), Vector3(110.0f, 0.0f, 0.0f)),
	false, Vector3::Zero
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::LineSegment(Vector3(0.0f, 120.0f, 0.0f), Vector3(0.0f, 110.0f, 0.0f)),
	false, Vector3::Zero
	},
	{
	Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	Physics::LineSegment(Vector3(0.0f, 0.0f, 120.0f), Vector3(0.0f, 0.0f, 110.0f)),
	false, Vector3::Zero
	},
};

Physics::AABB::AABB() {

	mMin = Vector3::Zero;
	mMax = Vector3::Zero;

}

Physics::AABB::AABB(Vector3 min, Vector3 max) {

	mMin = min;
	mMax = max;

}

Physics::LineSegment::LineSegment(Vector3 start, Vector3 end) {
	mStart = start;
	mEnd = end;
}

bool Physics::Intersect(const AABB& a, const AABB& b, AABB* pOverlap) {

	if ((a.mMax.x < b.mMin.x ||
		a.mMax.y < b.mMin.y ||
		a.mMax.z < b.mMin.z ||
		b.mMax.x < a.mMin.x ||
		b.mMax.y < a.mMin.y ||
		b.mMax.z < a.mMin.z)) {
		return false;
	}

	pOverlap->mMin.x = (a.mMin.x > b.mMin.x ? a.mMin.x : b.mMin.x);
	pOverlap->mMin.y = (a.mMin.y > b.mMin.y ? a.mMin.y : b.mMin.y);
	pOverlap->mMin.z = (a.mMin.z > b.mMin.z ? a.mMin.z : b.mMin.z);

	pOverlap->mMax.x = (a.mMax.x < b.mMax.x ? a.mMax.x : b.mMax.x);
	pOverlap->mMax.y = (a.mMax.y < b.mMax.y ? a.mMax.y : b.mMax.y);
	pOverlap->mMax.z = (a.mMax.z < b.mMax.z ? a.mMax.z : b.mMax.z);

	return true;

}

bool Physics::Intersect(const LineSegment& segment, const AABB& box, Vector3* pHitPoint) {

	float t_min = 0.0f;
	float t_max = FLT_MAX;
	Vector3 direction = segment.mEnd - segment.mStart;
	direction.Normalize();

	for (int i = 0; i < 3; i++) {

		if (abs(direction.Get(i)) < 1) { // is this epsilon checking correct?
			if (segment.mStart.Get(i) < box.mMin.Get(i) || segment.mStart.Get(i) > box.mMax.Get(i)) return 0;
		}
		else {

			float ood = 1.0f / direction.Get(i);
			float t1 = (box.mMin.Get(i) - segment.mStart.Get(i)) * ood;
			float t2 = (box.mMax.Get(i) - segment.mStart.Get(i)) * ood;

			if (t1 > t2) {
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}

			if (t1 > t_min) {
				t_min = t1;
			}

			if (t2 < t_max) {
				t_max = t2;
			}

			if (t_min > t_max) {
				return 0;
			}

		}

	}

	*pHitPoint = segment.mStart + direction * t_min;

	direction = segment.mEnd - segment.mStart;
	Vector3 cross = Cross(direction, *pHitPoint - segment.mStart);

	if (cross.Length() > .01) {
		return false;
	}

	int dot = Dot(direction, *pHitPoint - segment.mStart);
	int sqrlen = direction.LengthSq();

	if (dot < 0 || dot > sqrlen) {
		return false;
	}

	return true;

}

bool Physics::UnitTest() {

	// AABB Intersection test
	for (int i = 0; i < 13; i++) {

		AABB* overlapTest = new AABB();
		Intersect(testAABB[i].a, testAABB[i].b, overlapTest);

		if (overlapTest->mMax.x != testAABB[i].overlap.mMax.x || overlapTest->mMax.y != testAABB[i].overlap.mMax.y
			|| overlapTest->mMin.x != testAABB[i].overlap.mMin.x || overlapTest->mMin.y != testAABB[i].overlap.mMin.y
			|| overlapTest->mMin.z != testAABB[i].overlap.mMin.z || overlapTest->mMax.z != testAABB[i].overlap.mMax.z) {
			return false;
		}

	}

	// Ray, AABB Intersection test
	for (int i = 0; i < 12; i++) {

		Vector3 point;
		bool rayTest = Intersect(testSegment[i].segment, testSegment[i].box, &point);

		if (rayTest != testSegment[i].hit) {
			return false;
		}

		if (rayTest) {
			for (int j = 0; j < 3; j++) {
				if (point.Get(j) != testSegment[i].point.Get(j)) {
					return false;
				}
			}
		}

	}

	return true;

}

bool Physics::RayCast(const LineSegment& segment, Vector3* pHitPoint) {

	Vector3 currOffset(999, 999, 999);

	for (int i = 0; i < mCollisionComps.size(); i++) {

		AABB temp = mCollisionComps[i]->GetAABB(); 
		Vector3 point;
		bool rayTest = Intersect(segment, temp, &point);

		if (rayTest && ((point - segment.mStart).Length() < (currOffset - segment.mStart).Length())) {
			currOffset = point;
		}

	}

	if (abs(currOffset.x - 999) < 1) {
		return false;
	}

	pHitPoint = &currOffset;
	return true;

}