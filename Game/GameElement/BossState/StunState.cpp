#include "StunState.h"
#include "../Boss.h"
#include "../Player.h"
#include "EscapeState.h"

StunState::StunState()
{
}

void StunState::Init()
{
	stateTimer_.Start(3.f);
}

void StunState::Update(const float deltaTime)
{
	stateTimer_.Update(deltaTime);
	invincibleTime_.Update(deltaTime);

	if (stateTimer_.IsFinish()) {
		ChangeState();
	}
}

void StunState::OnCollision(IEntity *other)
{
	Player *player = dynamic_cast<Player *>(other);
	if (player) {
		Vector3 toPlayer = Vector3{ player->GetSphere().center_ - GetBoss()->GetTransform().translate_ }.Normalize();

		if (invincibleTime_.IsFinish()) {
			// 無敵時間を開始
			invincibleTime_.Start(0.5f);

			player->AddAcceleration(toPlayer * 2.f);
			GetBoss()->Damage(0.5f);
		}

	}
}

void StunState::ChangeState()
{
	GetBoss()->ChangeState<EscapeState>();
}
