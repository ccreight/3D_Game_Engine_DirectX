#pragma once
#include "stdafx.h"
#include <stdio.h>

#define PROFILE_SCOPE(name) \
Profiler::ScopedTimer name##_scope(Profiler::Get()->GetTimer(std::string(#name)))

static bool mProfSet = false;


class Profiler {

	public:

		static Profiler* Get();

		class Timer {

			friend class Profiler;

			public:
				void Start();
				void Stop();
				void Reset();
				const std::string& GetName() const;
				double GetTime_ms() const;
				double GetMax_ms() const;
				double GetAvg_ms() const;

			protected:
				Timer() {

				};

				~Timer() {

				};

				void SetName(std::string name) {
					mPurpose = name;
				}

				std::string mPurpose;
				double mLastTime = 0;
				double mLongestTime = 0;
				double mTotalTime = 0;
				int mFramesCaptured = 0;
				std::chrono::high_resolution_clock::time_point mStartTime;

		};

		Profiler::Timer* GetTimer(const std::string& name);
		void ResetAll();

		class ScopedTimer {

			public:
				ScopedTimer(Timer* t) {
					mTimer = t;
					mTimer->Start();
				}

				~ScopedTimer() {
					mTimer->Stop();
				}

			private:
				Timer* mTimer;

		};

	private:

		Profiler();
		~Profiler();
		
		std::unordered_map<std::string, Timer*> mTimerMap;

};