#pragma once
#include <vector>
#include "SoUtils/SoLib/SoLib_Timer.h"

template<typename T>
class StateParametor {
public:

	struct Parametor {
		float goalTime_ = 0.f;
		void(T:: *initFunc_)();
		void(T:: *updateFunc_)();
	};

public:

	void Update(const float deltaTime);

	void AddParametor(Parametor &&parametor);

private:
	std::vector<Parametor> stateArray_;
	SoLib::DeltaTimer stateTimer_;
	uint32_t stateIndex_ = 0u;

};

template<typename T>
inline void StateParametor<T>::Update(const float deltaTime) {
	stateTimer_.Update(deltaTime);

	if (stateTimer_.IsActive()) {

		// 関数が指定されているなら実行
		if (stateArray_[stateIndex_].updateFunc_) {
			(this->*stateArray_[stateIndex_].updateFunc_)();
		}
	}

	if (stateTimer_.IsFinish()) {

		++stateIndex_;
		// インデックスが範囲内なら再度実行
		if (stateIndex_ < stateArray_.size()) {

			stateTimer_.Start(stateArray_[stateIndex_].goalTime_);

			// 関数が指定されているなら実行
			if (stateArray_[stateIndex_].initFunc_) {
				(this->*stateArray_[stateIndex_].initFunc_)();
			}
		}
	}
}

template<typename T>
inline void StateParametor<T>::AddParametor(Parametor &&parametor) {
	stateArray_.push_back(parametor);
}
