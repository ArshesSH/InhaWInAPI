#pragma once

#include <vector>
#include <string>
#include "PatternMatchingListener.h"
#include "PhysicsEntity.h"

class PhysicsField
{
public:
	PhysicsField();
	
	void AddCircle( const Vec2<int>& pos );
	void AddRect( const Vec2<int>& pos );
	void AddStar( const Vec2<int>& pos );
	void AddRandomly( const Vec2<int>& pos );
	void Update( float dt, const RECT& w, const GameMode& curMode_in );
	void Draw( HDC hdc ) const;
	void Debug( HDC hdc ) const;
	void DestroyEntity();
	void ScaleUpEntity();

private:
	static constexpr float scaleUpLimit = 200.0f;
	std::vector<PhysicsEntity> field;
	PatternMatchingListener typePairSwitch;
	GameMode curMode = GameMode::Collision;
	GameMode lastMode = curMode;
	std::wstring modeOutput;
};
