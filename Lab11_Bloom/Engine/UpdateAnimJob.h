#pragma once
#include "Job.h"

class UpdateAnimJob : public Job {

	public:
		UpdateAnimJob(const class Animation* anim, class Skeleton* skele, float currtime, class SkinnedObj* obj);

		void DoIt();

	private:
		const class Animation* mCurrentAnim;
		class Skeleton* mSkeleton;
		float mCurrentTime;
		class SkinnedObj* mSkinnedObj;

};