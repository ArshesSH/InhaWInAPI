#pragma once

#include "framework.h"
#include <vector>
#include <memory>
#include "PhysicsEntity.h"

class PhysicsField
{

public:
	PhysicsField() = default;
	
	void AddCircle(const Vec2<int>& pos)
	{
		field.emplace_back( PhysicsEntity( PhysicsEntity::Type::Circle, pos ) );
	}

	void AddRect( const Vec2<int>& pos )
	{
		field.emplace_back( PhysicsEntity( PhysicsEntity::Type::Rect, pos ) );
	}
	void AddStar( const Vec2<int>& pos )
	{
		field.emplace_back( PhysicsEntity( PhysicsEntity::Type::Star, pos ) );
	}


	void Update(float dt, const RECT& w)
	{
		for ( auto& e : field )
		{
			e.Update(dt, w);
			for ( auto& other : field )
			{
				e.DoEntityCollisionWith( other );
			}
		}
	}

	void Draw(HDC hdc) const
	{
		for ( const auto& e : field )
		{
			e.Draw( hdc );
		}
	}

private:
	void DoEntityCollision(PhysicsEntity& e)
	{
		for ( auto& target : field )
		{
			// For Circle
			if ( e.IsCollideWith(target) && e != target && !e.GetCollide() && !target.GetCollide() )
			{
				// Get this and target's distance Vector and normal Vec
				const Vec2<float> distVec = target.GetCenter() - e.GetCenter();
				const Vec2<float> normalVec = distVec.GetNormalRightVec2().GetNormalized();

				// Calc half Distance and Move Center of this and target
				const float eSize = e.GetSize();
				const float targetSize = target.GetSize();
				const float halfDistance = (eSize + targetSize - distVec.GetLength()) / 2;

				e.SetCenter( e.GetCenter() - distVec.GetNormalized() * halfDistance );
				target.SetCenter( target.GetCenter() + distVec.GetNormalized() * halfDistance );

				// Calc Velocity from dist-normal Vec
				target.SetVelCollisionBy( normalVec );
				e.SetVelCollisionBy( normalVec );
					
				// Set Collide to true for avoid ovelap (Update to false at Entity Update)
				e.SetCollide();
				target.SetCollide();
			}
		}
	}

private:
	std::vector<PhysicsEntity> field;
};
