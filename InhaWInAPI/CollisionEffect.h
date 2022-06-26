#pragma once

#include "PhysicsEntity.h"
namespace CollisionEffect
{
	class CircleToCircle
	{
	public:
		void operator()( PhysicsEntity& e1, PhysicsEntity& e2 );
	private:
		bool CheckCircleOverlap( const PhysicsEntity& e1, const PhysicsEntity& e2 ) const
		{
			Circle<float> c1( e1.GetCenter(), e1.GetOuterRadius() );
			Circle<float> c2( e2.GetCenter(), e2.GetOuterRadius() );

			const Vec2<float> distance = c1.GetCenter() - c2.GetCenter();
			const float sumOfRadius = c1.GetRadius() + c2.GetRadius();
			return fabs( distance.x * distance.x + distance.y * distance.y ) < sumOfRadius * sumOfRadius;
		}
	};
}