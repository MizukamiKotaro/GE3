#include "BallSpawner.h"

BallSpawner::BallSpawner() {
}

void BallSpawner::Init() {

}

void BallSpawner::Update(const float deltaTime) {

	fireSpanTimer_.Update(deltaTime);

	if (fireSpanTimer_.IsFinish()) {
		fireSpanTimer_.Start(fireSpan_);

		Fire();
	}

}

void BallSpawner::Draw() {

}

void BallSpawner::Fire() {
	

}
