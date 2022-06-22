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
		std::uniform_real_distribution<float> speedGen( 100, 500 );
		std::uniform_real_distribution<float> dirXGen( -1, 1 );
		std::uniform_real_distribution<float> dirYGen( -1, 1 );

		speed = speedGen( rng );
		vel = { dirXGen( rng ), dirYGen( rng ) };
		vel *= speed;

		if ( type == Type::Rect )
		{
			const int size = sizeGen( rng );
			pObj = std::make_unique<Rect<float>>( (float)pos.x, (float)pos.y, size, size );
		}
		else if ( type == Type::Circle )
		{
			pObj = std::make_unique<Circle<float>>( (float)pos.x, (float)pos.y, sizeGen( rng ) );
		}
		else if ( type == Type::Star )
		{
			std::uniform_int_distribution<int> flareGen( 5, 10 );
			const Vec2<float> posStar{ (float)pos.x, (float)pos.y };
			pObj = std::make_unique<Star<float>>( posStar, sizeGen( rng ), flareGen( rng ) );
		}
	}

	void Update(float dt, const RECT& walls)
	{
		MovePos( dt );
		DoWallCollision( walls );
		/*
		if collide,
		*/

	}

	void Draw(HDC hdc) const
	{
		pObj->Draw( hdc );
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
			//const Vec2<float> topLeft( walls.left, walls.top );
			//const Vec2<float> bottomLeft( walls.left, walls.bottom );
			//SetVelCollisionByLine( bottomLeft, topLeft );

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
	void SetVelCollisionByLine(const Vec2<float>& lhs, const Vec2<float>& rhs)
	{
		const Vec2<float> lineDirect = (lhs - rhs).GetNormalized();
		const Vec2<float> changedVel = lineDirect * (vel * lineDirect) * 2.0f - vel;
		vel = changedVel;
	}

private:
	std::unique_ptr<GeometricObject<float>> pObj;
	Vec2<float> vel;
	float speed;
};

