#pragma once
#include <cstdint>

class Boss;
class Player;

class IEntity;

class IBossState {
public:

	enum class SlotState {
		kLow,
		kMiddle,
		kHigh,
		kMaxmum
	};

public:
	IBossState() = default;
	virtual ~IBossState() = default;

	virtual void Init() = 0;

	virtual void Update(const float deltaTime) = 0;

	virtual void OnCollision(IEntity *other) = 0;

	virtual bool IsAttacked() const { return false; }

	// virtual void Draw() = 0;

	static void SetBoss(Boss *boss) { if (boss) { pBoss_ = boss; } }
	static void SetPlayer(Player *player) { if (player) { pPlayer_ = player; } }

protected:

	SlotState slotState_;

	static Boss *const GetBoss() { return pBoss_; }
	static Player *const GetPlayer() { return pPlayer_; }

private:

	static Boss *pBoss_;
	static Player *pPlayer_;

};