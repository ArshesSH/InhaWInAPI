#include "PhysicsEntity.h"

#include "PhysicsEntityTypes.h"
#include "PatternMatchingListener.h"
#include "CollisionEffect.h"

PhysicsEntity::PhysicsEntity( Type type, const Vec2<int>& pos, int id )
	:
	id( id )
{
	std::random_device rd;
	std::mt19937 rng( rd() );
	std::uniform_int_distribution<int> sizeGen( 50, 50 );
	std::uniform_real_distribution<float> speedGen( 100, 300 );
	std::uniform_real_distribution<float> dirXGen( -1, 1 );
	std::uniform_real_distribution<float> dirYGen( -1, 1 );
	std::uniform_real_distribution<float> rotateGen( -0, 0.5 );

	speed = speedGen( rng );
	vel = { dirXGen( rng ), dirYGen( rng ) };
	vel *= speed;
	spinFreq = (float)(rotateGen( rng ) * MathSH::PI);

	if ( type == Type::Rect )
	{
		const int size = sizeGen( rng ) * 2;
		pObj = std::make_unique<Rect<float>>( (float)pos.x, (float)pos.y, size, size );
		pType = std::make_unique<TypeRect>();
	}
	else if ( type == Type::Circle )
	{
		pObj = std::make_unique<Circle<float>>( (float)pos.x, (float)pos.y, sizeGen( rng ) );
		pType = std::make_unique<TypeCircle>();
	}
	else if ( type == Type::Star )
	{
		std::uniform_int_distribution<int> flareGen( 5, 9 );
		const Vec2<float> posStar{ (float)pos.x, (float)pos.y };
		pObj = std::make_unique<Star<float>>( posStar, sizeGen( rng ), flareGen( rng ) );
		pType = std::make_unique<TypeStar>();
	}
}

void PhysicsEntity::Update( float dt, const RECT& walls )
{
	{
		time += dt;
		collideTime += dt;
		MovePos( dt );
		DoWallCollision( walls );
		SetAngle( spinFreq * time );
		pObj->ApplyTransformation( Mat3<float>::Rotation( angle ) );

		if ( objState == State::Collided )
		{
			if ( collideTime >= 0.03f )
			{
				objState = State::Normal;
				collideTime = 0.0f;
			}
		}

	}
}

void PhysicsEntity::Draw( HDC hdc ) const
{
	if ( objState == State::Normal )
	{
		pObj->Draw( hdc );
	}
	else if ( objState == State::Collided )
	{
		pObj->DrawDebug( hdc );
	}
}

void PhysicsEntity::DoEntityCollisionWith( PhysicsEntity& other, const GameMode& curMode )
{
	//if ( !WasCollided() && !other.WasCollided() )
	{
		if ( id != other.id )
		{
			Vec2<float> correctionVec;

			static PatternMatchingListener typePairSwitch;


			if ( curMode == GameMode::Collision )
			{
				typePairSwitch.Case<TypeCircle, TypeCircle>( CollisionEffect::CollideType::CircleToCircle() );
				typePairSwitch.Case<TypeRect, TypeCircle>( CollisionEffect::CollideType::ConvexToCircle() );
				typePairSwitch.Case<TypeStar, TypeCircle>( CollisionEffect::CollideType::ConvexToCircle() );
				typePairSwitch.Case<TypeRect, TypeRect>( CollisionEffect::CollideType::ConvexToConvex() );
				typePairSwitch.Case<TypeRect, TypeStar>( CollisionEffect::CollideType::ConvexToConvex() );
				typePairSwitch.Case<TypeStar, TypeStar>( CollisionEffect::CollideType::ConvexToConvex() );
				typePairSwitch.Switch( *this, other );
			}

			//if ( objType == Type::Circle )
			//{
			//	/*
			//	if ( other.objType == Type::Circle )
			//	{
			//		if ( CheckCircleOverlap( *this, other ) )
			//		{
			//			// Displace this and other
			//			const Vec2<float> distVec = GetCenter() - other.GetCenter();
			//			const float distance = distVec.GetLength();
			//			const float ovelapDist = (distance - GetSize() - other.GetSize()) * 0.5f;
			//			const Vec2<float> distOverlapVec = distVec.GetNormalized() * ovelapDist;
			//			SetCenter( GetCenter() - distOverlapVec );
			//			other.SetCenter( other.GetCenter() + distOverlapVec );

			//			std::swap( vel, other.vel );

			//			objState = State::Collided;
			//			other.objState = State::Collided;
			//		}
			//	}
			//	*/
			//	 if ( other.objType == Type::Rect )
			//	{
			//		if ( CheckConvexOverlapWithCircle( other, *this ) )
			//		{
			//			objState = State::Collided;
			//			other.objState = State::Collided;
			//		}
			//	}
			//}
			//else
			//{
			//	if ( other.objType == Type::Circle )
			//	{
			//		if ( CheckConvexOverlapWithCircle( *this, other ) )
			//		{
			//			objState = State::Collided;
			//			other.objState = State::Collided;
			//		}
			//	}
			//	else
			//	{
			//		if ( CheckConvexOverlapWithConvex( *this, other ) )
			//		{
			//			objState = State::Collided;
			//			other.objState = State::Collided;
			//		}
			//	}
			//}
		}
	}

}

bool PhysicsEntity::CheckCircleOverlap( const PhysicsEntity& e1, const PhysicsEntity& e2 ) const
{
	Circle<float> c1( e1.GetCenter(), e1.GetOuterRadius() );
	Circle<float> c2( e2.GetCenter(), e2.GetOuterRadius() );

	const Vec2<float> distance = c1.GetCenter() - c2.GetCenter();
	const float sumOfRadius = c1.GetRadius() + c2.GetRadius();
	return fabs( distance.x * distance.x + distance.y * distance.y ) < sumOfRadius * sumOfRadius;
}
