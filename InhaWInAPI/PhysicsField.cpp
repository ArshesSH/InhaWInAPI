#include "PhysicsField.h"

#include "CollisionEffect.h"
#include "CollisionManager.h"
#include "framework.h"
#include "GameMode.h"
#include "PhysicsEntityTypeTraits.h"
#include <string>

PhysicsField::PhysicsField( )
{
	CollisionManager collisionManager;
	typePairSwitch.Case<TypeCircle, TypeCircle>( [&](PhysicsEntity& circle1, PhysicsEntity& circle2)
		{
			collisionManager.CircleToCircle( circle1, circle2 );
		}
	);
	typePairSwitch.Case<TypeRect, TypeCircle>( [&]( PhysicsEntity& convex, PhysicsEntity& circle )
		{
			Vec2<float> minTranslateVec;
			if ( collisionManager.CheckConvexOverlapWitchCircle( convex, circle, minTranslateVec ) )
			{
				collisionManager.CenterCorrection( convex, minTranslateVec );
				collisionManager.CenterCorrection( circle, minTranslateVec * -1.0f );

				collisionManager.SwapVelocity( convex, circle );

				if ( curMode == GameMode::Collision )
				{
					collisionManager.SetCollisionStateBoth( convex, circle );
				}
				else if ( curMode == GameMode::Combine )
				{
					convex.SetStateToDestroy();
					circle.SetStateToScaleUP();
				}
				else
				{
					convex.SetStateToSplit();
					circle.SetStateToCollide();
				}
			}
		}
	);
	typePairSwitch.Case<TypeStar, TypeCircle>( [&]( PhysicsEntity& convex, PhysicsEntity& circle )
		{
			Vec2<float> minTranslateVec;
			if ( collisionManager.CheckConvexOverlapWitchCircle( convex, circle, minTranslateVec ) )
			{
				collisionManager.CenterCorrection( convex, minTranslateVec );
				collisionManager.CenterCorrection( circle, minTranslateVec * -1.0f );

				collisionManager.SwapVelocity( convex, circle );

				if ( curMode == GameMode::Collision )
				{
					collisionManager.SetCollisionStateBoth( convex, circle );
				}
				else if ( curMode == GameMode::Combine )
				{
					circle.SetStateToDestroy();
					convex.SetStateToScaleUP();
				}
				else
				{
					circle.SetStateToSplit();
					convex.SetStateToCollide();
				}
			}
		}
	);
	typePairSwitch.Case<TypeRect, TypeRect>( [&]( PhysicsEntity& convex1, PhysicsEntity& convex2 )
		{
			collisionManager.ConvexToConvex( convex1, convex2 );
		}
	);
	typePairSwitch.Case<TypeRect, TypeStar>( [&]( PhysicsEntity& convex1, PhysicsEntity& convex2 )
		{
			Vec2<float> minTranslateVecConvex1;
			Vec2<float> minTranslateVecConvex2;

			if ( collisionManager.CheckConvexOverlapWithConvex( convex1, convex2, minTranslateVecConvex1, minTranslateVecConvex2 ) )
			{
				// Set Center Correction
				collisionManager.CenterCorrection( convex1, minTranslateVecConvex1 );
				collisionManager.CenterCorrection( convex2, minTranslateVecConvex2 );

				collisionManager.SwapVelocity( convex1, convex2 );

				if ( curMode == GameMode::Collision )
				{
					collisionManager.SetCollisionStateBoth( convex1, convex2 );
				}
				else if ( curMode == GameMode::Combine )
				{
					convex2.SetStateToDestroy();
					convex1.SetStateToScaleUP();
				}
				else
				{
					convex2.SetStateToSplit();
					convex1.SetStateToCollide();
				}
			}
		} 
	);
	typePairSwitch.Case<TypeStar, TypeStar>( [&]( PhysicsEntity& convex1, PhysicsEntity& convex2 )
		{
			collisionManager.ConvexToConvex( convex1, convex2 );
		}
	);
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

void PhysicsField::AddRandomly( const Vec2<int>& pos )
{
	const int id = (int)field.size();
	std::random_device rd;
	std::mt19937 rng( rd() );
	std::uniform_int_distribution<int> typeGen( (int)PhysicsEntity::Type::Circle, (int)PhysicsEntity::Type::Star );
	switch ( typeGen(rng) )
	{
	case (int)PhysicsEntity::Type::Circle:
		field.emplace_back( PhysicsEntity( PhysicsEntity::Type::Circle, pos, id ) );
		break;
	case (int)PhysicsEntity::Type::Rect:
		field.emplace_back( PhysicsEntity( PhysicsEntity::Type::Rect, pos, id ) );
		break;
	case (int)PhysicsEntity::Type::Star:
		field.emplace_back( PhysicsEntity( PhysicsEntity::Type::Star, pos, id ) );
		break;
	}
}

void PhysicsField::Update( float dt, const RECT& w, const GameMode& curMode_in )
{
	curMode = curMode_in;
	for ( auto it = field.begin(); it != field.end(); ++it )
	{
		it->Update(dt, w);
		for ( auto itOther = it; itOther != field.end(); ++itOther )
		{
			it->DoEntityCollisionWith( *itOther, curMode, typePairSwitch );
		}
	}


	// For Debug
	if ( curMode == GameMode::Collision )
	{
		modeOutput = L"Collision Mode";
	}
	else if ( curMode == GameMode::Combine )
	{
		modeOutput = L"Combine Mode";
	}
	else if ( curMode == GameMode::Split )
	{
		modeOutput = L"Split Mode";
	}
}

void PhysicsField::Draw( HDC hdc ) const
{
	for ( const auto& e : field )
	{
		e.Draw( hdc );
	}

	Debug( hdc );
}

void PhysicsField::Debug( HDC hdc ) const
{
	std::wstring fieldCount = std::to_wstring( field.size() );
	TextOut( hdc, 100, 60, fieldCount.c_str(), (int)fieldCount.size() );
	TextOut( hdc, 100, 80, modeOutput.c_str(), (int)modeOutput.size() );
}
