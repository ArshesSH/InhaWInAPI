#pragma once

#include "FrameTimer.h"
#include "PhysicsField.h"
#include "GameMode.h"
#include "PatternMatchingListener.h"

class Game
{
public:
	void ComposeFrame(const HDC& hdc);
	void UpdateModel();

	float GetDeltaTime() const;
	void SetClientRECT( const RECT& r );
	void SetCollisionMode();
	void SetCombineMode();
	void SetSplitMode();
	void AddCircle( const Vec2<int>& pos );
	void AddRect( const Vec2<int>& pos );
	void AddStar( const Vec2<int>& pos );
private:
	FrameTimer ft;
	GameMode gameMode = GameMode::Collision;
	PhysicsField field;
	HDC hdc = nullptr;
	RECT screenRect;
	float dt = 0.0f;
};