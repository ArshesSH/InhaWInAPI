#include "PhysicsField.h"

#include "CollisionManager.h"
#include "framework.h"
#include "GameMode.h"
#include "PhysicsEntityTypeTraits.h"
#include <string>

PhysicsField::PhysicsField( )
{
	CollisionManager collisionManager;
	typePairSwitch.Case<TypeCircle, TypeCircle>( [&]( PhysicsEntity& circle1, PhysicsEntity& circle2 )
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
					convex.SetStateShouldDestroy();
					circle.SetStateShouldScaleUP();
					circle.SetSizeForAdd( convex.GetSize());
				}
				else
				{
					convex.SetStateShouldSplit();
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
					circle.SetStateShouldDestroy();
					convex.SetStateShouldScaleUP();
					convex.SetSizeForAdd( circle.GetSize() );
				}
				else
				{
					circle.SetStateShouldSplit();
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
					convex2.SetStateShouldDestroy();
					convex1.SetStateShouldScaleUP();
					convex1.SetSizeForAdd( convex2.GetSize() );
				}
				else
				{
					convex2.SetStateShouldSplit();
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
			if ( it != itOther )
			{
				Vec2<float> correctionVec;
				typePairSwitch.Switch( *it, *itOther );
			}
		}
	}

	ScaleUpEntity();
	SplitEntity();
	CheckSizeToDestroy();
	DestroyEntity();

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

void PhysicsField::DestroyEntity()
{
	const auto new_end = std::remove_if( field.begin(), field.end(),
		[]( const PhysicsEntity& entity )
		{
			return entity.GetStateShouldDestroy();
		}
	);
	field.erase( new_end, field.end() );
}

void PhysicsField::CheckSizeToDestroy()
{
	auto i = std::find_if( field.begin(), field.end(),
		[&]( PhysicsEntity& entity )
		{
			bool flag = false;

			const auto size = entity.GetSize();
			if ( size >= scaleUpLimit || size <= scaleDownLimit )
			{
				entity.SetStateShouldDestroy();
			}
			return flag;
		}
	);
}

void PhysicsField::ScaleUpEntity()
{
	auto i = std::find_if( field.begin(), field.end(),
		[&]( PhysicsEntity& entity )
		{
			bool flag = false;
			if ( entity.GetStateShouldScaleUp() )
			{
				flag = true;
				
				entity.AddSize( entity.GetSizeForAdd() );
				entity.SetStateToNormal();
			}
			return flag;
		}
	);
}

void PhysicsField::SplitEntity()
{
	Vec2<int> pos;
	Vec2<float> vel;
	PhysicsEntity::Type type;
	int size;
	float angle;
	float spinFreq;
	int nFlares;

	auto i = std::find_if( field.begin(), field.end(),
		[&]( PhysicsEntity& entity )
		{
			bool flag = false;

			if ( entity.GetStateShouldSplit() )
			{
				flag = true;

				type = entity.GetType();
				pos = { (int)entity.GetCenterX(), (int)entity.GetCenterY() };
				vel = entity.GetVelocity();
				size = entity.GetSize() / 2;
				angle = entity.GetAngle();
				spinFreq = entity.GetSpinFreq();
				nFlares = entity.GetFlareCount();

				entity.SetStateToNormal();
			}
			return flag;
		}
	);
	if ( i != field.end() )
	{
		field.erase( i );
		field.emplace_back( type, Vec2<int>{ pos.x - size/2, pos.y + size/2 }, field.size(), (float)size, vel, angle, spinFreq, nFlares );
		field.emplace_back( type, Vec2<int>{ pos.x + size/2, pos.y + size/2 }, field.size(), (float)size, vel, angle, spinFreq, nFlares );
		field.emplace_back( type, Vec2<int>{ pos.x + size/2, pos.y - size/2 }, field.size(), (float)size, vel, angle, spinFreq, nFlares );
		field.emplace_back( type, Vec2<int>{ pos.x - size/2, pos.y - size/2 }, field.size(), (float)size, vel, angle, spinFreq, nFlares );
	}
}
