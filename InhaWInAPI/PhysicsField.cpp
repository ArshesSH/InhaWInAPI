#include "PhysicsField.h"

#include "CollisionEffect.h"
#include "framework.h"
#include "GameMode.h"
#include "PhysicsEntityTypeTraits.h"

PhysicsField::PhysicsField()
{
	auto tmp = [](PhysicsEntity& a, PhysicsEntity& b) {};
	typePairSwitch.Case<TypeCircle, TypeCircle>( tmp );
	typePairSwitch.Case<TypeRect, TypeCircle>( tmp );
	typePairSwitch.Case<TypeStar, TypeCircle>( tmp );
	typePairSwitch.Case<TypeRect, TypeRect>( tmp );
	typePairSwitch.Case<TypeRect, TypeStar>( tmp );
	typePairSwitch.Case<TypeStar, TypeStar>( tmp );
}
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
			e.DoEntityCollisionWith( other, curMode, typePairSwitch );
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
