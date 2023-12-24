#include "MovingBall.h"

void MovingBall::Init() {

}

void MovingBall::Update([[maybe_unused]] const float deltaTime) {

	// 物理的に動かす
	UpdateRigidbody(deltaTime);


}

void MovingBall::Draw([[maybe_unused]] const Camera &camera) const {

}

void MovingBall::UpdateRigidbody([[maybe_unused]] const float deltaTime) {
	beforePos_ = sphere_.center_;

	velocity_ += acceleration_;
	Vector3 fixVelocity = velocity_ * deltaTime;

	sphere_.center_ += fixVelocity;
}
