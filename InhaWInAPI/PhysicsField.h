#pragma once

#include "framework.h"
#include <vector>
#include <memory>
#include "PhysicsEntity.h"

class PhysicsField
{

public:
	PhysicsField() = default;
	
	void AddCircle(const Vec2<int>& pos)
	{
		field.emplace_back( PhysicsEntity( PhysicsEntity::Type::Circle, pos ) );
	}

	void Update(float dt, const RECT& w)
	{
		for ( auto& e : field )
		{
			e.Update(dt, w);
		}
	}

	void Draw(HDC hdc) const
	{
		for ( const auto& e : field )
		{
			e.Draw( hdc );
		}
	}

private:
	std::vector<PhysicsEntity> field;
};
