#include "PhysicsEntity.h"

#include "TemplatedSwitch.h"
#include "CollisionEffect.h"

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

			TemplateSwitch typePairSwitch;


			if ( curMode == GameMode::Collision )
			{
				typePairSwitch.Case<PhysicsEntity, PhysicsEntity>( CollisionEffect::CircleToCircle() );
				typePairSwitch.Switch( *this, other );
			}

			if ( objType == Type::Circle )
			{
				/*
				if ( other.objType == Type::Circle )
				{
					if ( CheckCircleOverlap( *this, other ) )
					{
						// Displace this and other
						const Vec2<float> distVec = GetCenter() - other.GetCenter();
						const float distance = distVec.GetLength();
						const float ovelapDist = (distance - GetSize() - other.GetSize()) * 0.5f;
						const Vec2<float> distOverlapVec = distVec.GetNormalized() * ovelapDist;
						SetCenter( GetCenter() - distOverlapVec );
						other.SetCenter( other.GetCenter() + distOverlapVec );

						std::swap( vel, other.vel );

						objState = State::Collided;
						other.objState = State::Collided;
					}
				}
				*/
				 if ( other.objType == Type::Rect )
				{
					if ( CheckConvexOverlapWithCircle( other, *this ) )
					{
						objState = State::Collided;
						other.objState = State::Collided;
					}
				}
			}
			else
			{
				if ( other.objType == Type::Circle )
				{
					if ( CheckConvexOverlapWithCircle( *this, other ) )
					{
						objState = State::Collided;
						other.objState = State::Collided;
					}
				}
				else
				{
					if ( CheckConvexOverlapWithConvex( *this, other ) )
					{
						objState = State::Collided;
						other.objState = State::Collided;
					}
				}
			}
		}
	}

}
