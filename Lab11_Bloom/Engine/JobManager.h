#pragma once
#include "stdafx.h"
#include <mutex>
#include <queue>
#include <atomic>
#include <thread>

class JobManager {

	public:
		std::mutex mMut;

		JobManager();
		void Begin();
		void End();
		void AddJob(class Job* pJob);
		void WaitForJobs();
		bool GetShutdownSignal();

		std::queue<Job*>& GetJobs() {
			return mJobs;
		}

		std::atomic<int> mJobCount = 0;

	private:
		std::vector<class Worker*> mWorkers;
		bool mShuttingDown;
		std::queue<Job*> mJobs;

};