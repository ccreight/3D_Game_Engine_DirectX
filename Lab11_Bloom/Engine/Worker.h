#pragma once
#include "stdafx.h"
#include "Job.h"
#include <thread>

class JobManager;

class Worker {

	public:
		Worker(class JobManager* m);
		void Begin();
		void End();
		static void Loop();

	private:
		Job* mCurrentJob;
		std::thread mThread; 
		static class JobManager* mManager;

};