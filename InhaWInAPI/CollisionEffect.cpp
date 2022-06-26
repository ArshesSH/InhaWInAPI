#include "CollisionEffect.h"

#include <algorithm>

void CollisionEffect::CircleToCircle::operator()( PhysicsEntity& e1, PhysicsEntity& e2 )
{
	if ( CheckCircleOverlap( e1, e2 ) )
	{
		// Displace this and other
		const Vec2<float> distVec = e1.GetCenter() - e2.GetCenter();
		const float distance = distVec.GetLength();
		const float ovelapDist = (distance - e1.GetSize() - e2.GetSize()) * 0.5f;
		const Vec2<float> distOverlapVec = distVec.GetNormalized() * ovelapDist;
		e1.SetCenter( e1.GetCenter() - distOverlapVec );
		e2.SetCenter( e2.GetCenter() + distOverlapVec );
		
		auto tmpVel = e1.GetVelocity();
		e1.SetVelocity( e2.GetVelocity() );
		e2.SetVelocity( tmpVel );

		e1.SetState( PhysicsEntity::State::Collided );
		e2.SetState( PhysicsEntity::State::Collided );
	}
}
