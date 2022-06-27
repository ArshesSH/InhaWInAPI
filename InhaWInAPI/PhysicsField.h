#pragma once

#include <vector>
#include "PatternMatchingListener.h"
#include "PhysicsEntity.h"

class PhysicsField
{
public:
	PhysicsField();
	
	void AddCircle( const Vec2<int>& pos );
	void AddRect( const Vec2<int>& pos );
	void AddStar( const Vec2<int>& pos );
	void Update( float dt, const RECT& w, const GameMode& curMode );
	void Draw( HDC hdc ) const;

private:
	std::vector<PhysicsEntity> field;
	PatternMatchingListener typePairSwitch;
};
