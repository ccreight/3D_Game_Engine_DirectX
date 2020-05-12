#include "stdafx.h"
#include "Profiler.h"
#include <chrono>
#include <typeinfo>
#include <stdio.h>

static FILE* mJsonFile;
static bool firstJSONEntry = true;
std::string entireJSONEntry = "";

Profiler::Profiler() {

	char filename[] = "profile.json";
	fopen_s(&mJsonFile, filename, "w+");

	if (mJsonFile != nullptr) {
		fprintf(mJsonFile, "[");
	}

}

void Profiler::Timer::Start() {

	if (!firstJSONEntry) {
		entireJSONEntry += ",\n";
	}
	else {
		firstJSONEntry = false;
	}

	mStartTime = std::chrono::high_resolution_clock::now();

	if (mJsonFile != nullptr) {
		entireJSONEntry += "{\"name\": \"" + (std::string)mPurpose.c_str() + "\", \"ph\": \"B\", \"ts\": " + std::to_string((int)mStartTime.time_since_epoch().count() / 1000) + ", \"pid\": 2343, \"tid\": 2347},";
	}

}

Profiler* Profiler::Get(){

	static Profiler mprof; 
	return &mprof; 

};

Profiler::~Profiler() {

	FILE* fptr;
	char filename[] = "profile.txt";
	fopen_s(&fptr, filename, "w+");
	
	if (fptr != nullptr) {

		for (auto it = mTimerMap.begin(); it != mTimerMap.end(); it++) {

			fprintf(fptr, "%s:, ", it->second->GetName().c_str());
			fprintf(fptr, "%f, ", it->second->GetAvg_ms());
			fprintf(fptr, "%f", it->second->GetMax_ms());
			fprintf(fptr, "\n");

		}

		fclose(fptr);

	}

	fprintf(mJsonFile, entireJSONEntry.c_str());
	entireJSONEntry = "";
	fprintf(mJsonFile, "]");
	fclose(mJsonFile);
	
}

void Profiler::Timer::Stop() {

	std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

	if (mJsonFile != nullptr) {
		entireJSONEntry += "{\"ph\": \"E\", \"ts\": " + std::to_string((int)endTime.time_since_epoch().count() / 1000) + ", \"pid\": 2343, \"tid\": 2347}";
		}

	mLastTime = (double)std::chrono::duration_cast<std::chrono::nanoseconds>((endTime - mStartTime)).count() / 1000000;
	mTotalTime += mLastTime;
	
}

void Profiler::Timer::Reset() {

	mFramesCaptured++;
	mLongestTime = max(mLongestTime, mTotalTime);
	mLastTime = 0;

}

const std::string& Profiler::Timer::GetName() const {
	return mPurpose;
}

double Profiler::Timer::GetTime_ms() const {
	return mLastTime;
}

double Profiler::Timer::GetMax_ms() const {
	return mLongestTime;
}

double Profiler::Timer::GetAvg_ms() const {
	return mTotalTime / mFramesCaptured;
}

Profiler::Timer* Profiler::GetTimer(const std::string& name) {

	if (mTimerMap.find(name) != mTimerMap.end()) {
		return mTimerMap.at(name);
	}

	Profiler::Timer* t = new Profiler::Timer();
	mTimerMap[name] = t;
	t->SetName(name);
	return t;

}

void Profiler::ResetAll() {

	for (auto it = mTimerMap.begin(); it != mTimerMap.end(); it++) {
		it->second->Reset();
	}

}