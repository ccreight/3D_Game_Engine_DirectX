#include "stdafx.h"
#include "JobManager.h"
#include "Worker.h"
#include "Job.h"
#include <thread>



JobManager::JobManager() {

	// Add four workers at the start
	for (int i = 0; i < 4; i++) {
		mWorkers.push_back(new Worker(this));
	}

	mShuttingDown = false;
	
}

void JobManager::Begin() {

	for (int i = 0; i < mWorkers.size(); i++) {
		mWorkers[i]->Begin();
	}

}

void JobManager::End() {
	
	mShuttingDown = true;

	for (int i = 0; i < mWorkers.size(); i++) {
		mWorkers[i]->End();
	}

}

bool JobManager::GetShutdownSignal() {
	return mShuttingDown;
}

void JobManager::AddJob(Job* pJob) {

	mMut.lock();
	mJobs.push(pJob);
	mJobCount++;
	mMut.unlock();

}

void JobManager::WaitForJobs() {

	while (mJobCount != 0) {

		std::this_thread::sleep_for(std::chrono::nanoseconds(1));

	}

}