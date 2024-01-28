#pragma once
#include <cstdint>
#include "../Slot/Slot.h"

class Boss;
class Player;

class IEntity;

class IBossState {
public:

	enum class FaceState : int32_t {
		kSad, // 青
		kOko, // 赤
		kNormal, // 緑
		kGekiOko, // 今は亡き新たな顔
		kBad, // はずれ。プレイヤーにとってチャンス
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

	static void SetFaceType(const FaceType face);

protected:

	static Boss *const GetBoss() { return pBoss_; }
	static Player *const GetPlayer() { return pPlayer_; }

private:
	static FaceState faceState_;

	static Boss *pBoss_;
	static Player *pPlayer_;

};
