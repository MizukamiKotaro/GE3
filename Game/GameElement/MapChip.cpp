#include "MapChip.h"
#include <ModelDataManager.h>

void MapChip::Init() {
	pBlockManager_ = BlockManager::GetInstance();
	model_ = ModelDataManager::GetInstance()->LoadObj("Cube");
}

void MapChip::Load(const SoLib::IO::CSV &csv) {

	mapData_.Resize(csv.GetHeight(), csv.GetWidth());
	auto mapItr = mapData_.begin();
	for (auto lineItr = csv.Get().rbegin(); lineItr != csv.Get().rend(); ++lineItr) {
		for (auto &item : *lineItr) {
			if (item == "") { *mapItr = 0u; }
			else { *mapItr = static_cast<uint32_t>(std::stoul(item)); }
			++mapItr;
		}
	}

	Transfer();
}

void MapChip::Transfer() {
	TransferBox();
}

void MapChip::Update([[maybe_unused]] const float deltaTime) {
}

void MapChip::Draw() {
	//const Vector3 beginPos = Vector3{ -static_cast<float>(mapData_.GetCols()) + 1.f,-static_cast<float>(mapData_.GetRows()) + 1.f, 0.f } *(0.5f * kChipScale_);

	//TransferBox();

	for (auto &box : boxList_) {
		IBlock block = IBlock{ .transformMat_ = box.TransMat(),.color_ = 0xFFFFFFFF };
		pBlockManager_->AddBox(model_, std::move(block));
	}
}

void MapChip::TransferBox() {
	const Vector3 beginPos = Vector3{ -static_cast<float>(mapData_.GetCols()) + 1.f,-static_cast<float>(mapData_.GetRows()) + 1.f, 0.f } *(0.5f * kChipScale_);
	Vector3 offset{};
	boxList_.clear();
	const Vector3 boxRadius = Vector3::one * kChipScale_ * kBoxScale;

	for (auto &line : mapData_.get()) {
		offset.x = 0.f;
		for (auto item : line) {
			if (item) {
				boxList_.push_back(AABB::Create(beginPos + offset, boxRadius));
			}
			offset.x += kChipScale_;
		}
		offset.y += kChipScale_;
	}
}


Vector3 MapChip::HitMap(const Vector3 &beforePos, const Vector3 &afterPos, float) const {
	//const Vector3 beginPos = Vector3{ -static_cast<float>(mapData_.GetCols()) + 1.f,-static_cast<float>(mapData_.GetRows()) + 1.f, 0.f } *(0.5f * kChipScale_);
	const Vector3 offset = Vector3{ mapData_.GetCols() / 2.f, mapData_.GetRows() / 2.f,0.f } *kChipScale_;

	const uint32_t kChipSize = 32u;

	Vector3 resultPos = ((afterPos + offset) * kChipSize) / kChipScale_;

	Vector2 leftTop;
	Vector2 rightTop;

	Vector2 leftDown;
	Vector2 rightDown;

	leftTop.x = (resultPos.x - kChipSize / 2.f) / kChipSize;
	leftTop.y = (resultPos.y + kChipSize / 2.f - 1) / kChipSize;
	rightTop.x = (resultPos.x + kChipSize / 2.f - 1) / kChipSize;
	rightTop.y = (resultPos.y + kChipSize / 2.f - 1) / kChipSize;
	leftDown.x = (resultPos.x - kChipSize / 2.f) / kChipSize;
	leftDown.y = (resultPos.y - kChipSize / 2.f) / kChipSize;
	rightDown.x = (resultPos.x + kChipSize / 2.f - 1) / kChipSize;
	rightDown.y = (resultPos.y - kChipSize / 2.f) / kChipSize;

	if (
		(leftDown.x < 0.f || rightDown.x >= mapData_.GetCols())		// 左右が画面外
		|| (leftTop.y >= mapData_.GetRows() || leftDown.y < 0.f)	// 上下が画面外
		) {
		return afterPos;
	}

	//leftTop /= 2.f;
	//rightTop /= 2.f;
	//leftDown /= 2.f;
	//rightDown /= 2.f;

	const float kExtension = 7.f;

	///
	///	↓ マップチップとの当たり判定
#pragma region MapChipHit
	if (afterPos.x - beforePos.x < 0 && afterPos.y - beforePos.y < 0) { //	SA
		//				Novice::ScreenPrintf(10, 90, "SA");
		if ((mapData_.at((int)(rightDown.y), (int)(rightDown.x)) != 0u) && (mapData_.at((int)(leftTop.y), (int)(leftTop.x)) != 0u)) { //	右元と左頭が移動不可の場合
			resultPos.x = (int)(leftTop.x + 1) * kChipSize + 16.f;
			resultPos.y = (int)(rightDown.y + 1) * kChipSize + 16.f;

			// player.box.velocity.x *= 0.8;
			// player.box.velocity.y = 0;
			// player.isGround = true;
		}
		else if (mapData_.at((int)(rightDown.y), (int)(rightDown.x)) != 0u) { //	右元が移動不可の場合

			if (mapData_.at((int)(rightDown.y) + 1, (int)(rightDown.x)) == 0u) {	//	尚且つ右元の上マスが移動可能の場合
				resultPos.y = (int)(rightDown.y + 1) * kChipSize + 16.f;

				// player.box.velocity.y = 0;
				// player.isGround = true;
			}
		}
		else if (mapData_.at((int)(leftTop.y), (int)(leftTop.x)) != 0u) { //	左頭が移動不可の場合

			if (mapData_.at((int)(leftTop.y), (int)(leftTop.x) + 1) == 0u) {  //	尚且つ左頭の右マスが開いている場合。
				resultPos.x = (int)(leftTop.x + 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
			}
		}


		else if (mapData_.at((int)(leftDown.y), (int)(leftDown.x)) != 0u) { //	左元が移動不可の場合


			//		尚且つ右と上が開いている場合
			if (mapData_.at((int)(leftDown.y), (int)(leftDown.x) + 1) == 0u && mapData_.at((int)(leftDown.y) + 1, (int)(leftDown.x)) == 0u) {

				//if (mapData_.at((int)(leftTop.y),(int)(leftTop.x - 1)) == ChipState::air) {
				if ((((int)(resultPos.x + 15.f) % kChipSize) < ((int)(resultPos.y + 15.f) % kChipSize))) {
					resultPos.y = (int)(leftDown.y + 1) * kChipSize + 16.f;

					// player.box.velocity.y = 0;
					// player.isGround = true;
				}
				//	}
				//	else if(mapData_.at((int)(leftTop.y - 1),(int)(leftTop.x)) == ChipState::air) {
				else {
					resultPos.x = (int)(leftDown.x + 1) * kChipSize + 16.f;

					// player.box.velocity.x *= 0.8;
				}
				//	}
			}
			//		尚且つ左元の上マスが開いていて、右マスが移動不可の場合。
			else if (mapData_.at((int)(leftDown.y) + 1, (int)(leftDown.x)) == 0u && mapData_.at((int)(leftDown.y), (int)(leftDown.x) + 1) != 0u) {
				resultPos.y = (int)(leftDown.y + 1) * kChipSize + 16.f;

				// player.box.velocity.y = 0;
				// player.isGround = true;
			}
			//		尚且つ左元の右マスが開いていて、上マスが移動不可の場合。
			else if (mapData_.at((int)(leftDown.y), (int)(leftDown.x) + 1) == 0u && mapData_.at((int)(leftDown.y) + 1, (int)(leftDown.x)) != 0u) {
				resultPos.x = (int)(leftDown.x + 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
			}
			//		尚且つ左元の右マスと上マスが移動不可の場合。
			else if (mapData_.at((int)(leftDown.y), (int)(leftDown.x) + 1) != 0u && mapData_.at((int)(leftDown.y) + 1, (int)(leftDown.x)) != 0u) {
				resultPos.x = (int)(leftDown.x + 1) * kChipSize + 16.f;
				resultPos.y = (int)(leftDown.y + 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
				// player.box.velocity.y = 0;
				// player.isGround = true;
			}
		}
	}


	else if (afterPos.x - beforePos.x < 0 && afterPos.y - beforePos.y > 0) { //	WA
		//				Novice::ScreenPrintf(10, 90, "WA");
		if ((mapData_.at((int)(rightTop.y), (int)(rightTop.x)) != 0u) && (mapData_.at((int)(leftDown.y), (int)(leftDown.x)) != 0u)) { //	右頭と左元が移動不可の場合
			resultPos.x = (int)(leftDown.x + 1) * kChipSize + 16.f;
			resultPos.y = (int)(rightTop.y - 1) * kChipSize + 16.f;

			// player.box.velocity.x *= 0.8;
			// player.box.velocity.y = 0;
		}
		else if (mapData_.at((int)(rightTop.y), (int)(rightTop.x)) != 0u) { //	右頭が移動不可の場合

			if (mapData_.at((int)(rightTop.y), (int)(rightTop.x - (kExtension / kChipSize))) != 0u) { //	右頭の3ピクセルが移動不可の場合
				if (mapData_.at((int)(rightTop.y) - 1, (int)(rightTop.x)) == 0u) {	//	尚且つ右頭の下マスが移動可能の場合
					resultPos.y = (int)(rightTop.y - 1) * kChipSize + 16.f;

					// player.box.velocity.y = 0;
				}
			}
			else {
				resultPos.x = (int)(rightTop.x - 1) * kChipSize + 16.f;
			}
		}
		else if (mapData_.at((int)(leftDown.y), (int)(leftDown.x)) != 0u) { //	左元が移動不可の場合

			if (mapData_.at((int)(leftDown.y), (int)(leftDown.x) + 1) == 0u) {  //	尚且つ左元の右マスが開いている場合。
				resultPos.x = (int)(leftDown.x + 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
			}
		}


		else if (mapData_.at((int)(leftTop.y), (int)(leftTop.x)) != 0u) { //	左頭が移動不可の場合


			//		尚且つ右と下が開いている場合
			if (mapData_.at((int)(leftTop.y), (int)(leftTop.x) + 1) == 0u && mapData_.at((int)(leftTop.y) - 1, (int)(leftTop.x)) == 0u) {

				//if (mapData_.at((int)(leftTop.y),(int)(leftTop.x - 1)) == ChipState::air) {
				if (((int)(resultPos.x + 15.f) % kChipSize) <= ((kChipSize)-((int)(resultPos.y + 17.f) % kChipSize)) % kChipSize) {
					resultPos.y = (int)(leftTop.y - 1) * kChipSize + 16.f;

					// player.box.velocity.y = 0;
				}
				//	}
				//	else if(mapData_.at((int)(leftTop.y - 1),(int)(leftTop.x)) == ChipState::air) {
				else {
					resultPos.x = (int)(leftTop.x + 1) * kChipSize + 16.f;

					// player.box.velocity.x *= 0.8;
				}
				//	}
			}
			//		尚且つ左頭の下マスが開いていて、右マスが移動不可の場合。
			else if (mapData_.at((int)(leftTop.y) - 1, (int)(leftTop.x)) == 0u && mapData_.at((int)(leftTop.y), (int)(leftTop.x) + 1) != 0u) {
				resultPos.y = (int)(leftTop.y - 1) * kChipSize + 16.f;

				// player.box.velocity.y = 0;
			}
			//		尚且つ左頭の右マスが開いていて、下マスが移動不可の場合。
			else if (mapData_.at((int)(leftTop.y), (int)(leftTop.x) + 1) == 0u && mapData_.at((int)(leftTop.y) - 1, (int)(leftTop.x)) != 0u) {
				resultPos.x = (int)(leftTop.x + 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
			}
			//		尚且つ左頭の右マスと下マスが移動不可の場合。
			else if (mapData_.at((int)(leftTop.y), (int)(leftTop.x) + 1) != 0u && mapData_.at((int)(leftTop.y) - 1, (int)(leftTop.x)) != 0u) {
				resultPos.x = (int)(leftTop.x + 1) * kChipSize + 16.f;
				resultPos.y = (int)(leftTop.y - 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
				// player.box.velocity.y = 0;
			}
		}
	}

	else if (afterPos.x - beforePos.x > 0 && afterPos.y - beforePos.y < 0) { //	SD
		//				Novice::ScreenPrintf(10, 90, "SD");
		if ((mapData_.at((int)(leftDown.y), (int)(leftDown.x)) != 0u) && (mapData_.at((int)(rightTop.y), (int)(rightTop.x)) != 0u)) { //	左元と右頭が移動不可の場合
			resultPos.x = (int)(rightTop.x - 1) * kChipSize + 16.f;
			resultPos.y = (int)(leftDown.y + 1) * kChipSize + 16.f;

			// player.box.velocity.x *= 0.8;
			// player.box.velocity.y = 0;
			// player.isGround = true;
		}
		else if (mapData_.at((int)(leftDown.y), (int)(leftDown.x)) != 0u) { //	左元が移動不可の場合

			if (mapData_.at((int)(leftDown.y) + 1, (int)(leftDown.x)) == 0u) {	//	尚且つ左元の上マスが移動可能の場合
				resultPos.y = (int)(leftDown.y + 1) * kChipSize + 16.f;

				// player.box.velocity.y = 0;
				// player.isGround = true;
			}
		}
		else if (mapData_.at((int)(rightTop.y), (int)(rightTop.x)) != 0u) { //	右頭が移動不可の場合

			if (mapData_.at((int)(rightTop.y), (int)(rightTop.x) - 1) == 0u) {  //	尚且つ右頭の左マスが開いている場合。
				resultPos.x = (int)(rightTop.x - 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
			}
		}


		else if (mapData_.at((int)(rightDown.y), (int)(rightDown.x)) != 0u) { //	右元が移動不可の場合


			//		尚且つ左と上が開いている場合
			if (mapData_.at((int)(rightDown.y), (int)(rightDown.x) - 1) == 0u && mapData_.at((int)(rightDown.y) + 1, (int)(rightDown.x)) == 0u) {

				//if (mapData_.at((int)(rightTop.y),(int)(rightTop.x - 1)) == ChipState::air) {
				if (((kChipSize)-((int)(resultPos.x + 17.f) % kChipSize)) % kChipSize < ((int)(resultPos.y + 15.f) % kChipSize)) {
					resultPos.y = (int)(rightDown.y + 1) * kChipSize + 16.f;

					// player.box.velocity.y = 0;
					// player.isGround = true;
				}
				//	}
				//	else if(mapData_.at((int)(rightTop.y - 1),(int)(rightTop.x)) == ChipState::air) {
				else {
					resultPos.x = (int)(rightDown.x - 1) * kChipSize + 16.f;
				}
				//	}
			}
			//		尚且つ右元の上マスが開いていて、左マスが移動不可の場合。
			else if (mapData_.at((int)(rightDown.y) + 1, (int)(rightDown.x)) == 0u && mapData_.at((int)(rightDown.y), (int)(rightDown.x) - 1) != 0u) {
				resultPos.y = (int)(rightDown.y + 1) * kChipSize + 16.f;

				// player.box.velocity.y = 0;
				// player.isGround = true;
			}
			//		尚且つ右元の左マスが開いていて、上マスが移動不可の場合。
			else if (mapData_.at((int)(rightDown.y), (int)(rightDown.x) - 1) == 0u && mapData_.at((int)(rightDown.y) + 1, (int)(rightDown.x)) != 0u) {
				resultPos.x = (int)(rightDown.x - 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
			}
			//		尚且つ右元の左マスと上マスが移動不可の場合。
			else if (mapData_.at((int)(rightDown.y), (int)(rightDown.x) - 1) != 0u && mapData_.at((int)(rightDown.y) + 1, (int)(rightDown.x)) != 0u) {
				resultPos.x = (int)(rightDown.x - 1) * kChipSize + 16.f;
				resultPos.y = (int)(rightDown.y + 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
				// player.box.velocity.y = 0;
				// player.isGround = true;
			}
		}
	}


	else if (afterPos.x - beforePos.x > 0 && afterPos.y - beforePos.y > 0) { //	WD
		//				Novice::ScreenPrintf(10, 90, "WD");
		if ((mapData_.at((int)(leftTop.y), (int)(leftTop.x)) != 0u) && (mapData_.at((int)(rightDown.y), (int)(rightDown.x)) != 0u)) { //	左頭と右元が移動不可の場合
			resultPos.x = (int)(rightDown.x - 1) * kChipSize + 16.f;
			resultPos.y = (int)(leftTop.y - 1) * kChipSize + 16.f;

			// player.box.velocity.x *= 0.8;
			// player.box.velocity.y = 0;
		}
		else if (mapData_.at((int)(leftTop.y), (int)(leftTop.x)) != 0u) { //	左頭が移動不可の場合

			if (mapData_.at((int)(leftTop.y), (int)(leftTop.x + (kExtension / kChipSize))) != 0u) { //	左頭の3ピクセルが移動不可の場合
				if (mapData_.at((int)(leftTop.y) - 1, (int)(leftTop.x)) == 0u) {	//	尚且つ左頭の下マスが移動可能の場合
					resultPos.y = (int)(leftTop.y - 1) * kChipSize + 16.f;

					// player.box.velocity.y = 0;
				}
			}
			else {
				resultPos.x = (int)(leftTop.x + 1) * kChipSize + 16.f;
			}
		}
		else if (mapData_.at((int)(rightDown.y), (int)(rightDown.x)) != 0u) { //	右元が移動不可の場合

			if (mapData_.at((int)(rightDown.y), (int)(rightDown.x) - 1) == 0u) {  //	尚且つ右元の左マスが開いている場合。
				resultPos.x = (int)(rightDown.x - 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
			}
		}


		else if (mapData_.at((int)(rightTop.y), (int)(rightTop.x)) != 0u) { //	右頭が移動不可の場合


			//		尚且つ左と下が開いている場合
			if (mapData_.at((int)(rightTop.y), (int)(rightTop.x) - 1) == 0u && mapData_.at((int)(rightTop.y) - 1, (int)(rightTop.x)) == 0u) {

				//if (mapData_.at((int)(rightTop.y),(int)(rightTop.x - 1)) == ChipState::air) {
				if (((kChipSize)-((int)(resultPos.x + 17.f) % kChipSize)) % kChipSize <= ((kChipSize)-((int)(resultPos.y + 17.f) % kChipSize)) % kChipSize) {
					resultPos.y = (int)(rightTop.y - 1) * kChipSize + 16.f;

					// player.box.velocity.y = 0;
				}
				//	}
				//	else if(mapData_.at((int)(rightTop.y - 1),(int)(rightTop.x)) == ChipState::air) {
				else {
					resultPos.x = (int)(rightTop.x - 1) * kChipSize + 16.f;

					// player.box.velocity.x *= 0.8;
				}
				//	}
			}
			//		尚且つ右頭の下マスが開いていて、左マスが移動不可の場合。
			else if (mapData_.at((int)(rightTop.y) - 1, (int)(rightTop.x)) == 0u && mapData_.at((int)(rightTop.y), (int)(rightTop.x) - 1) != 0u) {
				resultPos.y = (int)(rightTop.y - 1) * kChipSize + 16.f;

				// player.box.velocity.y = 0;
			}
			//		尚且つ右頭の左マスが開いていて、下マスが移動不可の場合。
			else if (mapData_.at((int)(rightTop.y), (int)(rightTop.x) - 1) == 0u && mapData_.at((int)(rightTop.y) - 1, (int)(rightTop.x)) != 0u) {
				resultPos.x = (int)(rightTop.x - 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
			}
			//		尚且つ右頭の左マスと下マスが移動不可の場合。
			else if (mapData_.at((int)(rightTop.y), (int)(rightTop.x) - 1) != 0u && mapData_.at((int)(rightTop.y) - 1, (int)(rightTop.x)) != 0u) {
				resultPos.x = (int)(rightTop.x - 1) * kChipSize + 16.f;
				resultPos.y = (int)(rightTop.y - 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
				// player.box.velocity.y = 0;
			}
		}
	}

	else if (afterPos.x - beforePos.x > 0) {
		if ((mapData_.at((int)(rightTop.y), (int)(rightTop.x)) != 0u) || (mapData_.at((int)(rightDown.y), (int)(rightDown.x)) != 0u)) {
			resultPos.x = (int)(rightTop.x - 1) * kChipSize + 16.f;
		}
	}

	else if (afterPos.x - beforePos.x < 0) {
		if ((mapData_.at((int)(leftTop.y), (int)(leftTop.x)) != 0u) || (mapData_.at((int)(leftDown.y), (int)(leftDown.x)) != 0u)) {
			resultPos.x = (int)(leftTop.x + 1) * kChipSize + 16.f;
		}
	}

	else if (afterPos.y - beforePos.y > 0) {
		if ((mapData_.at((int)(leftTop.y), (int)(leftTop.x)) != 0u) && mapData_.at((int)(leftTop.y), (int)(leftTop.x + (kExtension / kChipSize))) == 0u) { //	左頭の3ピクセルが移動可の場合
			resultPos.x = (int)(leftTop.x + 1) * kChipSize + 16.f;
		}
		else if ((mapData_.at((int)(rightTop.y), (int)(rightTop.x)) != 0u) && mapData_.at((int)(rightTop.y), (int)(rightTop.x - (kExtension / kChipSize))) == 0u) { //	左頭の3ピクセルが移動可の場合
			resultPos.x = (int)(rightTop.x - 1) * kChipSize + 16.f;
		}

		else if ((mapData_.at((int)(leftTop.y), (int)(leftTop.x)) != 0u) || (mapData_.at((int)(rightTop.y), (int)(rightTop.x)) != 0u)) {
			resultPos.y = (int)(rightTop.y - 1) * kChipSize + 16.f;

			// player.box.velocity.y = 0;
		}
	}

	else if (afterPos.y - beforePos.y < 0) {
		if ((mapData_.at((int)(leftDown.y), (int)(leftDown.x)) != 0u) || (mapData_.at((int)(rightDown.y), (int)(rightDown.x)) != 0u)) {
			resultPos.y = (int)(leftDown.y + 1) * kChipSize + 16.f;

			// player.box.velocity.y = 0;

			// player.isGround = true;

		}
	}
	else {
		return afterPos;
	}

#pragma endregion
	///	↑ マップチップとの当たり判定
	/// 

	return (resultPos * kChipScale_) / kChipSize - offset;

}

