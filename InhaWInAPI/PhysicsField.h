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

private:
	std::vector<PhysicsEntity> field;
	PatternMatchingListener typePairSwitch;
	GameMode curMode;
	std::wstring modeOutput;
};
