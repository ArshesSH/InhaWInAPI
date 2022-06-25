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
		const int id = field.size();
		field.emplace_back( PhysicsEntity( PhysicsEntity::Type::Circle, pos, id ) );
	}

	void AddRect( const Vec2<int>& pos )
	{
		const int id = field.size();
		field.emplace_back( PhysicsEntity( PhysicsEntity::Type::Rect, pos, id ) );
	}
	void AddStar( const Vec2<int>& pos )
	{
		const int id = field.size();
		field.emplace_back( PhysicsEntity( PhysicsEntity::Type::Star, pos, id ) );
	}

	void Update(float dt, const RECT& w)
	{
		//for ( auto itEntity = field.begin(); itEntity != field.end(); ++itEntity )
		//{
		//	itEntity->Update( dt, w );

		//	for ( auto itOther = itEntity ; itOther != field.end(); ++itOther )
		//	{
		//		itEntity->DoEntityCollisionWith( *itOther );
		//	}
		//}
		for ( auto& e : field )
		{
			e.Update( dt, w );

			for ( auto& other : field )
			{
				e.DoEntityCollisionWith( other );
			}
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
