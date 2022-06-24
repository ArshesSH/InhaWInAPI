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
	PhysicsEntity( PhysicsEntity::Type type, const Vec2<int>& pos )
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
		if ( isCollide )
		{
			if ( collideTime >= 0.03f )
			{
				isCollide = false;
				collideTime = 0.0f;
			}
		}
	}

	void Draw(HDC hdc) const
	{
		pObj->Draw( hdc );
		//pObj->DrawTransformed( hdc, Mat3<float>::Rotation( angle ) * Mat3<float>::Scale( scale ) );
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
		return pObj->GeometricOverlap_SAT( *(other.pObj) );
	}

	Vec2<float> GetCenter() const
	{
		return pObj->GetCenter();
	}

	bool GetCollide() const
	{
		return isCollide;
	}
	void SetCollide( bool state = true )
	{
		isCollide = state;
	}

	bool operator!=( const PhysicsEntity& rhs ) const
	{
		return pObj != rhs.pObj;
	}
	
	void SetCenter(const Vec2<float>& c)
	{
		pObj->SetCenter( c );
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
		// For Circle
		if ( IsCollideWith( other ) && *this != other && !GetCollide() && !other.GetCollide() )
		{
			// Get this and target's distance Vector and normal Vec
			const Vec2<float> distVec = other.GetCenter() - GetCenter();
			const Vec2<float> normalVec = distVec.GetNormalRightVec2().GetNormalized();

			// Calc half Distance and Move Center of this and target
			const float thisSize = GetSize();
			const float otherSize = other.GetSize();
			const float halfDistance = (thisSize + otherSize - distVec.GetLength()) / 2;

			SetCenter( GetCenter() - distVec.GetNormalized() * halfDistance );
			other.SetCenter( other.GetCenter() + distVec.GetNormalized() * halfDistance );

			// Calc Velocity from dist-normal Vec
			other.SetVelCollisionBy( normalVec );
			SetVelCollisionBy( normalVec );

			// Set Collide to true for avoid ovelap (Update to false at Entity Update)
			SetCollide();
			other.SetCollide();
		}

		if ( IsCollideWith_SAT( other ) && *this != other )
		{
			// Get this and target's distance Vector and normal Vec
			const Vec2<float> distVec = other.GetCenter() - GetCenter();
			const Vec2<float> normalVec = distVec.GetNormalRightVec2().GetNormalized();

			// Calc half Distance and Move Center of this and target
			const float thisSize = GetSize();
			const float otherSize = other.GetSize();
			const float halfDistance = (thisSize + otherSize - distVec.GetLength()) / 2;

			SetCenter( GetCenter() - distVec.GetNormalized() * halfDistance );
			other.SetCenter( other.GetCenter() + distVec.GetNormalized() * halfDistance );

			// Calc Velocity from dist-normal Vec
			other.SetVelCollisionBy( normalVec );
			SetVelCollisionBy( normalVec );
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
		const RECT objRect = pObj->GetRECT();

		if ( objRect.left < walls.left )
		{
			pObj->SetCenterX( pObj->GetCenterX() + walls.left - objRect.left );
			ReboundX();
		}
		else if ( objRect.right > walls.right )
		{
			pObj->SetCenterX( pObj->GetCenterX() + ( walls.right - objRect.right) );
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
	void ApplyTransformation( const Mat3<float>& transformation_in )
	{
		transform = transformation_in * transform;
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
	bool isCollide = false;
};

