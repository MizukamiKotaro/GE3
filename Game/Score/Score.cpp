#include "Score.h"

void Score::Reset() {

	score_ = 0;
	combo_ = 0;
	perfectNum_ = 0;
	greatNum_ = 0;
	goodNum_ = 0;
	missNum_ = 0;
	isFullCom_ = false;
	hiCombo_ = 0;
}

void Score::ComboUpdate() {
	if (hiCombo_ < combo_) {
		hiCombo_ = combo_;
	}
}

void Score::SetUpdate(const Score& score) {
	
	if (score_ < score.score_) {
		score_ = score.score_;
	}

	if (hiCombo_ < score.hiCombo_) {
		hiCombo_ = score.hiCombo_;
	}

	if (!isFullCom_) {
		isFullCom_ = score.isFullCom_;
	}
}

void Score::IsFullComUpdate() {
	if (missNum_ == 0 && goodNum_ == 0) {
		isFullCom_ = true;
	} else {
		isFullCom_ = false;
	}
}

void Score::AddCom() {
	combo_++;
	if (combo_ > hiCombo_) {
		hiCombo_ = combo_;
	}
}