#include "MovingBall.h"
#include "BlockManager.h"
#include "BoundPin.h"
#include "MapChip.h"
#include "Player.h"
#include <ModelDataManager.h>
#include <SoUtils/SoLib/SoLib_Easing.h>
#include "calc.h"

MapChip *MovingBall::mapChip_ = nullptr;

void MovingBall::Init()
{

	model_ = ModelDataManager::GetInstance()->LoadObj("Sphere");
	sphere_.Initialize(Vector3::zero, 0.3f);
	modelScale_ = 1.f;

    aliveTime_.Start();

    color_ = 0x55FF55FF;
    SetTeam(Team::kPlayer);

    velocity_ = {};
    acceleration_ = {};

    beforePos_ = sphere_.center_;
}

void MovingBall::Update([[maybe_unused]] const float deltaTime)
{

    aliveTime_.Update(deltaTime);
    // 物理的に動かす
    UpdateRigidbody(deltaTime);

    acceleration_.y -= 9.8f * deltaTime * 2.f;

    sphere_.center_ = mapChip_->HitMap(beforePos_, sphere_.center_, 1.f);

    for (uint8_t i = 0u; i < 3u; i++) {
        if ((&beforePos_.x)[i] == (&sphere_.center_.x)[i]) {
            (&velocity_.x)[i] *= -0.9f;
        }
    }

    transformMat_ = Matrix4x4::MakeAffinMatrix(sphere_.scale_ * (sphere_.radius_ * modelScale_), sphere_.rotate_, sphere_.center_);

    color_.a = 1.f - SoLib::easeInCirc(aliveTime_.GetProgress());

    if (aliveTime_.IsFinish()) {
        isAlive_ = false;
    }
}

void MovingBall::Draw()
{
    static auto *const blockManager = BlockManager::GetInstance();

    blockManager->AddBox(model_, IBlock{.transformMat_ = transformMat_, .color_ = color_});
}

void MovingBall::AddAcceleration(const Vector3 &vec)
{
    velocity_ = vec;
}

void MovingBall::SetMapChip(MapChip *mapChip)
{
    mapChip_ = mapChip;
}

void MovingBall::SetTeam(const Team team)
{
    team_ = team;

    switch (team) {
    case Team::kPlayer:
        color_ = 0x55FF55FF;
        break;

    case Team::kEnemy:
        color_ = 0xFF55FFFF;

        break;

    default:
        break;
    }
}

void MovingBall::OnCollision(IEntity *other)
{

    // 接触対象がプレイヤなら実行
    Player *player = dynamic_cast<Player *>(other);
    if (player) {

        // 自分がプレイヤ属性を持っていない場合
        if (this->team_ != Team::kPlayer) {
            this->isAlive_ = false;
        }
    }
    // 接触対象がピンなら実行
    BoundPin *pin = dynamic_cast<BoundPin *>(other);
    if (pin) {
        ReflectPin(*pin);
    }
}

void MovingBall::ReflectPin(const BoundPin &pin)
{

    const Vector3 normalVec = (this->sphere_.center_ - pin.GetPos());

    // 法線の向きがvelocityと逆を向いていた場合は反射しない
    if (Calc::Dot(velocity_, normalVec) < 0.f) {
        velocity_ = velocity_.Reflect(normalVec.Normalize());
    }
}

void MovingBall::UpdateRigidbody([[maybe_unused]] const float deltaTime)
{
    beforePos_ = sphere_.center_;

    velocity_ += acceleration_;
    Vector3 fixVelocity = velocity_ * deltaTime;
    acceleration_ = {};

    sphere_.center_ += fixVelocity;
}
