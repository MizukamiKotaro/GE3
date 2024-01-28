#include "IBossState.h"
Boss *IBossState::pBoss_ = nullptr;
Player *IBossState::pPlayer_ = nullptr;
IBossState::FaceState IBossState::faceState_ = IBossState::FaceState::kNormal;

void IBossState::SetFaceType(const FaceType face)
{
	faceState_ = static_cast<IBossState::FaceState>(face);
}
