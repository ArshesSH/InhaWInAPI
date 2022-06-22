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
	PhysicsEntity( PhysicsEntity::Type type )
	{
		std::random_device rd;
		std::mt19937 rng( rd() );
		std::uniform_int_distribution<int> sizeGen( 20, 50 );
		std::uniform_int_distribution<int> posXGen( 50, 1000 );
		std::uniform_int_distribution<int> posYGen( 50, 1000 );
		std::uniform_real_distribution<float> speedGen( 0, 10 );
		std::uniform_real_distribution<float> dirXGen( 0, 1 );
		std::uniform_real_distribution<float> dirYGen( 0, 1 );

		speed = speedGen( rng );
		dir = { dirXGen( rng ), dirYGen( rng ) };
		dir *= speed;

		if ( type == Type::Rect )
		{
			const int size = sizeGen( rng );
			pObj = std::make_unique<Rect<float>>( posXGen( rng ), posYGen( rng ), size, size );
		}
		else if ( type == Type::Circle )
		{
			pObj = std::make_unique<Circle<float>>( posXGen( rng ), posYGen( rng ), sizeGen( rng ) );
		}
		else if ( type == Type::Star )
		{
			std::uniform_int_distribution<int> flareGen( 5, 10 );
			const Vec2<float> pos{ (float)posXGen( rng ), (float)posYGen( rng ) };
			pObj = std::make_unique<Star<float>>( pos, sizeGen( rng ), flareGen( rng ) );
		}
	}

	void Update(float dt)
	{
		const Vec2<float> curPos = pObj->GetCenter();
		pObj->SetCenter( curPos + (dir * dt) );
	}

	void Draw(HDC hdc) const
	{
		pObj->Draw( hdc );
	}

private:
	std::unique_ptr<GeometricObject<float>> pObj;
	Vec2<float> dir;
	float speed;
};

