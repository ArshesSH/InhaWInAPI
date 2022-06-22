#pragma once

#include "framework.h"
#include <vector>
#include <memory>
#include "PhysicsEntity.h"

class PhysicsField
{

public:
	PhysicsField() = default;
	
	void AddCircle()
	{
		field.emplace_back( PhysicsEntity( PhysicsEntity::Type::Circle ) );
	}

	void Update(float dt)
	{
		for ( auto& e : field )
		{
			e.Update(dt);
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
