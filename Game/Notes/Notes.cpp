#include "Notes.h"
#include "Camera.h"
#include <fstream>
#include "SceneSystem/IScene/IScene.h"
#include "Ease/Ease.h"

const float Notes::kSpeed_ = 200.0f;

float Notes::speed_ = 200.0f;

Notes::Notes(float frame, int type)
{
	time_ = frame;
	type_ = type;

	switch (type_)
	{
	case LEFT_ARROW:
		sprite_ = std::make_unique<Sprite>("Resources/leftArrow.png");
		break;
	case RIGHT_ARROW:
		sprite_ = std::make_unique<Sprite>("Resources/rightArrow.png");
		break;
	case UP_ARROW:
		sprite_ = std::make_unique<Sprite>("Resources/upArrow.png");
		break;
	case DOWN_ARROW:
		sprite_ = std::make_unique<Sprite>("Resources/downArrow.png");
		break;
	default:
		sprite_ = std::make_unique<Sprite>("Resources/leftArrow.png");
		break;
	}

	firstPos_ = { 1000.0f,-speed_ * time_ + 500.0f };

	sprite_->pos_ = firstPos_;
	sprite_->Update();
}

void Notes::Update(float time)
{
	if (!isDead_) {
		sprite_->pos_.y = Ease::UseEase(firstPos_.y, 500.0f, time, time_, Ease::Constant);
		sprite_->Update();
	}
}

void Notes::Draw(Camera* camera)
{
	if (!isDead_) {
		sprite_->Draw(*camera);
	}
}

void Notes::SetSpeed(float index)
{

}

NotesList* NotesList::GetInstance()
{
	static NotesList instance;
	return &instance;
}

void NotesList::Init()
{
	lane_ = std::make_unique<Sprite>("Resources/white.png");
	lane_->size_ = { 100.0f,720.0f };
	lane_->pos_ = { 1000.0f,360.0f };
	lane_->SetColor({ 0.1f,0.1f,0.1f,0.5f });
	lane_->Update();

	indication_= std::make_unique<Sprite>("Resources/white.png");
	indication_->size_ = { 100.0f,8.0f };
	indication_->pos_ = { 1000.0f,500.0f };
	indication_->SetColor({ 0.9f,0.9f,0.9f,0.9f });
	indication_->Update();
}

void NotesList::Draw(Camera* camera)
{
	lane_->Draw(*camera);
	indication_->Draw(*camera);

	for (std::unique_ptr<Notes>& notes : notesList_) {
		notes->Draw(camera);
	}
}

void NotesList::PopCommands()
{
	notesList_.clear();

	notesPopCommands_.clear(std::stringstream::goodbit);
	LoadPopData();

	// 曲のノーツの配置。曲の始めに毎回。

	// 1列分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(notesPopCommands_, line)) {
		// 1列分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行は飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {

			// modelType
			getline(line_stream, word, ',');
			int type = (int)std::atoi(word.c_str());

			// frame
			getline(line_stream, word, ',');
			float time = (float)std::atof(word.c_str());

			notesList_.push_back(std::make_unique<Notes>(time, type));

		}
		// WAITコマンド
		else if (word.find("END") == 0) {
			
			break;
		}
	}
}

void NotesList::LoadPopData()
{// ファイルを開く
	std::ifstream file;

	switch (IScene::stagrNo_)
	{
	case SHINING_STAR:
		file.open("./Resources/sakurasakura.csv");
		break;
	default:
		file.open("./Resources/sakurasakura.csv");
		break;
	}

	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	notesPopCommands_ << file.rdbuf();

	// ファイルを閉じる
	file.close();
}
