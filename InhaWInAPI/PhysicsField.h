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

	void Update(float dt, const RECT& w)
	{
		for ( auto& e : field )
		{
			e.Update(dt, w);
			DoCircleCollision( e );
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
	void DoCircleCollision(PhysicsEntity& e)
	{
		for ( auto& target : field )
		{
			if ( e.IsCollideWith(target) && e != target && !e.GetCollide() && !target.GetCollide() )
			{
				const Vec2<float> distVecToE = target.GetCenter() - e.GetCenter();
				const Vec2<float> distVecToTarget = e.GetCenter() - target.GetCenter();


				const Vec2<float> normalVec = distVecToE.GetNormalRightVec2().GetNormalized();
					
				target.SetVelCollisionBy( normalVec );
				e.SetVelCollisionBy( normalVec );
				const float eSize = e.GetSize();
				const float targetSize = target.GetSize();
				const float halfDistance = (eSize + targetSize - distVecToE.GetLength()) / 2;
				e.SetCenter( e.GetCenter() - distVecToE.GetNormalized() * halfDistance );
				target.SetCenter( target.GetCenter() - distVecToTarget.GetNormalized() * halfDistance );
				
				e.SetCollide();
				target.SetCollide();
			}
		}
	}

private:
	std::vector<PhysicsEntity> field;
};
