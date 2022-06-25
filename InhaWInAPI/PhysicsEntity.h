#pragma once

#include "GeometricObject.h"
#include "Mat3.h"
#include <memory>
#include <random>

class PhysicsEntity
{
public:
	enum class Type
	{
		Rect,
		Circle,
		Star
	};

public:
	enum class State
	{
		Normal,
		Collided,
		NeedToSplit,
		NeedToScaleUP,
		NeedToDestroy
	};

public:
	PhysicsEntity( PhysicsEntity::Type type, const Vec2<int>& pos, int id )
		:
		id( id ),
		objType( type )
	{
		std::random_device rd;
		std::mt19937 rng( rd() );
		std::uniform_int_distribution<int> sizeGen( 20, 50 );
		std::uniform_real_distribution<float> speedGen( 200, 300 );
		std::uniform_real_distribution<float> dirXGen( -1, 1 );
		std::uniform_real_distribution<float> dirYGen( -1, 1 );
		std::uniform_real_distribution<float> rotateGen( -2, 2 );

		speed = speedGen( rng );
		vel = { dirXGen( rng ), dirYGen( rng ) };
		vel *= speed;
		spinFreq = (float)(rotateGen( rng ) * MathSH::PI);

		if ( type == Type::Rect )
		{
			const int size = sizeGen( rng ) * 2;
			pObj = std::make_unique<Rect<float>>( (float)pos.x, (float)pos.y, size, size );
		}
		else if ( type == Type::Circle )
		{
			pObj = std::make_unique<Circle<float>>( (float)pos.x, (float)pos.y, sizeGen( rng ) );
		}
		else if ( type == Type::Star )
		{
			std::uniform_int_distribution<int> flareGen( 5, 12 );
			const Vec2<float> posStar{ (float)pos.x, (float)pos.y };
			pObj = std::make_unique<Star<float>>( posStar, sizeGen( rng ), flareGen( rng ) );
		}
	}

	void Update(float dt, const RECT& walls)
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

	void Draw(HDC hdc) const
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

	void SetVelCollisionByTwoPointLine( const Vec2<float>& lhs, const Vec2<float>& rhs )
	{
		const Vec2<float> lineDirect = (lhs - rhs).GetNormalized();
		const Vec2<float> changedVel = lineDirect * (vel * lineDirect) * 2.0f - vel;
		vel = changedVel;
	}
	void SetVelCollisionBy( const Vec2<float>& line )
	{
		vel = line * (vel * line) * 2.0f - vel;
	}

	bool IsCollideWith( const PhysicsEntity& other )
	{
		return pObj->IsOverlapWith( *(other.pObj) );
	}

	bool IsCollideWith_SAT( const PhysicsEntity& other )
	{
		return pObj->CheckConvexOverlapWithConvex( *(other.pObj) );
	}

	Vec2<float> GetCenter() const
	{
		return pObj->GetCenter();
	}
	bool operator==( const PhysicsEntity& rhs ) const
	{
		return id == rhs.id;
	}
	bool operator!=( const PhysicsEntity& rhs ) const
	{
		return id != rhs.id;
	}
	
	bool WasCollided() const
	{
		return objState == State::Collided;
	}
	void SetCenter(const Vec2<float>& c)
	{
		pObj->SetCenter( c );
	}
	int GetID() const
	{
		return id;
	}
	float GetSize() const
	{
		return pObj->GetSize();
	}
	void SetAngle( float angle_in )
	{
		angle = angle_in;
	}
	float GetAngle() const
	{
		return angle;
	}
	void SetScale(float scale_in)
	{
		scale = scale_in;
	}
	float GetScale() const
	{
		return scale;
	}

	void DoEntityCollisionWith( PhysicsEntity& other )
	{
		if ( id != other.id )
		{
			if ( objType == Type::Circle )
			{
				if ( IsCollideWith( other ) && !WasCollided() && !other.WasCollided() )
				{
					const Vec2<float> distVec = GetCenter() - other.GetCenter();
					const float distance = distVec.GetLength();
					const float ovelapDist = (distance - GetSize() - other.GetSize()) * 0.5f;

					//// Calc Velocity from dist-normal Vec
					//const Vec2<float> normalVec = distVec.GetNormalLeftVec2().GetNormalized();
					//other.SetVelCollisionBy( normalVec );
					//SetVelCollisionBy( normalVec );

					//// Displace this and other
					//const Vec2<float> distOverlapVec = distVec.GetNormalized() * ovelapDist;
					//SetCenter( GetCenter() - distOverlapVec );
					//other.SetCenter( other.GetCenter() + distOverlapVec );
					
					objState = State::Collided;
					other.objState = State::Collided;

				}
			}
			else if ( objType == Type::Rect )
			{
				if ( IsCollideWith( other ) )
				{
					objState = State::Collided;
					other.objState = State::Collided;
				}
			}
		}
	}

private:
	void MovePos( float dt )
	{
		const Vec2<float> curPos = pObj->GetCenter() + (vel * dt);
		pObj->SetCenter( curPos );
	}
	void ReboundX()
	{
		vel.x = -vel.x;
	}
	void ReboundY()
	{
		vel.y = -vel.y;
	}
	void DoWallCollision( const RECT& walls )
	{
		if ( objType == Type::Rect )
		{
			const Vec2<float> topLeftVec{ (float)walls.left, (float)walls.top };
			const Vec2<float> topRightVec{ (float)walls.right, (float)walls.top };
			const Vec2<float> bottomRightVec { (float)walls.right, (float)walls.bottom };
			const Vec2<float> bottomLeftVec { (float)walls.left, (float)walls.bottom };

			const Vec2<float> wallLeftVec = bottomLeftVec - topLeftVec;
			const Vec2<float> wallRightVec = bottomRightVec - topRightVec;
			const Vec2<float> wallTopVec = topRightVec - topLeftVec;
			const Vec2<float> wallBottomVec = bottomRightVec - bottomLeftVec;

			if ( pObj->CheckConvexOverlapWithLine( wallLeftVec ) )
			{
				ReboundX();
			}
			else if ( pObj->CheckConvexOverlapWithLine( wallRightVec ) )
			{
				ReboundX();
			}

			if ( pObj->CheckConvexOverlapWithLine( wallTopVec ) )
			{
				ReboundY();
			}
			else if ( pObj->CheckConvexOverlapWithLine( wallBottomVec ) )
			{
				ReboundY();
			}
			
		}
		else
		{
			const RECT objRect = pObj->GetRECT();
			if ( objRect.left < walls.left )
			{
				pObj->SetCenterX( pObj->GetCenterX() + walls.left - objRect.left );
				ReboundX();
			}
			else if ( objRect.right > walls.right )
			{
				pObj->SetCenterX( pObj->GetCenterX() + (walls.right - objRect.right) );
				ReboundX();
			}

			if ( objRect.top < walls.top )
			{
				pObj->SetCenterY( pObj->GetCenterY() + walls.top - objRect.top );
				ReboundY();
			}
			else if ( objRect.bottom > walls.bottom )
			{
				pObj->SetCenterY( pObj->GetCenterY() + (walls.bottom - objRect.bottom) );
				ReboundY();
			}
		}
	}
	void ApplyTransformation( const Mat3<float>& transformation_in )
	{
		transform = transformation_in * transform;
	}

	bool CheckVerticesSAT( const PhysicsEntity& shape1, const PhysicsEntity& shape2 ) const
	{
		auto shape1Vertices = shape1.pObj->GetVertices();
		auto shape1VerticesSize = shape1Vertices.size();
		auto shape2Vertices = shape2.pObj->GetVertices();

		for ( int vIdx = 0; vIdx < shape1VerticesSize; ++vIdx )
		{
			const int vIdxNext = (vIdx + 1) % shape1VerticesSize;
			Vec2<float> axisProj = (shape1Vertices[vIdx] - shape1Vertices[vIdxNext]).GetNormalLeftVec2();

			float minThis = INFINITY;
			float maxThis = -INFINITY;
			for ( auto e : shape1Vertices )
			{
				const float p = e * axisProj;
				minThis = (std::min)(minThis, p);
				maxThis = (std::max)(maxThis, p);
			}

			float minOther = INFINITY;
			float maxOther = -INFINITY;
			for ( auto e : shape2Vertices )
			{
				const float p = e * axisProj;
				minOther = (std::min)(minOther, p);
				maxOther = (std::max)(maxOther, p);
			}

			if ( !(maxOther >= minThis && maxThis >= minOther) )
			{
				return false;
			}
		}
		return true;
	}

	bool CheckCircleOverlap( const Circle<float>& c1, const Circle<float>& c2 ) const
	{
		const Vec2<float> distance = c1.GetCenter() - c2.GetCenter();
		const float sumOfRadius = c1.GetRadius() + c2.GetRadius();
		return fabs( distance.x * distance.x + distance.y * distance.y ) < sumOfRadius * sumOfRadius;
	}

	bool CheckConvexOverlapWithConvex( const PhysicsEntity& other ) const
	{
		// Create Cricles for Overlap Optimising
		Circle<float> thisOuterCircle( pObj->GetCenter(), pObj->GetRadius() );
		Circle<float> otherOuterCircle( other.pObj->GetCenter(), other.pObj->GetRadius() );

		if ( CheckCircleOverlap( thisOuterCircle, otherOuterCircle ) )
		{
			if ( CheckVerticesSAT( *this, other ) == false )
			{
				return false;
			}
			if ( CheckVerticesSAT( other, *this ) == false )
			{
				return false;
			}
			return true;
		}
		return false;
	}


private:
	std::unique_ptr<GeometricObject<float>> pObj;

	Vec2<float> vel;
	Mat3<float> transform = Mat3<float>::Identity();
	float speed;
	float scale = 1.0f;
	float angle = 0.0f;
	float spinFreq = 0.0f;
	float time = 0.0f;
	float collideTime = 0.0f;
	int id;
	Type objType;
	State objState = State::Normal;
};

