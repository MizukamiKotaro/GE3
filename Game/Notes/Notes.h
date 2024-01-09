#pragma once
#include "Sprite.h"
#include <memory>
#include <list>
#include <sstream>

class Camera;

enum NotesType {
	LEFT_ARROW,
	RIGHT_ARROW,
	UP_ARROW,
	DOWN_ARROW
};

class Notes {
public:
	
	Notes(float frame, int type);

	void Update(float time);

	void Draw(Camera* camera);

	float GetTime() { return time_; }

	int GetType() { return type_; }

	void Dead() { isDead_ = true; }

	bool IsDead() { return isDead_; }

	static void SetSpeed(float index);

private:

	static const float kSpeed_;

	static float speed_;

private:

	// モデル
	std::unique_ptr<Sprite> sprite_;

	Vector2 firstPos_;

	float time_ = 0;

	int type_ = 0;

	bool isDead_ = false;

};

class NotesList {
public:

	static NotesList* GetInstance();

	void Init();

	void Draw(Camera* camera);

	void PopCommands();

	std::list<std::unique_ptr<Notes>>& GetNotesList() { return notesList_; }

private:

	NotesList() = default;
	~NotesList() = default;
	NotesList(const NotesList&) = delete;
	NotesList& operator=(const NotesList&) = delete;

	void LoadPopData();

private:

	std::list<std::unique_ptr<Notes>> notesList_;

	std::stringstream notesPopCommands_;

	std::unique_ptr<Sprite> lane_;
	std::unique_ptr<Sprite> indication_;

};