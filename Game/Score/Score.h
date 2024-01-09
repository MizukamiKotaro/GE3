#pragma once


class Score {

public:

	void Reset();

	void ComboUpdate();

	void SetUpdate(const Score& score);

	void AddScore(int score) { score_ += score; }

	void AddPer() {
		perfectNum_++;
		AddCom();
		AddScore(200 * (combo_ / 10 + 1));
	}

	void AddGre() {
		greatNum_++;
		AddCom();
		AddScore(100 * (combo_ / 10 + 1));
	}

	void AddGood() {
		goodNum_++;
		ComboReset();
		AddScore(50);
	}

	void AddMiss() {
		missNum_++;
		ComboReset();
	}

	void AddCom();

	void ComboReset() { combo_ = 0; }

	void IsFullComUpdate();

	int GetScore() { return score_; }

	int GetCom() { return combo_; }

	int GetHiCombo() { return hiCombo_; }

	int GetPer() { return perfectNum_; }

	int GetGre() { return greatNum_; }

	int GetGood() { return goodNum_; }

	int GetMiss() { return missNum_; }

	bool IsFullCom() { return isFullCom_; }

private:

	int score_ = 0;
	int combo_ = 0;
	int perfectNum_ = 0;
	int greatNum_ = 0;
	int goodNum_ = 0;
	int missNum_ = 0;

	int hiCombo_ = 0;
	bool isFullCom_ = false;

};
