#include "PhysicsField.h"

#include "CollisionEffect.h"
#include "framework.h"
#include "GameMode.h"
#include "PhysicsEntityTypeTraits.h"
#include <string>

PhysicsField::PhysicsField()
{
	typePairSwitch.Case<TypeCircle, TypeCircle>( CollisionEffect::CollideType::CircleToCircle() );
	typePairSwitch.Case<TypeRect, TypeCircle>( CollisionEffect::CollideType::ConvexToCircle() );
	typePairSwitch.Case<TypeStar, TypeCircle>( CollisionEffect::CollideType::ConvexToCircle() );
	typePairSwitch.Case<TypeRect, TypeRect>( CollisionEffect::CollideType::ConvexToConvex() );
	typePairSwitch.Case<TypeRect, TypeStar>( CollisionEffect::CollideType::ConvexToConvex() );
	typePairSwitch.Case<TypeStar, TypeStar>( CollisionEffect::CollideType::ConvexToConvex() );
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
	std::wstring fieldCount = std::to_wstring( field.size() );
	TextOut( hdc, 100, 80, fieldCount.c_str(), (int)fieldCount.size() );
}
