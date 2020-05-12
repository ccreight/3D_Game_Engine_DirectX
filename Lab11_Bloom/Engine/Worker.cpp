#include "stdafx.h"
#include "Worker.h"
#include <thread>
#include "JobManager.h"

JobManager* Worker::mManager = nullptr;
std::mutex mMutex;

Worker::Worker(class JobManager* m) {

	mCurrentJob = nullptr;
	mManager = m;

}

void Worker::Begin() {

	mThread = std::thread(Loop);

}

void Worker::End() {

	mThread.join();
	delete mCurrentJob;

}

void Worker::Loop() {

	while (!mManager->GetShutdownSignal()) {

		//std::this_thread::sleep_for(std::chrono::nanoseconds(1));

		if (!mManager->GetJobs().empty()) {

			//JobManager::mMut.lock();
			//mMutex.lock();
			mManager->mMut.lock();
			Job* job = nullptr;

			if (mManager->GetJobs().empty()) {
				mManager->mJobCount = 0;
			}

			if (mManager != nullptr && !mManager->GetJobs().empty()) {
				job = mManager->GetJobs().front();
				mManager->GetJobs().pop();
				mManager->mJobCount--;
			}

			//mMutex.unlock();
			mManager->mMut.unlock();
			//JobManager::mMut.unlock();

			if (job != nullptr) {
				job->DoIt();
			}

		}

	}

}