#include "FrameInfo.h"

FrameInfo* FrameInfo::GetInstance()
{
	static FrameInfo instance;
	return &instance;
}

void FrameInfo::Init()
{
	frameStartTime_ = std::chrono::steady_clock::now();
}

void FrameInfo::End()
{
	std::chrono::steady_clock::time_point frameEndTime = std::chrono::steady_clock::now();

	std::chrono::microseconds frameTime =
		std::chrono::duration_cast<std::chrono::microseconds>(frameEndTime - frameStartTime_);

	deltaTime_ = static_cast<float>(static_cast<double>(frameTime.count()) * 0.000001);

	frameStartTime_ = std::chrono::steady_clock::now();
}
