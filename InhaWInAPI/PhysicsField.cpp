#include "PhysicsField.h"

#include "framework.h"
#include "GameMode.h"
void PhysicsField::AddCircle( const Vec2<int>& pos )
{
	const int id = (int)field.size();
	field.emplace_back( PhysicsEntity( PhysicsEntity::Type::Circle, pos, id ) );
}

void PhysicsField::AddRect( const Vec2<int>& pos )
{
	const int id = (int)field.size();
	field.emplace_back( PhysicsEntity( PhysicsEntity::Type::Rect, pos, id ) );
}

void PhysicsField::AddStar( const Vec2<int>& pos )
{
	const int id = (int)field.size();
	field.emplace_back( PhysicsEntity( PhysicsEntity::Type::Star, pos, id ) );
}

void PhysicsField::Update( float dt, const RECT& w, const GameMode& curMode )
{
	for ( auto& e : field )
	{
		e.Update( dt, w );

		for ( auto& other : field )
		{
			e.DoEntityCollisionWith( other, curMode );
		}
	}

}

void PhysicsField::Draw( HDC hdc ) const
{
	for ( const auto& e : field )
	{
		e.Draw( hdc );
	}
}
