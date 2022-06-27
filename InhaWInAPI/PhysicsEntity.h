#pragma once

#include "GeometricObject.h"
#include "Mat3.h"
#include <memory>
#include <random>
#include "MathSH.h"
#include "GameMode.h"


class PhysicsEntity
{
public:
	enum class Type
	{
		Circle,
		Rect,
		Star
	};

	class TypeTrait
	{
	public:
		virtual ~TypeTrait() = default;
		virtual Type GetType() const = 0;
		virtual std::unique_ptr<TypeTrait> Clone() const = 0;
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
	PhysicsEntity( Type type, const Vec2<int>& pos, int id );

	bool operator==( const PhysicsEntity& rhs ) const;
	bool operator!=( const PhysicsEntity& rhs ) const;

	void Update( float dt, const RECT& walls );
	void Draw( HDC hdc ) const;

	Vec2<float> GetCenter() const;
	float GetSize() const;
	float GetOuterRadius() const;
	float GetAngle() const;
	float GetScale() const;
	const TypeTrait& GetEntityType() const;
	Vec2<float> GetVelocity() const;
	std::vector<Vec2<float>> GetVertices() const;
	bool WasCollided() const;

	void SetCenter( const Vec2<float>& c );
	void SetAngle( float angle_in );
	void SetScale( float scale_in );
	void SetVelocity( const Vec2<float>& v );
	void SetState( const State& s );

	void DoEntityCollisionWith( PhysicsEntity& other, const GameMode& curMode, class PatternMatchingListener& listener );

private:
	void ApplyTransformation( const Mat3<float>& transformation_in );

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
		if ( pType->GetType() == Type::Rect )
		{
			const Vec2<float> topLeftVec{ (float)walls.left, (float)walls.top };
			const Vec2<float> bottomRightVec{ (float)walls.right, (float)walls.bottom };

			if ( CheckConvexOverlapWithborder( topLeftVec, bottomRightVec ) )
			{
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


	bool CheckConvexOverlapWithborder( const Vec2<float>& topLeft, const Vec2<float>& bottomRight )
	{
		// Create Normalized Horizontal and Vertical Window Sized Vectors
		const Vec2<float> NormalizedHorizontal = Vec2<float>( bottomRight.x - topLeft.x, topLeft.y ).GetNormalized();
		const Vec2<float> NormalizedVertical = Vec2<float>( topLeft.x, bottomRight.y - topLeft.y ).GetNormalized();

		// Set Projection vals
		float minHorizon = INFINITY;
		float maxHorizon = -INFINITY;
		float minVertical = INFINITY;
		float maxVertical = -INFINITY;

		std::vector<Vec2<float>> vertices = pObj->GetVertices();

		for ( auto e : vertices )
		{
			// Check Horizontal
			const float pHorizon = e * NormalizedHorizontal;
			minHorizon = (std::min)(minHorizon, pHorizon);
			maxHorizon = (std::max)(maxHorizon, pHorizon);

			// Case Left Collision
			if ( minHorizon < topLeft.x )
			{
				const Vec2<float> minimumTranslateVec = NormalizedHorizontal * (topLeft.x - e.x);
				SetCenter( GetCenter() + minimumTranslateVec );
				ReboundX();
				return true;
			}
			// Case Right Collision
			else if ( bottomRight.x < maxHorizon )
			{
				const Vec2<float> minimumTranslateVec = NormalizedHorizontal * (bottomRight.x - e.x);
				SetCenter( GetCenter() + minimumTranslateVec );
				ReboundX();
				return true;
			}

			// Check Vertical
			const float pVertical = e * NormalizedVertical;
			minVertical = (std::min)(minVertical, pVertical);
			maxVertical = (std::max)(maxVertical, pVertical);

			// Case Top Collision
			if ( minVertical < topLeft.y )
			{
				const Vec2<float> minimumTranslateVec = NormalizedVertical * (topLeft.y - e.y);
				SetCenter( GetCenter() + minimumTranslateVec );
				ReboundY();
				return true;
			}
			// Case Bottom Collision
			else if ( bottomRight.y < maxVertical )
			{
				const Vec2<float> minimumTranslateVec = NormalizedVertical * (bottomRight.y - e.y);
				SetCenter( GetCenter() + minimumTranslateVec );
				ReboundY();
				return true;
			}
		}
		return false;
	}

private:
	std::unique_ptr<GeometricObject<float>> pObj;
	std::unique_ptr<TypeTrait> pType;
	int id;
	Vec2<float> vel;
	Mat3<float> transform = Mat3<float>::Identity();
	float speed;
	float scale = 1.0f;
	float angle = 0.0f;
	float spinFreq = 0.0f;
	float time = 0.0f;
	float collideTime = 0.0f;
	State objState = State::Normal;
};