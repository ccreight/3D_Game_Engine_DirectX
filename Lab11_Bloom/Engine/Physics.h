#pragma once
#include "stdafx.h"
#include "engineMath.h"

class Physics {

	public:
		
		class AABB {

			public:
				AABB();
				AABB(Vector3 min, Vector3 max);

				Vector3 mMin;
				Vector3 mMax;

		};

		class LineSegment {

			public:
				LineSegment(Vector3 start, Vector3 end);

				Vector3 mStart;
				Vector3 mEnd;

		};

		static bool Intersect(const AABB& a, const AABB& b,
			AABB* pOverlap = nullptr);
		static bool Intersect(const LineSegment& segment, const AABB& box,
			Vector3* pHitPoint = nullptr);
		static bool UnitTest();

		void AddObj(class CollisionComponent* pObj) {
			mCollisionComps.push_back(pObj);
		}

		// MAKE SURE THIS WORKS! erasing is always weird
		void RemoveObj(class CollisionComponent* pObj) {
			mCollisionComps.erase(std::find(mCollisionComps.begin(), mCollisionComps.end(), pObj));
		}

		bool RayCast(const LineSegment& segment, Vector3*
			pHitPoint = nullptr);

	private:
		std::vector<CollisionComponent*> mCollisionComps;

};